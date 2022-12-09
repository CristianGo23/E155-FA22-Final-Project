////////////////////////////////////
// System Verilog for Pinball Machine
// Names: Cristian Gonzalez, Alessandro Maiuolo
// Emails: cgonzalez@g.hmc.edu, amaiuolo@g.hmc.edu
// Last Updated: December 8, 2022
////////////////////////////////////

module top(input logic target, n_reset,
		   input logic [3:0] tunnel_p,
		   input logic lose_arch,
		   input logic enable_FSM,
		   output logic v_1, v_2,
		   output logic [6:0] seg7,
		   output logic [2:0] alert_MCU);
	
	// inverting reset since we want reset on high while using the pullup resistor for the pin
	logic reset;
	assign reset = ~n_reset;
	
	//clk setup
	logic [25:0] counter;
	logic int_osc;
	logic clk;
	HSOSC #(.CLKHF_DIV(2'b01)) 
			hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
			
	always_ff @(posedge int_osc) begin
			counter <= counter + 1;
	end
	
	always_comb begin
			clk <= counter[17];
	end
	
	// multeplexed 7-segment diplay
	logic [3:0] ones, tens, seg7_input;
	assign v_1 = counter[17];
	assign seg7_input = v_2 ? ones : tens;
	assign v_2 = ~v_1;
	sevendecoder dec7(seg7_input, seg7);
	
	//pinball main module
	pinball_main pbm(.clk, .reset, .enable_FSM, .tunnel_p, .target, .lose_arch, .alert_MCU, .ones, .tens);
	
endmodule
module pinball_main(input logic clk, reset, enable_FSM,
	input logic [3:0] tunnel_p,
	input logic target, lose_arch,
	output logic [2:0] alert_MCU,
	output logic [3:0] ones, tens);
	
	logic enable_score;
	
	score_module scorem(.clk, .reset, .ones, .tens, .enable_score(enable_score && enable_FSM));
	pinball_fsm pfsm(.clk, .reset, .enable_FSM, .score({tens, ones}), .target, .tunnel_p, .lose_arch, .alert_MCU, .enable_score);

endmodule

// Module that controls the incrementation for decimal number
module score_module(input logic clk, reset,
		    input logic enable_score,
		    output logic [3:0] ones, tens);
	
	logic [3:0] next_ones, next_tens;
	always_comb
		if (ones == 9) begin
			// reset ones house to 0 and carry the 1 to the tens house
			next_ones = 0;
			next_tens = tens + 1;
		end
		else begin
			// just incrememt ones house if there is no carry
			next_ones = ones + 1;
			next_tens = tens;
		end
	
	// update score on clock edge when score module is enabled
	flopenr #(8) score_flop(clk, reset, enable_score, {next_ones, next_tens}, {ones, tens});
	
endmodule
module pinball_fsm(input logic clk, reset, enable_FSM,
				   input logic [7:0] score,									 
				   input logic target,
									 input logic [3:0] tunnel_p,
									 input logic lose_arch,
									 output logic [2:0] alert_MCU,
									 output logic enable_score);
									 	
	// defining the states for our FSM
	typedef enum logic [3:0] {Sensing_State, Tunnel_P1_Wait, Tunnel_P1, Tunnel_P2_Wait, Tunnel_P2, Tunnel_P3, Tunnel_P3_Wait, Target_State, Target_Wait, Win_State, Lose_State} statetype;
  statetype state, nextstate;

  always_ff @(posedge clk)
	if (reset) state <= Sensing_State;
	else if (enable_FSM) state <= nextstate;
	else state <= state;

  always_comb
	case(state) 
		// detect and drive game based on win, lose, target, and tunnel entry conditions
		Sensing_State: if (score > 8'h07) nextstate = Win_State; // hex is actually dec bc decoder
					   else if (~lose_arch) nextstate = Lose_State;
					   else if (~tunnel_p[1]) nextstate = Tunnel_P1;
					   else if (target) nextstate = Target_State;
					   
					   else nextstate = Sensing_State;
		// stay in P1 for one cycle and then move on to the wait
		Tunnel_P1: 		nextstate = Tunnel_P1_Wait;
		// wait until the ball moves further up the tunnel or exits
		Tunnel_P1_Wait: 	   if (~tunnel_p[2]) nextstate = Tunnel_P2;
					   else if (~tunnel_p[0]) nextstate = Sensing_State;
					   else nextstate = Tunnel_P1_Wait;
		// similar to corres P1
		Tunnel_P2: 		nextstate = Tunnel_P2_Wait;
		// similar to corres P1
		Tunnel_P2_Wait:     if (~tunnel_p[3]) nextstate = Tunnel_P3;
					   else if (~tunnel_p[0]) nextstate = Sensing_State;
					   else nextstate = Tunnel_P2_Wait;
		// similar to corres P1
		Tunnel_P3:     nextstate = Tunnel_P3_Wait;
		// wait until ball exits the tunnel
		Tunnel_P3_Wait: if (~tunnel_p[0]) nextstate = Sensing_State;
						else nextstate = Tunnel_P3_Wait;
		// stay in Target for one cycle and then go to wait
		Target_State:  nextstate = Target_Wait;
		// wait until the button is released until going back sensing
		Target_Wait:   if (target) nextstate = Target_Wait;
					   else nextstate = Sensing_State;
		// game stays in win state until reset
		Win_State:     nextstate = Win_State;
		// game stays in lose state until reset
		Lose_State:    nextstate = Lose_State;
		default: nextstate = Sensing_State;
	endcase
	
	// increment score if in Target or any Tunnel state
	// note: wait states are in the game logic so that any scoring events are awarded exactly 1 point
	assign enable_score = (state == Target_State | state == Tunnel_P1 | state == Tunnel_P2 | state == Tunnel_P3);
	always_comb
		case(state)
			// alert_mcu is a signal to communicate to the MCU the current state of the game
			// the tunnel and corresponding wait states have the same alert_MCU value so the user can know where the ball is
			// the target wait state has a different value so that we can know if the button got stuck
			Sensing_State: alert_MCU = 0;
			Tunnel_P1: alert_MCU = 1;
			Tunnel_P2: alert_MCU = 2;
			Tunnel_P3: alert_MCU = 3;
			Target_State: alert_MCU = 4;
			Win_State: alert_MCU = 5;
			Lose_State: alert_MCU = 6;
			Target_Wait: alert_MCU = 7;
			Tunnel_P1_Wait: alert_MCU = 1;
			Tunnel_P2_Wait: alert_MCU = 2;
			Tunnel_P3_Wait: alert_MCU = 3;
			default: alert_MCU = 0;
		endcase

endmodule

// decode 4 bit hex digit to segment values for display
module sevendecoder(input logic [3:0] switch,
					output logic [6:0] seg);
	always_comb
			case(~switch)
			4'b0000: seg = 7'b0001110;
			4'b0001: seg = 7'b0000100;
			4'b0010: seg = 7'b0100001;
			4'b0011: seg = 7'b1000110;
			4'b0100: seg = 7'b0000011;
			4'b0101: seg = 7'b0001000;
			4'b0110: seg = 7'b0011000;
			4'b0111: seg = 7'b0000000;
			4'b1000: seg = 7'b1111000;
			4'b1001: seg = 7'b0000010;
			4'b1010: seg = 7'b0010010;
			4'b1011: seg = 7'b0011001;
			4'b1100: seg = 7'b0110000;
			4'b1101: seg = 7'b0100100;
			4'b1110: seg = 7'b1111001;
			4'b1111: seg = 7'b1000000;
			default: seg = 7'bxxxxxxx;
		endcase
endmodule


module flopenr #(parameter WIDTH = 8)
              (input  logic             clk, reset, en,
               input  logic [WIDTH-1:0] d, 
               output logic [WIDTH-1:0] q);

  always_ff @(posedge clk)
    if (reset)   q <= 0;
    else if (en) q <= d;
    else q <= q;
endmodule
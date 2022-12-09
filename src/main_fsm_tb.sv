`timescale 10ns/1ns
/////////////////////////////////////////////
// main_fsm_tb
// Tests main fsm for game logic
// Alessandro Maiuolo and Cristian Gonzalez
// 11-3-22
// amaiuolo@hmc.edu cgonzalez@hmc.edu
/////////////////////////////////////////////

module testbench_main_fsm();
    logic clk, reset, target, lose_arch, enable_score;
	logic [7:0] score;									 
	logic [3:0] tunnel_p;
	logic [2:0] alert_MCU;
    
    // device under test
    pinball_fsm dut(.clk, .reset, .target, .enable_score, .score, .tunnel_p, .alert_MCU, .enable_FSM('1), .lose_arch);
    

    
    // generate clock and load signals
    initial 
        forever begin
            clk = 1'b0; #5;
            clk = 1'b1; #5;
        end
        
    initial begin
      reset = 1'b1; #22; reset = 1'b0; //Pulse reset
	  tunnel_p[0] = 1; tunnel_p[1] = 1; tunnel_p[2] = 1; tunnel_p[3] = 1;
    end
	
	//test inputs
	initial begin
	score = 0; 

	// Hits the target for brief moment
	#30; target = 1;
	#5; target = 0;

	// Enters tunnel, and passes through fully
	// Passes through 1st sensor
	#5; tunnel_p[0] = 0;
	// Passes through second sensor
	#5; tunnel_p[0] = 1;
	#5; tunnel_p[1] = 0;
	
	// Passes through third sensor
	#5; tunnel_p[1] = 1;
	#5; tunnel_p[2] = 0;
	
	// Passes through fourth sensor
	#5; tunnel_p[2] = 1;
	#5; tunnel_p[3] = 0;
	#5; tunnel_p[3] = 1;
	#5;
	
	// Enters tunnel, passes the second sensor, but falls back down
	// Passes through 1st sensor
	#5; tunnel_p[0] = 0;
	// Passes through second sensor
	#5; tunnel_p[0] = 1;
	#5; tunnel_p[1] = 0;
	
	// Starts to fall down, not passing third sensor
	#5; tunnel_p[1] = 1;
	#5; tunnel_p[1] = 0;
	#5; tunnel_p[1] = 1;
	#5; tunnel_p[0] = 0;
	
	// Finally falls out of tunnel
	#5; tunnel_p[0] = 1;
	#5;

	//#50; score = 1;
	
	$stop;
end
    
endmodule
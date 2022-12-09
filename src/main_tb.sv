`timescale 10ns/1ns
/////////////////////////////////////////////
// main_fsm_tb
// Tests main fsm for game logic
// Alessandro Maiuolo and Cristian Gonzalez
// 11-3-22
// amaiuolo@hmc.edu cgonzalez@hmc.edu
/////////////////////////////////////////////

module testbench_main();
    logic clk, reset, target, lose_arch, enable_score;
	logic [3:0] ones, tens;									 
	logic [3:0] tunnel_p;
	logic [2:0] alert_MCU;
    
    // device under test
    pinball_main dut(.clk, .reset, .target, .lose_arch, .ones, .tens, .tunnel_p, .alert_MCU, .enable_FSM('1));
    

    
    // generate clock and load signals
    initial 
        forever begin
            clk = 1'b0; #5;
            clk = 1'b1; #5;
        end
        
    initial begin
		//tunnel_p = 4'hff;
	  //tunnel_p[0] = 1; tunnel_p[1] = 1; tunnel_p[2] = 1; tunnel_p[3] = 1;
      reset = 1'b1; #22; reset = 1'b0; //Pulse reset
	  
    end
	
	//test inputs
	initial begin
	target = 0;
	tunnel_p = 4'b1111;
	lose_arch = 0;

	// Hits the target for brief moment
	#10; target = 1;
	#10; target = 0;
	
	// Hits the target for long moment
	#10; target = 1;
	#60; target = 0;

	// Enters tunnel, and passes through fully
	// Passes through 1st sensor
	#10; tunnel_p[0] = 0;
	// Passes through second sensor
	#10; tunnel_p[0] = 1;
	#10; tunnel_p[1] = 0;
	
	// Passes through third sensor
	#10; tunnel_p[1] = 1;
	#10; tunnel_p[2] = 0;
	
	// Passes through fourth sensor
	#10; tunnel_p[2] = 1;
	#10; tunnel_p[3] = 0;
	#10; tunnel_p[3] = 1;
	#10;
	
	// Enters tunnel, passes the second sensor, but falls back down
	// Passes through 1st sensor
	#10; tunnel_p[0] = 0;
	// Passes through second sensor
	#10; tunnel_p[0] = 1;
	#10; tunnel_p[1] = 0;
	
	// Starts to fall down, not passing third sensor
	#10; tunnel_p[1] = 1;
	#150; tunnel_p[1] = 0;
	#10; tunnel_p[1] = 1;
	#10; tunnel_p[0] = 0;
	
	// falls out of tunnel
	#10; tunnel_p[0] = 1;
	#40;
	
	// Hits the target several times
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	#10; target = 1; #20; target = 0;
	

	
	#50; $stop;
end
    
endmodule
`timescale 1ns / 1ps

import "DPI-C" context task c_write_resp;
import "DPI-C" context task accept_transaction;

module testbench;
	export "DPI-C" task sv_write;
	export "DPI-C" task sv_finish;
	reg rst_n;
	reg clk;

	always begin
		#5 clk = ~clk;
	end

	initial begin
		rst_n = 0;
		clk = 0;
		#10;
		rst_n = 1;
	end

	task sv_write;
		input int addr;
		input int data;
		$display("Get on addr %d char %c", addr, data);
		c_write_resp();
	endtask
	
	always @(posedge clk) begin
		if (rst_n) begin
			accept_transaction();
		end
	end

	task sv_finish;
		$finish();
	endtask

endmodule

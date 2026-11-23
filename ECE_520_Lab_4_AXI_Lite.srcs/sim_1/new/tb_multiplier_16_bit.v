`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2026 01:00:27 PM
// Design Name: 
// Module Name: tb_multiplier_16_bit
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module tb_multiplier_16_bit;
    reg clk = 0;
    reg rst_n = 0;
    reg [15:0] a = 0;
    reg [15:0] b = 0;
    wire [31:0] prod;
    integer i;

    multiplier_16_bit dut (
        .clk(clk),
        .rst_n(rst_n),
        .a(a),
        .b(b),
        .prod(prod)
    );

    always #5 clk = ~clk;

    initial begin
        repeat (2) @(posedge clk);
        rst_n <= 1;

        for (i = 0; i < 32; i = i + 1) begin
            a <= i + 1;
            b <= 300 - 9 * i;
            @(posedge clk);
        end

        @(posedge clk); // let last product appear
        $finish;
    end
endmodule

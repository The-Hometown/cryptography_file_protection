////////////////////////////
//        x_or             //
/////////////////////////////

module x_or (x,k,y);
    input [127:0] x;
    input [31:0] k;
    output [31:0] y;

    assign y=x[31:0]^x[63:32]^x[95:64]^k[31:0];

endmodule


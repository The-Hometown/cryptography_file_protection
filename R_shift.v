/////////////////////////////
//        R_shift      //
/////////////////////////////
module R_shift(x,y);
input [127:0]x;
output [127:0]y;

assign y={x[31:0],x[63:32],x[95:64],x[127:96]};


endmodule


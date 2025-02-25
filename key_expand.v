/////////////////////////////
//        key_expand      //
/////////////////////////////
module key_expand(k,Rrg,rk,s_k,L_k);
input [127:0]k;
input [31:0]Rrg;
output [127:0]rk;
wire [127:0]k_in;
output [31:0] s_k,L_k;
  key_xor key_xor_m(k,Rrg,k_in);
  key_sub key_sub_m(k_in,Rrg,s_k);
  key_L_shift key_L_shift_m(s_k,L_k);
  assign rk={k_in[95:0],L_k[31:0]^k_in[127:96]};

endmodule
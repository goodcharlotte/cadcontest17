module top ( y1 , y2 , a , b , c , d , e , f , g );
input a , b , c , d , e , f , g ;
output y1 , y2 ;
wire g1 , g2 , g3 ;
wire t_0 ;

and ( g1 , a , b , f );
xor ( g2 , a , c , d );
nor ( g3 , b , c , g );
and ( y1 , g1 , g2 , e );
or ( y2 , t_0 , g3 );

endmodule

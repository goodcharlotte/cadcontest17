module G ( 
    g7 , g29 , g30 , g32 , g33 , g34 , g35 , g36 , g37 , g38 , g39 , g41 , g43 , g44 ,
    g45 , g46 , g47 , g48 , g49 , g51 , g53 , g54 , g55 , g56 , g57 , g58 , g59 , g60 ,
    g62 , g64 , g65 , g66 , g67 , g68 , g69 , g70 , g71 , g74 , g78 , g79 , g81 , g82 ,
    g84 , g88 , g89 , g91 , g92 , g94 , g98 , g99 , g101 , g102 , g104 , g108 , g109 ,
    g111 , g112 , g116 , g119 , g124 , g125 , g126 , g127 , g128 , g129 , g130 , g131 ,
    g132 , g133 , g134 , g135 , g137 , g138 , g139 , g140 , g141 , g144 , g145 ,
    g217  );
  input  g7 , g29 , g30 , g32 , g33 , g34 , g35 , g36 , g37 , g38 , g39 , g41 , g43 ,
    g44 , g45 , g46 , g47 , g48 , g49 , g51 , g53 , g54 , g55 , g56 , g57 , g58 , g59 ,
    g60 , g62 , g64 , g65 , g66 , g67 , g68 , g69 , g70 , g71 , g74 , g78 , g79 , g81 ,
    g82 , g84 , g88 , g89 , g91 , g92 , g94 , g98 , g99 , g101 , g102 , g104 , g108 ,
    g109 , g111 , g112 , g116 , g119 , g124 , g125 , g126 , g127 , g128 , g129 , g130 ,
    g131 , g132 , g133 , g134 , g135 , g137 , g138 , g139 , g140 , g141 , g144 , g145 ;
  output g217 ;
  wire n80 , n81 , n82 , n83 , n84 , n85 , n86 , n87 , n88 , n89 , n90 , n91 , n92 , n93 ,
    n94 , n95 , n96 , n97 , n98 , n99 , n100 , n101 , n102 , n103 , n104 , n105 , n106 ,
    n107 , n108 , n109 , n110 , n111 , n112 , n113 , n114 , n115 , n116 , n117 , n118 ,
    n119 , n120 , n121 , n122 , n123 , n124 , n125 , n126 , n127 , n128 , n129 , n130 ,
    n131 , n132 , n133 , n134 , n135 , n136 , n137 , n138 , n139 , n140 , n141 , n142 ,
    n143 , n144 , n145 , n146 , n147 , n148 , n149 , n150 , n151 , n152 , n153 , n154 ,
    n155 , n156 , n157 , n158 , n159 , n160 , n161 , n162 , n163 , n164 , n165 , n166 ,
    n167 , n168 , n169 , n170 , n171 , n172 , n173 , n174 , n175 , n176 , n177 , n178 ,
    n179 , n180 , n181 , n182 , n183 , n184 , n185 , n186 , n187 , n188 , n189 , n190 ,
    n191 , n192 , n193 , n194 , n195 , n196 , n197 , n198 , n199 , n200 , n201 , n202 ,
    n203 , n204 , n205 , n206 , n207 , n208 , n209 , n210 , n211 , n212 , n213 , n214 ,
    n215 , n216 , n217 , n218 , n219 , n220 , n221 , n222 , n223 , n224 , n225 , n226 ,
    n227 , n228 , n229 , n230 , n231 , n232 , n233 , n234 , n235 , n236 , n237 , n238 ,
    n239 , n240 , n241 , n242 , n243 , n244 , n245 , n246 , n247 , n248 , n249 , n250 ,
    n251 , n252 , n253 , n254 , n255 , n256 , n257 , n258 , n259 , n260 , n261 , n262 ,
    n263 , n264 , n265 , n266 , n267 , n268 , n269 , n270 , n271 ;
  assign n80 = g78 & ~g145;
  assign n81 = g88 & g145;
  assign n82 = g144 & ~n80;
  assign n83 = ~n81 & n82;
  assign n84 = g108 & ~g145;
  assign n85 = g98 & g145;
  assign n86 = ~g144 & ~n84;
  assign n87 = ~n85 & n86;
  assign n88 = ~n83 & ~n87;
  assign n89 = g29 & ~n88;
  assign n90 = ~g79 & ~g145;
  assign n91 = ~g89 & g145;
  assign n92 = g144 & ~n90;
  assign n93 = ~n91 & n92;
  assign n94 = ~g109 & ~g145;
  assign n95 = ~g99 & g145;
  assign n96 = ~g144 & ~n94;
  assign n97 = ~n95 & n96;
  assign n98 = ~n93 & ~n97;
  assign n99 = ~g126 & ~n98;
  assign n100 = n89 & ~n99;
  assign n101 = g102 & ~g144;
  assign n102 = g92 & g144;
  assign n103 = g145 & ~n101;
  assign n104 = ~n102 & n103;
  assign n105 = g112 & ~g144;
  assign n106 = g82 & g144;
  assign n107 = ~g145 & ~n105;
  assign n108 = ~n106 & n107;
  assign n109 = ~n104 & ~n108;
  assign n110 = g135 & n109;
  assign n111 = ~g116 & ~g119;
  assign n112 = g64 & n111;
  assign n113 = ~g116 & g119;
  assign n114 = g43 & n113;
  assign n115 = g116 & g119;
  assign n116 = g53 & n115;
  assign n117 = g116 & ~g119;
  assign n118 = g32 & n117;
  assign n119 = ~n112 & ~n114;
  assign n120 = ~n116 & ~n118;
  assign n121 = n119 & n120;
  assign n122 = g133 & ~n121;
  assign n123 = g94 & ~g144;
  assign n124 = g84 & g144;
  assign n125 = g145 & ~n123;
  assign n126 = ~n124 & n125;
  assign n127 = g104 & ~g144;
  assign n128 = g74 & g144;
  assign n129 = ~g145 & ~n127;
  assign n130 = ~n128 & n129;
  assign n131 = ~n126 & ~n130;
  assign n132 = g134 & n131;
  assign n133 = g101 & ~g144;
  assign n134 = g91 & g144;
  assign n135 = g145 & ~n133;
  assign n136 = ~n134 & n135;
  assign n137 = g111 & ~g144;
  assign n138 = g81 & g144;
  assign n139 = ~g145 & ~n137;
  assign n140 = ~n138 & n139;
  assign n141 = ~n136 & ~n140;
  assign n142 = g137 & n141;
  assign n143 = ~n110 & ~n122;
  assign n144 = ~n132 & ~n142;
  assign n145 = n143 & n144;
  assign n146 = n100 & ~n145;
  assign n147 = n89 & n99;
  assign n148 = g7 & ~n147;
  assign n149 = g54 & n115;
  assign n150 = g65 & n111;
  assign n151 = g44 & n113;
  assign n152 = g33 & n117;
  assign n153 = ~n149 & ~n150;
  assign n154 = ~n151 & ~n152;
  assign n155 = n153 & n154;
  assign n156 = g132 & ~n155;
  assign n157 = g34 & n117;
  assign n158 = g66 & ~g116;
  assign n159 = ~g55 & g116;
  assign n160 = g119 & ~n159;
  assign n161 = ~n157 & ~n158;
  assign n162 = ~n160 & n161;
  assign n163 = g131 & ~n162;
  assign n164 = ~n156 & ~n163;
  assign n165 = n148 & ~n164;
  assign n166 = ~g56 & n115;
  assign n167 = ~g35 & n117;
  assign n168 = ~g67 & n111;
  assign n169 = ~g45 & n113;
  assign n170 = ~n166 & ~n167;
  assign n171 = ~n168 & ~n169;
  assign n172 = n170 & n171;
  assign n173 = g130 & ~n147;
  assign n174 = g141 & n147;
  assign n175 = ~n173 & ~n174;
  assign n176 = n172 & ~n175;
  assign n177 = ~g46 & n113;
  assign n178 = ~g36 & n117;
  assign n179 = ~g68 & n111;
  assign n180 = ~g57 & n115;
  assign n181 = ~n177 & ~n178;
  assign n182 = ~n179 & ~n180;
  assign n183 = n181 & n182;
  assign n184 = g129 & ~n147;
  assign n185 = g140 & n147;
  assign n186 = ~n184 & ~n185;
  assign n187 = n183 & ~n186;
  assign n188 = ~n176 & ~n187;
  assign n189 = g7 & ~n188;
  assign n190 = ~g37 & n117;
  assign n191 = ~g47 & n113;
  assign n192 = ~g69 & n111;
  assign n193 = ~g58 & n115;
  assign n194 = ~n190 & ~n191;
  assign n195 = ~n192 & ~n193;
  assign n196 = n194 & n195;
  assign n197 = g128 & ~n147;
  assign n198 = g139 & n147;
  assign n199 = ~n197 & ~n198;
  assign n200 = ~n196 & n199;
  assign n201 = n196 & ~n199;
  assign n202 = g48 & n113;
  assign n203 = g59 & n115;
  assign n204 = g38 & n117;
  assign n205 = g70 & n111;
  assign n206 = ~n202 & ~n203;
  assign n207 = ~n204 & ~n205;
  assign n208 = n206 & n207;
  assign n209 = g127 & ~n147;
  assign n210 = g138 & n147;
  assign n211 = ~n209 & ~n210;
  assign n212 = ~n208 & ~n211;
  assign n213 = g137 & n147;
  assign n214 = ~g71 & n111;
  assign n215 = ~g39 & n117;
  assign n216 = ~g60 & n115;
  assign n217 = ~g49 & n113;
  assign n218 = ~n214 & ~n215;
  assign n219 = ~n216 & ~n217;
  assign n220 = n218 & n219;
  assign n221 = g125 & ~n147;
  assign n222 = ~n213 & ~n220;
  assign n223 = ~n221 & n222;
  assign n224 = n208 & n211;
  assign n225 = ~g62 & n111;
  assign n226 = ~g30 & n117;
  assign n227 = ~g41 & n113;
  assign n228 = ~g51 & n115;
  assign n229 = ~n225 & ~n226;
  assign n230 = ~n227 & ~n228;
  assign n231 = n229 & n230;
  assign n232 = g125 & n220;
  assign n233 = ~g124 & ~n232;
  assign n234 = ~n147 & n233;
  assign n235 = g137 & n220;
  assign n236 = ~g135 & ~n235;
  assign n237 = n147 & n236;
  assign n238 = ~n234 & ~n237;
  assign n239 = ~n231 & ~n238;
  assign n240 = ~n223 & ~n224;
  assign n241 = ~n239 & n240;
  assign n242 = ~n201 & ~n212;
  assign n243 = ~n241 & n242;
  assign n244 = g7 & ~n183;
  assign n245 = n186 & n244;
  assign n246 = ~n200 & ~n245;
  assign n247 = ~n243 & n246;
  assign n248 = ~n189 & ~n247;
  assign n249 = g7 & ~n172;
  assign n250 = n175 & n249;
  assign n251 = ~n248 & ~n250;
  assign n252 = ~n165 & ~n251;
  assign n253 = ~g132 & n155;
  assign n254 = ~g131 & n162;
  assign n255 = ~n253 & ~n254;
  assign n256 = ~n156 & ~n255;
  assign n257 = n148 & n256;
  assign n258 = ~n252 & ~n257;
  assign n259 = ~n146 & ~n258;
  assign n260 = ~g137 & ~n141;
  assign n261 = ~g135 & ~n109;
  assign n262 = n100 & n110;
  assign n263 = ~g134 & ~n131;
  assign n264 = ~g133 & n121;
  assign n265 = ~n263 & ~n264;
  assign n266 = ~n132 & ~n265;
  assign n267 = ~n262 & n266;
  assign n268 = ~n260 & ~n261;
  assign n269 = ~n267 & n268;
  assign n270 = n100 & ~n142;
  assign n271 = ~n269 & n270;
  assign g217 = n259 | n271;
endmodule



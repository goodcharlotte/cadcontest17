module patch ( 
    g7 , g29 , g30 , g32 , g33 , g34 , g35 , g36 , g37 , g38 , g39 , g41 , g43 , g44 ,
    g45 , g46 , g47 , g48 , g49 , g51 , g53 , g54 , g55 , g56 , g57 , g58 , g59 , g60 ,
    g62 , g64 , g65 , g66 , g67 , g68 , g69 , g70 , g71 , g74 , g78 , g79 , g81 , g82 ,
    g84 , g88 , g89 , g91 , g92 , g94 , g98 , g99 , g101 , g102 , g104 , g108 , g109 ,
    g111 , g112 , g116 , g119 , g124 , g125 , g126 , g127 , g128 , g129 , g130 , g131 ,
    g132 , g133 , g134 , g135 , g137 , g138 , g139 , g140 , g141 , g144 , g145 ,
    t_0  );
  input  g7 , g29 , g30 , g32 , g33 , g34 , g35 , g36 , g37 , g38 , g39 , g41 , g43 ,
    g44 , g45 , g46 , g47 , g48 , g49 , g51 , g53 , g54 , g55 , g56 , g57 , g58 , g59 ,
    g60 , g62 , g64 , g65 , g66 , g67 , g68 , g69 , g70 , g71 , g74 , g78 , g79 , g81 ,
    g82 , g84 , g88 , g89 , g91 , g92 , g94 , g98 , g99 , g101 , g102 , g104 , g108 ,
    g109 , g111 , g112 , g116 , g119 , g124 , g125 , g126 , g127 , g128 , g129 , g130 ,
    g131 , g132 , g133 , g134 , g135 , g137 , g138 , g139 , g140 , g141 , g144 , g145 ;
  output t_0 ;
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
    n215 , n216 , n217 ;
  not  ( n80 , g7 );
  not  ( n81 , g116 );
  not  ( n82 , g119 );
  nor  ( n83 , n82 , n81 , g56 );
  nor  ( n84 , n82 , g116 , g45 );
  nor  ( n85 , g119 , n81 , g35 );
  nor  ( n86 , g119 , g116 , g67 );
  nor  ( n87 , n86 , n85 , n84 , n83 , n80 );
  not  ( n88 , g29 );
  and  ( n89 , g145 , g144 , g88 );
  not  ( n90 , g145 );
  and  ( n91 , n90 , g144 , g78 );
  not  ( n92 , g144 );
  and  ( n93 , n90 , n92 , g108 );
  and  ( n94 , g145 , n92 , g98 );
  nor  ( n95 , n94 , n93 , n91 , n89 , n88 );
  not  ( n96 , n95 );
  and  ( n97 , g145 , n92 , g99 );
  and  ( n98 , n90 , g144 , g79 );
  and  ( n99 , g145 , g144 , g89 );
  and  ( n100 , n90 , n92 , g109 );
  nor  ( n101 , n100 , n99 , n98 , n97 );
  nor  ( n102 , n101 , n96 , g126 );
  and  ( n103 , n102 , g141 );
  not  ( n104 , n102 );
  and  ( n105 , n104 , g130 );
  nor  ( n106 , n105 , n103 , n80 );
  not  ( n107 , n106 );
  nand  ( n108 , n107 , n87 );
  or  ( n109 , n107 , n87 );
  and  ( n110 , g119 , n81 , g48 );
  and  ( n111 , g119 , g116 , g59 );
  and  ( n112 , n82 , g116 , g38 );
  and  ( n113 , n82 , n81 , g70 );
  nor  ( n114 , n113 , n112 , n111 , n110 );
  nor  ( n115 , n104 , g138 );
  nor  ( n116 , n102 , g127 );
  nor  ( n117 , n116 , n115 );
  not  ( n118 , n117 );
  nand  ( n119 , n118 , n114 );
  nor  ( n120 , g119 , g116 , g71 );
  nor  ( n121 , g119 , n81 , g39 );
  nor  ( n122 , n82 , n81 , g60 );
  nor  ( n123 , n82 , g116 , g49 );
  nor  ( n124 , n123 , n122 , n121 , n120 );
  and  ( n125 , n124 , g125 );
  nor  ( n126 , n125 , g124 );
  or  ( n127 , n126 , n102 );
  not  ( n128 , g135 );
  nand  ( n129 , n124 , g137 );
  and  ( n130 , n129 , n128 );
  or  ( n131 , n130 , n104 );
  nor  ( n132 , g119 , g116 , g62 );
  nor  ( n133 , g119 , n81 , g30 );
  nor  ( n134 , n82 , g116 , g41 );
  nor  ( n135 , n82 , n81 , g51 );
  or  ( n136 , n135 , n134 , n133 , n132 );
  and  ( n137 , n136 , n131 , n127 , n119 , n109 );
  and  ( n138 , g119 , g116 , g54 );
  and  ( n139 , n82 , n81 , g65 );
  and  ( n140 , g119 , n81 , g44 );
  and  ( n141 , n82 , g116 , g33 );
  nor  ( n142 , n141 , n140 , n139 , n138 );
  not  ( n143 , n142 );
  nand  ( n144 , n143 , g132 );
  nor  ( n145 , n81 , g55 );
  nor  ( n146 , n145 , n82 );
  and  ( n147 , n82 , g116 , g34 );
  and  ( n148 , n81 , g66 );
  nor  ( n149 , n148 , n147 , n146 );
  not  ( n150 , n149 );
  nand  ( n151 , n150 , g131 );
  or  ( n152 , n143 , g132 );
  or  ( n153 , n150 , g131 );
  and  ( n154 , n153 , n152 , n151 , n144 );
  or  ( n155 , n154 , n80 );
  not  ( n156 , g134 );
  nor  ( n157 , n92 , g84 );
  nor  ( n158 , g144 , g94 );
  nor  ( n159 , n158 , n157 , n90 );
  nor  ( n160 , n92 , g74 );
  nor  ( n161 , g144 , g104 );
  nor  ( n162 , n161 , n160 , g145 );
  nor  ( n163 , n162 , n159 );
  nand  ( n164 , n163 , n156 );
  nor  ( n165 , n92 , g92 );
  nor  ( n166 , g144 , g102 );
  nor  ( n167 , n166 , n165 , n90 );
  nor  ( n168 , n92 , g82 );
  nor  ( n169 , g144 , g112 );
  nor  ( n170 , n169 , n168 , g145 );
  nor  ( n171 , n170 , n167 );
  or  ( n172 , n171 , n128 );
  nand  ( n173 , n90 , g144 , g81 );
  nand  ( n174 , g145 , g144 , g91 );
  nand  ( n175 , n90 , n92 , g111 );
  nand  ( n176 , g145 , n92 , g101 );
  nand  ( n177 , n176 , n175 , n174 , n173 );
  xor  ( n178 , n177 , g137 );
  and  ( n179 , n178 , n172 , n164 );
  nand  ( n180 , n82 , n81 , g64 );
  nand  ( n181 , g119 , n81 , g43 );
  nand  ( n182 , g119 , g116 , g53 );
  nand  ( n183 , n82 , g116 , g32 );
  nand  ( n184 , n183 , n182 , n181 , n180 );
  xor  ( n185 , n184 , g133 );
  nand  ( n186 , n171 , n128 );
  or  ( n187 , n163 , n156 );
  and  ( n188 , n187 , n186 , n185 , n179 );
  or  ( n189 , n188 , n96 );
  and  ( n190 , n189 , n155 );
  or  ( n191 , n190 , n102 );
  nor  ( n192 , n82 , g116 , g46 );
  nor  ( n193 , g119 , n81 , g36 );
  nor  ( n194 , g119 , g116 , g68 );
  nor  ( n195 , n82 , n81 , g57 );
  nor  ( n196 , n195 , n194 , n193 , n192 );
  and  ( n197 , n104 , g129 );
  and  ( n198 , n102 , g140 );
  nor  ( n199 , n198 , n197 );
  and  ( n200 , n199 , n196 );
  nor  ( n201 , n199 , n196 );
  or  ( n202 , n201 , n200 , n80 );
  nor  ( n203 , g119 , n81 , g37 );
  nor  ( n204 , n82 , g116 , g47 );
  nor  ( n205 , g119 , g116 , g69 );
  nor  ( n206 , n82 , n81 , g58 );
  nor  ( n207 , n206 , n205 , n204 , n203 );
  nor  ( n208 , n104 , g139 );
  nor  ( n209 , n102 , g128 );
  nor  ( n210 , n209 , n208 );
  nand  ( n211 , n210 , n207 );
  or  ( n212 , n118 , n114 );
  and  ( n213 , n104 , g125 );
  and  ( n214 , n102 , g137 );
  or  ( n215 , n214 , n213 , n124 );
  or  ( n216 , n210 , n207 );
  and  ( n217 , n216 , n215 , n212 , n211 , n202 );
  and  ( t_0 , n217 , n191 , n137 , n108 );
endmodule



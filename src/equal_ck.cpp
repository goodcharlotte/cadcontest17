#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "datatype.h"

using namespace std;

#define sat_new_var(curr_sat, id) do { \
	while(id >= curr_sat.nVars()) { \
		curr_sat.newVar(); \
	} \
} while(0)


void gate2CNF(Solver& sat, int gate_out, Node_t gate);
void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2);


bool Circuit_t::euqal_ck(int F_nid, int P_nid)
{
	Solver ckt_sat;	
	CNF_fanin(ckt_sat, F_nid);
	CNF_fanin(ckt_sat, P_nid);
	miter2CNF(ckt_sat, F_nid, P_nid);
	
	ckt_sat.solve();
	
	//cout << "ckt_sat.nVars: " << ckt_sat.nVars() << endl;
	//cout << "ckt_sat.nClauses: " << ckt_sat.nClauses() << endl;
	//cout << "ckt_sat.ok: " << ckt_sat.okay() << endl;
	return  ckt_sat.okay();
}

void Circuit_t::CNF_fanin(Solver& sat, int node_id)
{
	//cout << "====CNF_fanin, node_id " << node_id << endl;
    vector<bool> visit_flag(allnodevec.size(), false);	
	queue<int> nodeque;
	nodeque.push(node_id);

	int node;
	//this part is form circuit.cpp - findRelatedPI()
	while (nodeque.size() != 0) {
		node = nodeque.front();
		for (int i = 0; i < allnodevec[node].in.size(); i++) {
			if (visit_flag[allnodevec[node].in[i]] == false) {
				nodeque.push(allnodevec[node].in[i]);
				//append clause to sat
				gate2CNF(sat, allnodevec[node].in[i], allnodevec[node]);
				//cout << "sat.nClauses: " << sat.nClauses() << endl;
			}
				
		}
		visit_flag[node] = true;
		nodeque.pop();
	}
}

void gate2CNF(Solver& sat, int gate_out, Node_t gate)
{
	vec<Lit> vec_lit;
	
	sat_new_var(sat, gate_out);
	for (int i; i < (gate.in).size(); i++) {
		sat_new_var(sat, gate.in[i]);
	}	

	switch (gate.type) {
		
		case BUF:
			/*
				C = A
				(A V -C) ^ (-A V C)
			*/			
			sat.addClause(mkLit(gate_out), ~mkLit(gate.in[0]));
			sat.addClause(~mkLit(gate_out), mkLit(gate.in[0]));
		break;	
		
		case NOT:
			/*
				C = not A
				(A V C) ^ (-A V ~C)
			*/	
			sat.addClause(mkLit(gate_out), mkLit(gate.in[0]));
			sat.addClause(~mkLit(gate_out), ~mkLit(gate.in[0]));			
		break;			

		case AND:
			/*
				C = A and B
				(~A V ~B V C) ^ (A V ~C) ^ (B V ~C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(gate.in[i]));		
			}	
			vec_lit.push(mkLit(gate_out));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(mkLit(gate.in[i]));
				vec_lit.push(~mkLit(gate_out));	
				sat.addClause(vec_lit);
			}
		break;
		
		case NAND:
			/*
				C = A nand B
				(~A V ~B V ~C) ^ (A V C) ^ (B V C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(gate.in[i]));		
			}	
			vec_lit.push(~mkLit(gate_out));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(mkLit(gate.in[i]));
				vec_lit.push(mkLit(gate_out));	
				sat.addClause(vec_lit);
			}			
		break;			
		case OR: 
			/*
				C = A or B
				(A V B V ~C) ^ (~A V C) ^ (~B V C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(gate.in[i]));		
			}	
			vec_lit.push(~mkLit(gate_out));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(~mkLit(gate.in[i]));
				vec_lit.push(mkLit(gate_out));	
				sat.addClause(vec_lit);
			}				
		break;	
		case NOR: 
			/*
				C = A nor B
				(A V B V C) ^ (~A V ~C) ^ (~B V ~C) 
			*/			
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(gate.in[i]));		
			}	
			vec_lit.push(mkLit(gate_out));
			
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(~mkLit(gate.in[i]));
				vec_lit.push(~mkLit(gate_out));	
				sat.addClause(vec_lit);
			}	
			
		break;	
		case XOR:
			/*
				C = A xor B
				(~A V ~B V ~C) ^ (A V B V ~C) \
				(A V ~B V C) ^ (~A V B V C)
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(gate.in[i]));		
			}	
			vec_lit.push(~mkLit(gate_out));
			sat.addClause(vec_lit);
			//-----
			vec_lit.clear();
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(gate.in[i]));		
			}	
			vec_lit.push(~mkLit(gate_out));
			sat.addClause(vec_lit);			
			//-----
	
			for (int neg = 0; neg < (gate.in).size(); neg++) {
				vec_lit.clear();
				for (int i = 0; i < (gate.in).size(); i++) {
					
					if(neg == i) {
						vec_lit.push(~mkLit(gate.in[i]));
					} else {
						vec_lit.push(mkLit(gate.in[i]));
					}		
				}
				vec_lit.push(mkLit(gate_out));	
				sat.addClause(vec_lit);			
			}
			
		break;		
		case NXOR: 
			/*
				C = A xnor B
				(~A V ~B V C) ^ (A V B V C) \
				(A V ~B V ~C) ^ (~A V B V ~C)
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(gate.in[i]));		
			}	
			vec_lit.push(mkLit(gate_out));
			sat.addClause(vec_lit);
			//-----
			vec_lit.clear();
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(gate.in[i]));		
			}	
			vec_lit.push(mkLit(gate_out));
			sat.addClause(vec_lit);			
			//-----

			for (int neg = 0; neg < (gate.in).size(); neg++) {
				vec_lit.clear();
				for (int i = 0; i < (gate.in).size(); i++) {
					
					if(neg == i) {
						vec_lit.push(~mkLit(gate.in[i]));
					} else {
						vec_lit.push(mkLit(gate.in[i]));
					}		
				}
				vec_lit.push(~mkLit(gate_out));	
				sat.addClause(vec_lit);			
			}
				
		break;		
		
		case PORT: 
		
		
		break;
		
	}
}

void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2)
{
	/*
		XOR
		C = A xor B
		(~A V ~B V ~C) ^ (A V B V ~C) \
		(A V ~B V C) ^ (~A V B V C)
	*/	
	sat_new_var(sat, gate_in_1);
	sat_new_var(sat, gate_in_2);	
	int gate_out = sat.newVar();
	sat.addClause(~mkLit(gate_in_1), ~mkLit(gate_in_2), ~mkLit(gate_out));
	sat.addClause( mkLit(gate_in_1),  mkLit(gate_in_2), ~mkLit(gate_out));
	sat.addClause( mkLit(gate_in_1), ~mkLit(gate_in_2),  mkLit(gate_out));
	sat.addClause(~mkLit(gate_in_1),  mkLit(gate_in_2),  mkLit(gate_out));	
}
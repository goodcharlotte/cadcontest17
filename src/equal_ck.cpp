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
int inver2CNF(Solver& sat, int node);



int Circuit_t::euqal_ck(int F_nid, int P_nid)
{
	Solver ckt_sat;	
	CNF_fanin(ckt_sat, F_nid);
	CNF_fanin(ckt_sat, P_nid);
	miter2CNF(ckt_sat, F_nid, P_nid);
	ckt_sat.solve();
	
	if (ckt_sat.okay() == false) {
		return EQ_UNSAT;
	}
	
	// add inverter to check again
	//TODO: try another way, bad to find fanin again
	Solver ckt_sat_invF;
	CNF_fanin(ckt_sat_invF, F_nid);
	CNF_fanin(ckt_sat_invF, P_nid);
	int F_nid_inv = inver2CNF(ckt_sat_invF, F_nid);
	miter2CNF(ckt_sat_invF, F_nid_inv, P_nid);
	ckt_sat_invF.solve();
	if (ckt_sat_invF.okay() == false) {
		return EQ_INV_UNSAT;
	}
	//TODO: check whether this is must.
	Solver ckt_sat_invP;
	CNF_fanin(ckt_sat_invP, F_nid);
	CNF_fanin(ckt_sat_invP, P_nid);
	int P_nid_inv = inver2CNF(ckt_sat_invP, P_nid);
	miter2CNF(ckt_sat_invP, F_nid, P_nid_inv);
	ckt_sat_invP.solve();
	if (ckt_sat_invP.okay() == false) {
		return EQ_INV_UNSAT;
	}	
	
	return EQ_SAT;
}

void Circuit_t::CNF_fanin(Solver& sat, int node_id)
{
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
				gate2CNF(sat, node, allnodevec[node]);
			}
				
		}
		visit_flag[node] = true;
		nodeque.pop();
	}
}

void gate2CNF(Solver& sat, int gate_out, Node_t gate)
{
	if ((gate.in).size() == 0) {
		return;
	}

	vec<Lit> vec_lit;
	sat_new_var(sat, gate_out);
	for (int i = 0; i < (gate.in).size(); i++) {
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
			//...
		break;
		
	}
}

void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2)
{
	/*
		XOR
		C = A xor B
		(~A V ~B V ~C) ^ (A V B V ~C) ^ \
		(A V ~B V C) ^ (~A V B V C)
	*/	
	//cout << "gate_in_1 " << gate_in_1 << endl;
	//cout << "gate_in_2 " << gate_in_2 << endl;	
	sat_new_var(sat, gate_in_1);
	sat_new_var(sat, gate_in_2);	
	int gate_out = sat.newVar();
	sat.addClause(~mkLit(gate_in_1), ~mkLit(gate_in_2), ~mkLit(gate_out));
	sat.addClause( mkLit(gate_in_1),  mkLit(gate_in_2), ~mkLit(gate_out));
	sat.addClause( mkLit(gate_in_1), ~mkLit(gate_in_2),  mkLit(gate_out));
	sat.addClause(~mkLit(gate_in_1),  mkLit(gate_in_2),  mkLit(gate_out));	
	//let output = 1
	sat.addClause(mkLit(gate_out));		
}

int inver2CNF(Solver& sat, int node)
{
	/*
		C = not A
		(A V C) ^ (-A V ~C)
	*/	
	sat_new_var(sat, node);
	int new_out = sat.newVar();
	sat.addClause(mkLit(node), mkLit(new_out));
	sat.addClause(~mkLit(node), ~mkLit(new_out));
	return new_out;
}

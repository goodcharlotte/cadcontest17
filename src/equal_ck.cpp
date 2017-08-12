#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "datatype.h"

using namespace std;

#define DEBUG_EQ 0

#define sat_new_var(curr_sat, id) do { \
	while(id >= curr_sat.nVars()) { \
		curr_sat.newVar(); \
	} \
} while(0)

	


void gate2CNF(Solver& sat, int gate_out, Node_t gate);
void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2, int assume_lit, int gate_out);
int inver2CNF(Solver& sat, int node, int assume_lit);

template <class T>
void print_queue(queue<T> q)
{
  while (!q.empty()) {
	cout << q.front() << " ";
    q.pop();
  }
  cout << endl;
}

template <class T>
void print_vector(vector<T> v)
{
	for(int i = 0; i < v.size(); i++) {
		cout << v[i] << " ";
	}
	cout << endl;
}


int Circuit_t::euqal_ck(int F_nid, int P_nid)
{
	//check if PI number is the same and equal
	/*
	print_vector(allnodevec[F_nid].allpi);
	print_vector(allnodevec[P_nid].allpi);
	*/
	if (allnodevec[F_nid].allpi != allnodevec[P_nid].allpi) {
		return EQ_SAT;
	}

	Solver ckt_sat;	
	sat_new_var(ckt_sat, F_nid);
	sat_new_var(ckt_sat, P_nid);
	CNF_fanin(ckt_sat, F_nid);
	CNF_fanin(ckt_sat, P_nid);	
	//This is a trick to simulate removing clause.
	int assume_lit_no_inv = ckt_sat.newVar();
	int assume_lit_has_inv = ckt_sat.newVar();
	int gate_out_no_inv = ckt_sat.newVar();
	int gate_out_has_inv = ckt_sat.newVar();
	
	//miter with inverter
	int F_nid_inv = inver2CNF(ckt_sat, F_nid, assume_lit_has_inv);
	miter2CNF(ckt_sat, F_nid_inv, P_nid, assume_lit_has_inv, gate_out_has_inv);
	//miter without inverter
	miter2CNF(ckt_sat, F_nid, P_nid, assume_lit_no_inv, gate_out_no_inv);
	
	//let output = 1
	ckt_sat.addClause(mkLit(gate_out_no_inv));
	ckt_sat.addClause(mkLit(gate_out_has_inv));
	
	//TODO: check if any difference if using the below
	//ckt_sat.addClause(mkLit(assume_lit_no_inv), mkLit(gate_out_no_inv));
	//ckt_sat.addClause(mkLit(assume_lit_has_inv), mkLit(gate_out_has_inv));
	
	//cout << "\n no_inv solve " << ckt_sat.solve(~mkLit(assume_lit_no_inv), mkLit(assume_lit_has_inv)) << endl;
	//cout << " ckt_sat.okay() " << ckt_sat.okay() << endl;
	if (ckt_sat.solve(~mkLit(assume_lit_no_inv), mkLit(assume_lit_has_inv)) == false) {
		return EQ_UNSAT;
	}
	//cout << " has_inv_inv solve " <<ckt_sat.solve(mkLit(assume_lit_no_inv), ~mkLit(assume_lit_has_inv)) << endl;
	//cout << " ckt_sat.okay() " << ckt_sat.okay() << endl;
	if (ckt_sat.solve(mkLit(assume_lit_no_inv), ~mkLit(assume_lit_has_inv)) == false) {
		return EQ_INV_UNSAT;
	}
	return EQ_SAT;	
	

	/*
	Solver ckt_sat;	
	sat_new_var(ckt_sat, F_nid);
	sat_new_var(ckt_sat, P_nid);
	CNF_fanin(ckt_sat, F_nid);
	CNF_fanin(ckt_sat, P_nid);
	miter2CNF(ckt_sat, F_nid, P_nid);
	ckt_sat.solve();
	
	if (ckt_sat.okay() == false) {
		return EQ_UNSAT;
	
	// add inverter to check again
	//TODO: try another way, bad to find fanin again
	Solver ckt_sat_invF;
	sat_new_var(ckt_sat_invF, F_nid);
	sat_new_var(ckt_sat_invF, P_nid);
	CNF_fanin(ckt_sat_invF, F_nid);
	CNF_fanin(ckt_sat_invF, P_nid);
	int F_nid_inv = inver2CNF(ckt_sat_invF, F_nid);
	miter2CNF(ckt_sat_invF, F_nid_inv, P_nid);
	ckt_sat_invF.solve();
	if (ckt_sat_invF.okay() == false) {
		return EQ_INV_UNSAT;
	}
	
	return EQ_SAT;
	*/
}



void Circuit_t::CNF_fanin(Solver& sat, int node_id)
{
    vector<bool> visit_flag(allnodevec.size(), false);	
	queue<int> nodeque;
	nodeque.push(node_id);
	#if DEBUG_EQ
	cout << "now queue is ";
	print_queue(nodeque);
	#endif
	int node;
	//this part is form circuit.cpp - findRelatedPI()
	while (nodeque.size() != 0) {
		node = nodeque.front();
		//append clause to sat
		gate2CNF(sat, node, allnodevec[node]);	
		for (int i = 0; i < allnodevec[node].in.size(); i++) {
			if (visit_flag[allnodevec[node].in[i]] == false) {
				nodeque.push(allnodevec[node].in[i]);
				#if DEBUG_EQ
				cout << "push  " << allnodevec[node].in[i] << endl;
				cout << "now queue is ";
				print_queue(nodeque);
				#endif
			}
			
		}
		visit_flag[node] = true;
		nodeque.pop();
		#if DEBUG_EQ
		cout << "pop  " << endl;
		cout << "now queue is ";
		print_queue(nodeque);
		#endif
	}
}

void gate2CNF(Solver& sat, int gate_out, Node_t gate)
{
	#if DEBUG_EQ
	cout << "0 gate2CNF , gate_out=" << gate_out << endl; 
	#endif
	if ((gate.in).size() == 0) {
		return;
	}

	vec<Lit> vec_lit;
	sat_new_var(sat, gate_out);
	for (int i = 0; i < (gate.in).size(); i++) {
		#if DEBUG_EQ
		cout << "1 gate2CNF in " << gate.in[i] << endl;
		#endif
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

void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2, int assume_lit, int gate_out)
{
	/*
		XOR
		C = A xor B
		(~A V ~B V ~C) ^ (A V B V ~C) ^ \
		(A V ~B V C) ^ (~A V B V C)
	*/	
	//cout << " miter2CNF, gate_in_1 " << gate_in_1 << endl;
	//cout << " miter2CNF, gate_in_2 " << gate_in_2 << endl;
	sat_new_var(sat, gate_in_1);
	sat_new_var(sat, gate_in_2);	

	vec<Lit> vec_lit;
	vec_lit.push(~mkLit(gate_in_1));
	vec_lit.push(~mkLit(gate_in_2));
	vec_lit.push(~mkLit(gate_out));
	vec_lit.push( mkLit(assume_lit));
	sat.addClause(vec_lit);
	vec_lit.clear();
	
	vec_lit.push( mkLit(gate_in_1));
	vec_lit.push( mkLit(gate_in_2));
	vec_lit.push(~mkLit(gate_out));
	vec_lit.push( mkLit(assume_lit));
	sat.addClause(vec_lit);
	vec_lit.clear();

	vec_lit.push( mkLit(gate_in_1));
	vec_lit.push(~mkLit(gate_in_2));
	vec_lit.push( mkLit(gate_out));
	vec_lit.push( mkLit(assume_lit));
	sat.addClause(vec_lit);
	vec_lit.clear();

	vec_lit.push(~mkLit(gate_in_1));
	vec_lit.push( mkLit(gate_in_2));
	vec_lit.push( mkLit(gate_out));
	vec_lit.push( mkLit(assume_lit));
	sat.addClause(vec_lit);
	vec_lit.clear();		
		
}

int inver2CNF(Solver& sat, int node, int assume_lit)
{
	/*
		C = not A
		(A V C) ^ (-A V ~C)
	*/	
	sat_new_var(sat, node);
	int new_out = sat.newVar();
	sat.addClause(mkLit(node), mkLit(new_out), mkLit(assume_lit));
	sat.addClause(~mkLit(node), ~mkLit(new_out), mkLit(assume_lit));
	return new_out;
}

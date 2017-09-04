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
	

#define EN_ERROR_EXIT 1

#if EN_ERROR_EXIT
#define sat_ck_exist(curr_sat, id, error_info) do { \
	if(id >= curr_sat.nVars()) { \
		cout << "Error :" << error_info << id << " does not exist (should already exist)\n"; \
		exit(1); \
	} \
} while(0)
#else 
#define sat_ck_exist(curr_sat, id, error_info)	0
#endif

unsigned int sat_var_record[4] = {0, 0, 0, 0};

void gate2CNF(Solver& sat, int gate_out, Node_t gate, unsigned int id_offset = 0);
void miter2CNF(Solver& sat, int gate_in_1, int gate_in_2, int assume_lit, int gate_out);
int inver2CNF(Solver& sat, int node, int assume_lit);
void patchckt2CNF(Solver &sat, Circuit_t & ckt, map<string, int> &relative_pi, unsigned int id_offset = 0);
map<string, int> fckt2CNF(Solver &sat, Circuit_t & ckt, vector<int> &allcandidate, unsigned int id_offset = 0);
void in2_xnor2CNF(Solver &sat, int in1, int in2, int out);
void bindpi2CNF(Solver &sat, map<string, int> &relative_pi_f, map<string, int> &relative_pi_patch);
map<int, int> xor_SA1_2CNF(Solver &sat, Circuit_t &ckt, vector<int> &allcandidate, unsigned int id_offset /*= 0*/);
void check_mapvalue(map<string, int> &mymap);
void init_mapvalue(map<string, int> &mymap);
map<int, int> constructDLN(Solver &sat, Circuit_t &F_v_ckt, Circuit_t &patchckt1_only , Circuit_t &patchckt2_only, vector<int> &allcandidate);
bool is_basenode_all_cover(Solver& sat, map<int, int> &id_map, const vector<int> &choosebase);

int Circuit_t::euqal_ck(int F_nid, int P_nid)
{
	//check if PI number is the same and equal
	/*
	print_vector(allnodevec[F_nid].allpi);
	print_vector(allnodevec[P_nid].allpi);
	*/
	/*
	Even though PI not equal, they also can be solved as UNSAT
	so comment out.
	if (allnodevec[F_nid].allpi != allnodevec[P_nid].allpi) {
		print_vector(allnodevec[F_nid].allpi);
		print_vector(allnodevec[P_nid].allpi);
		return EQ_SAT;
	}
	*/
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

void gate2CNF(Solver& sat, int gate_out, Node_t gate, unsigned int id_offset)
{
	#if 1
	#define OFFSET_CNFID(in, off) (in + off)
	#else
	#define OFFSET_CNFID(in, off) (in)
	#endif
	
	
	#if DEBUG_EQ
	cout << "0 gate2CNF , gate_out=" << gate_out << endl; 
	#endif
	if ((gate.in).size() == 0) {
		return;
	}

	vec<Lit> vec_lit;
	sat_new_var(sat, OFFSET_CNFID(gate_out, id_offset));
	for (int i = 0; i < (gate.in).size(); i++) {
		#if DEBUG_EQ
		cout << "1 gate2CNF in " << gate.in[i] << endl;
		#endif
		sat_new_var(sat, OFFSET_CNFID(gate.in[i], id_offset));		
	}	
	switch (gate.type) {
		
		case BUF:
			/*
				C = A
				(A V -C) ^ (-A V C)
			*/			
			sat.addClause(mkLit(OFFSET_CNFID(gate_out, id_offset)), ~mkLit(OFFSET_CNFID(gate.in[0], id_offset)));
			sat.addClause(~mkLit(OFFSET_CNFID(gate_out, id_offset)), mkLit(OFFSET_CNFID(gate.in[0], id_offset)));
		break;	
		
		case NOT:
			/*
				C = not A
				(A V C) ^ (-A V ~C)
			*/	
			sat.addClause(mkLit(OFFSET_CNFID(gate_out, id_offset)), mkLit(OFFSET_CNFID(gate.in[0], id_offset)));
			sat.addClause(~mkLit(OFFSET_CNFID(gate_out, id_offset)), ~mkLit(OFFSET_CNFID(gate.in[0], id_offset)));			
		break;			

		case AND:
			/*
				C = A and B
				(~A V ~B V C) ^ (A V ~C) ^ (B V ~C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
				vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));	
				sat.addClause(vec_lit);
			}
		break;
		
		case NAND:
			/*
				C = A nand B
				(~A V ~B V ~C) ^ (A V C) ^ (B V C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
				vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));	
				sat.addClause(vec_lit);
			}			
		break;			
		case OR: 
			/*
				C = A or B
				(A V B V ~C) ^ (~A V C) ^ (~B V C) 
			*/	
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			
			//-----
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
				vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));	
				sat.addClause(vec_lit);
			}
		break;	
		case NOR: 
			/*
				C = A nor B
				(A V B V C) ^ (~A V ~C) ^ (~B V ~C) 
			*/			
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			//-----
		
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.clear();
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
				vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));		
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
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			//-----
			vec_lit.clear();
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);			
			//-----
	
			for (int neg = 0; neg < (gate.in).size(); neg++) {
				vec_lit.clear();
				for (int i = 0; i < (gate.in).size(); i++) {
					
					if(neg == i) {
						vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
					} else {
						vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
					}		
				}
				vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));	
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
				vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);
			//-----
			vec_lit.clear();
			for (int i = 0; i < (gate.in).size(); i++) {
				vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));		
			}	
			vec_lit.push(mkLit(OFFSET_CNFID(gate_out, id_offset)));
			sat.addClause(vec_lit);			
			//-----

			for (int neg = 0; neg < (gate.in).size(); neg++) {
				vec_lit.clear();
				for (int i = 0; i < (gate.in).size(); i++) {
					
					if(neg == i) {
						vec_lit.push(~mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
					} else {
						vec_lit.push(mkLit(OFFSET_CNFID(gate.in[i], id_offset)));
					}		
				}
				vec_lit.push(~mkLit(OFFSET_CNFID(gate_out, id_offset)));	
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



void patchckt2CNF(Solver &sat, Circuit_t & ckt, map<string, int> &relative_pi, unsigned int id_offset)
{
	unsigned int node_cout;
	unsigned int out_cout;
	unsigned int id_curr_out;
	vector<int> visited(ckt.allnodevec.size(), 0);
	//vector<int> pi_id;
	//cout << "~~~patchckt2CNF PI " << endl;
	//cout << "--total" << ' ' << ckt.allnodevec.size() << endl;
	for (node_cout = 0; node_cout < ckt.allnodevec.size(); node_cout++) {
		//cout << node_cout << ' ' << ckt.allnodevec[node_cout].out.size() << endl;
		for (out_cout = 0; out_cout < ckt.allnodevec[node_cout].out.size(); out_cout++) {
			id_curr_out = ckt.allnodevec[node_cout].out[out_cout];
			if (visited[id_curr_out] == 0) {
				gate2CNF(sat, id_curr_out, ckt.allnodevec[id_curr_out], id_offset);
				visited[id_curr_out] = 1;
			}
		}
		if (relative_pi.find(ckt.allnodevec[node_cout].name) != relative_pi.end()) {
			relative_pi[ckt.allnodevec[node_cout].name] = OFFSET_CNFID(node_cout, id_offset);
		}
		
		if (ckt.allnodevec[node_cout].name == "t_0") {
			//cout << "add t_0" << endl;
			//let output 1
			sat_new_var(sat, OFFSET_CNFID(node_cout, id_offset));
			sat.addClause(mkLit(OFFSET_CNFID(node_cout, id_offset)));
		}
	}
}

map<string, int> fckt2CNF(Solver &sat, Circuit_t & ckt, vector<int> &allcandidate, unsigned int id_offset)
{
	map<string, int> relative_pi; //key: name , value: id used in CNF
	unsigned int node_cout;
	unsigned int out_cout;
	unsigned int id_curr_out;
	vector<int> pi_id;
	vector<int> visited(ckt.allnodevec.size(), 0);
	//cout <<" my allcandidate \n"; 
	//print_vector(allcandidate);
	//cout << "~~~fckt2CNF PI " << endl;
	//cout << "--total" << ' ' << ckt.allnodevec.size() << endl;
	
	for (node_cout = 0; node_cout < ckt.allnodevec.size(); node_cout++) {
		if (find(allcandidate.begin(), allcandidate.end(), node_cout) !=  allcandidate.end()) {
			//cout << node_cout << ' ' << ckt.allnodevec[node_cout].out.size() << endl;
			for (out_cout = 0; out_cout < ckt.allnodevec[node_cout].out.size(); out_cout++) {
				id_curr_out = ckt.allnodevec[node_cout].out[out_cout];
				if (visited[id_curr_out] == 0) {
					gate2CNF(sat, id_curr_out, ckt.allnodevec[id_curr_out], id_offset);
					visited[id_curr_out] = 1;
				}
			}
			if ((ckt.allnodevec[node_cout].type == PORT) && (ckt.allnodevec[node_cout].name != "t_0")) { 
				relative_pi[ckt.allnodevec[node_cout].name] = OFFSET_CNFID(node_cout, id_offset);
				//cout << node_cout << '(' << ckt.allnodevec[node_cout].name << endl;
			}			
			
		}
	}

	return relative_pi;
}


void in2_xnor2CNF(Solver &sat, int in1, int in2, int out)
{
	
	/*
		C = A xnor B
		(~A V ~B V C) ^ (A V B V C) \
		(A V ~B V ~C) ^ (~A V B V ~C)
	*/	
	sat.addClause(~mkLit(in1), ~mkLit(in2),  mkLit(out));
	sat.addClause( mkLit(in1),  mkLit(in2),  mkLit(out));
	sat.addClause( mkLit(in1), ~mkLit(in2), ~mkLit(out));
	sat.addClause(~mkLit(in1),  mkLit(in2), ~mkLit(out));
	
}



void bindpi2CNF(Solver &sat, map<string, int> &relative_pi_f, map<string, int> &relative_pi_patch)
{
	int out;
	map<string, int>::const_iterator iter_fckt = relative_pi_f.begin();
	map<string, int>::const_iterator iter_patch = relative_pi_patch.begin();
	#if EN_ERROR_EXIT
	if (relative_pi_f.size() != relative_pi_patch.size()) {
		cout << "Error : " << "bindpi2CNF size not match " << relative_pi_f.size() << ' ' << relative_pi_patch.size() << endl;
		exit(1);		
	}
	#endif
	while(iter_fckt != relative_pi_f.end()) {
		//cout << iter_fckt->first << ' ' << iter_patch->first << endl;
		//cout << iter_fckt->second << ' ' << iter_patch->second << endl;	
		
		if (iter_fckt->first == iter_patch->first) {

			sat_ck_exist(sat, iter_fckt->second, "bindpi2CNF ");
			sat_ck_exist(sat, iter_patch->second, "bindpi2CNF ");
			
			#if 1
			/* do buffer
				C = A
				(A V -C) ^ (-A V C)
			*/
			//must should already exist!
			//cout << iter_fckt->second << ' ' << iter_patch->second << endl;
			sat.addClause(mkLit(iter_fckt->second), ~mkLit(iter_patch->second));
			sat.addClause(~mkLit(iter_fckt->second), mkLit(iter_patch->second));
			
			#else
			
			//do xnor
			out = sat.newVar();
			in2_xnor2CNF(sat, iter_fckt->second, iter_patch->second, out);
			sat.addClause(mkLit(out));
			//let xnor output =1
			#endif
		
		} 
		#if EN_ERROR_EXIT
		else {
			cout << "Error : " << "bindpi2CNF not match " << iter_fckt->first << ' ' << iter_patch->first << endl;
			exit(1);
		}
		#endif
		iter_fckt ++;
		iter_patch ++;
	}
	

}


map<int, int> xor_SA1_2CNF(Solver &sat, Circuit_t &ckt, vector<int> &allcandidate, unsigned int id_offset /*= 0*/)
{
 
	int id_xor_out;
	int id_stuckatone;
	int id_assume;
	int id_final_and_out;
	/*
	
	      -------                    -------
	g1----| nxor |------id_xor_out--- | or |-----id_stuckatone      == and all =>  id_final_and_out
	g2----|     |      id_assume---- |     |
	      ------                     ------
	
	*/
	unsigned int node_cout;
	map<int, int> id_map_assume; //ori_id -> SA1 assume id
	vec<Lit> vec_lit;
	vector<int> allid_stuckatone;
	//cout <<" my allcandidate \n"; 
	//print_vector(allcandidate);
	for (node_cout = 0; node_cout < ckt.allnodevec.size(); node_cout++) {
		if (find(allcandidate.begin(), allcandidate.end(), node_cout) !=  allcandidate.end()) {

			sat_ck_exist(sat, node_cout, "xor_SA1_2CNF ");
			sat_ck_exist(sat, OFFSET_CNFID(node_cout, id_offset), "xor_SA1_2CNF ");
			id_xor_out = sat.newVar();
			id_stuckatone = sat.newVar();
			id_assume = sat.newVar();
			allid_stuckatone.push_back(id_stuckatone);
			id_map_assume[node_cout] = id_assume;
			
			//cout << "id_map_assume " << node_cout << "(" << ckt.allnodevec[node_cout].name << ")"
			//" nxor " << OFFSET_CNFID(node_cout, id_offset) << " -> "<< id_assume << endl;
			// id_xor_out = g1 nxor g2
			in2_xnor2CNF(sat, node_cout, OFFSET_CNFID(node_cout, id_offset), id_xor_out);	
			
			// id_stuckatone = id_xor_out or id_assume
			/*
				C = A or B
				(A V B V ~C) ^ (~A V C) ^ (~B V C) 
			*/		
			sat.addClause( mkLit(id_xor_out),  mkLit(id_assume), ~mkLit(id_stuckatone));
			sat.addClause(~mkLit(id_xor_out),  mkLit(id_stuckatone));
			sat.addClause(~mkLit(id_assume) ,  mkLit(id_stuckatone));
		}
	}

	
	// and all id_stuckatone
	/*
		C = A and B
		(~A V ~B V C) ^ (A V ~C) ^ (B V ~C) 
	*/
	id_final_and_out = sat.newVar();
	vec_lit.clear();
	//cout << "id_final_and_out " << id_final_and_out << endl;
	for (node_cout = 0; node_cout < allid_stuckatone.size(); node_cout++) {
		vec_lit.push(~mkLit(allid_stuckatone[node_cout]));		
	}	
	vec_lit.push(mkLit(id_final_and_out));
	sat.addClause(vec_lit);

	for (node_cout = 0; node_cout < allid_stuckatone.size(); node_cout++) {
		vec_lit.clear();
		vec_lit.push(mkLit(allid_stuckatone[node_cout]));
		vec_lit.push(~mkLit(id_final_and_out));	
		sat.addClause(vec_lit);
	}
	
	//let output 1
	sat.addClause(mkLit(id_final_and_out));
	return id_map_assume;

}

void check_mapvalue(map<string, int> &mymap)
{
	#if EN_ERROR_EXIT
	for (map<string, int>::const_iterator it = mymap.begin(); it != mymap.end(); ++it) {
		if (it->second < 0) {
			cout << "Error: "<< it->first << " => " << it->second << "(should not value -1)" << '\n';
			cout << "This pi in F.v but not in patch." << endl;
			exit(1);
		}
    }
	#endif
}


void init_mapvalue(map<string, int> &mymap)
{
	for (map<string, int>::const_iterator it = mymap.begin(); it != mymap.end(); ++it) {
		mymap[it->first] = -1;
		//cout << it->first << " => " << it->second << '\n';
    }
}


//Dependency Logic Network (DLN)
map<int, int> constructDLN(Solver &sat, Circuit_t &F_v_ckt, Circuit_t &patchckt1_only , Circuit_t &patchckt2_only, vector<int> &allcandidate)
{
	/*
	Note:
	Remember to check the PI "id" relationship of the three Circuit_t parameters, 
	or you won't get the result you expect.
	(The three Circuit_t parameters's PI id is supposed to the same in this function)
	
	Need: 
	F.v
	patch1(t=0)
	patch2(t=1)
	
	*/
	unsigned int i;
	
	map<string, int> relative_pi_f1; //key: name , value: id used in CNF
	map<string, int> relative_pi_f2; 
	map<string, int> relative_pi_patch1;
	map<string, int> relative_pi_patch2;
	map<int, int> id_map_assume;
	/*
	The order below will affect the circuit architecture.
	Because the SAT solver node id
	Do "not" change arbitrary!!
	*/
	relative_pi_f1 = fckt2CNF(sat, F_v_ckt, allcandidate, 0);
	sat_var_record[0] = sat.nVars();
	
	if (!sat.okay()){ cout << "fckt2CNF 1 solver is in contradictory state\n";/*exit(1);*/}
	
	relative_pi_patch1 = relative_pi_f1;
	init_mapvalue(relative_pi_patch1);
	
	
	patchckt2CNF(sat, patchckt1_only, relative_pi_patch1, sat_var_record[0]);
	sat_var_record[1] = sat.nVars();

	if (!sat.okay()){ cout << "patchckt2CNF 1 solver is in contradictory state\n"; /*exit(1);*/}
	// relative_pi_f1 and relative_pi_f2 's name should be the same
	relative_pi_f2 = fckt2CNF(sat, F_v_ckt, allcandidate, sat_var_record[1]);	
	sat_var_record[2] = sat.nVars();
	relative_pi_patch2 = relative_pi_f2;
	init_mapvalue(relative_pi_patch2);
	sat.toDimacs_nomap("mytest_A.cnf");
	
	if (!sat.okay()){ cout << "fckt2CNF 2 solver is in contradictory state\n";/*exit(1);*/}
	patchckt2CNF(sat, patchckt2_only, relative_pi_patch2, sat_var_record[2]);
	if (!sat.okay()){ cout << "patchckt2CNF 2 solver is in contradictory state\n"; /*exit(1);*/}
	sat_var_record[3] = sat.nVars();

	check_mapvalue(relative_pi_patch1);	
	check_mapvalue(relative_pi_patch2);	

	/*
	cout << "@@@ relative_pi_f1" << endl;
	print_map(relative_pi_f1);
	cout << "@@@ relative_pi_f2" << endl;
	print_map(relative_pi_f2);	
	
	cout << "@@@ relative_pi_patch1" << endl;
	print_map(relative_pi_patch1);
	cout << "@@@ relative_pi_patch2" << endl;
	print_map(relative_pi_patch2);
	
	for ( i = 0; i < 4; i++) {
		cout << "sat_var_record " << i << ' ' << sat_var_record[i] << '\n';
	}	
	*/
	bindpi2CNF(sat, relative_pi_f1, relative_pi_patch1);
	//sat.toDimacs("test5.cnf");
	bindpi2CNF(sat, relative_pi_f2, relative_pi_patch2);
	//sat.toDimacs("test6.cnf");
	if (!sat.okay()){ cout << "bindpi2CNF solver is in contradictory state\n"; /*exit(1);*/}
	id_map_assume = xor_SA1_2CNF(sat, F_v_ckt, allcandidate, sat_var_record[1]);
	if (!sat.okay()){ cout << "xor_SA1_2CNF solver is in contradictory state\n"; /*exit(1);*/}
	//cout << "len " << sat.nVars() << endl;
	//cout << "cal " << sat.nClauses() << endl;
	//sat.toDimacs("mytest_no_assume.cnf");
	//print_map(id_map_assume);
	return id_map_assume;

}



bool is_basenode_all_cover(Solver& sat, map<int, int> &id_map, const vector<int> &choosebase)
{
	unsigned int i;
	vec<Lit> assume;
	map<int, int>::const_iterator idmap_iter;
	//cout << "choosebase ";
	//print_vector(choosebase);
	//check if choosebase in map key
	// if not , doesn't make sense
	for (i = 0; i < choosebase.size(); i++) {
		if (id_map.find(choosebase[i]) == id_map.end()) {
			cout << "Error: " << choosebase[i] << " not in map" << endl;
			exit(1);
		}
	}

	for ( idmap_iter = id_map.begin(); idmap_iter != id_map.end(); ++idmap_iter) {
		//cout << idmap_iter->first << ' ' << idmap_iter->second << endl;
		if (find(choosebase.begin(), choosebase.end(), idmap_iter->first) != choosebase.end()) {
			//cout << idmap_iter->first << " is cand\n";
			//if     in choosebase==>assign assume 0
			assume.push(~mkLit(idmap_iter->second));
		} else {
			//cout << idmap_iter->first << " not cand\n";
			//if not in choosebase==>assign assume 1
			assume.push(mkLit(idmap_iter->second));
		}
		
    }
	//if UNSAT, choosebase can cover, return true
	if (sat.solve(assume) == false) {
		cout << "choosebase ";
		print_vector(choosebase);
		cout << "==> UNSAT ^____^ " << endl; 
		sat.toDimacs_nomap("mytest_AB.cnf", assume, sat_var_record[2]);	
	
		return true;
	} 
	//cout << "SAT " << endl; 
	return false;
}





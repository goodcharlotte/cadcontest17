#ifndef _DATATYPE_
#define _DATATYPE_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <math.h>
#include "core/minisat.h"

using namespace::std;

#define EQ_SAT 			( 0)
#define EQ_UNSAT		( 1)
#define EQ_INV_UNSAT	(-1)


#define EN_DEBUG_SIM 0

#if EN_DEBUG_SIM
#define debug_print(x) do { cout << x; } while (0)
#else 
#define debug_print(x) 
#endif


#define OFFSET_BIT 5

#if OFFSET_BIT == 6
#define SHOW_BIT_SET  64
#else 
#define SHOW_BIT_SET 32
#endif

#define GRAY_INIT -1
#define GRAY_NO_CHAGNE -2
#define SIM_ALL -1


vector<int> gene_graycode(int n);
void find_signature(vector< vector<string> > &sig, int target_size, int pi_size, int po_diff[]);
void print_signature(const vector< vector<string> > &sig);
void write_pla(const vector< vector<string> > &sig, vector<string> &piName, vector<string> &targetName,vector<int> graydiff);
void constructPatch(string cktF_name, string cktG_name);

//              0     1    2    3    4    5    6    7     8
enum GateType {BUF, NOT, AND, NAND, OR, NOR, XOR, NXOR, PORT};


	

class Node_t
{
public:
    string name;
    GateType type;
    int cost;
    bool patch_flag;
    vector<int> in;
    vector<int> out;
    Node_t(string n, GateType t);
    
    void print(vector<Node_t> allnode);
};

class Circuit_t
{
public:
    map<string, int> allnodemap;
    map<string, int>::iterator iter;
    
    vector<int> pi;
    vector<int> po;
    vector<int> topology_order;
    vector<int> topology_id;
	vector<int> greycode_diff;
    vector<Node_t> allnodevec;
    vector< vector<int> > fanout;
	vector<int> target;
	vector<int> po_value;
	vector<int> allnodevalue;

    Circuit_t();
	int euqal_ck(int F_nid, int P_nid);
	void CNF_fanin(Solver& sat, int node_id);

    bool readfile(char* fname);
    bool readpatch(char* fname);
    bool write_verilog(string cktname);
    bool writefile(char* fname, vector<int> candidate);
    bool readcost(char* fname);
    void print();
	void topology(int start_node_id);
    void init_simulation();
    void find_pi_fanout();
    void quicksort(vector<int>& array, int low, int high);
	void simulation(int gray_diff);
	void input_target_pattern(int target_value);
	int calculate_gate_out(GateType gate_type, vector<int> in);
	void print_pi();
	void print_po();
    void print_topology();
    int removebuffer();
    void removeredundant(vector<int> relatedPO);
    vector<int> findRelatedPO();
    vector<int> findRelatedPI(vector<int> relatedPO);
    void printstatus();
    void init_simp(string cktname);
	///////////////////////CCW/////////////////////////////////
	vector<int> newpi;
	int NewPI(Circuit_t &c2);
	void CompareNode(int node1, int node2, Circuit_t &c2);
	bool IsPiReplace(int node1, vector <int> &New, vector <int> &relateNode, vector <int> &NodetoCheck, vector <int> &visit, vector <int> &ready);
	void ReplacePi(vector <int> &New, vector <int> &RelateNode);
    void updateNewPI();

    vector<int> newpo;
};

#endif

#ifndef _DATATYPE_
#define _DATATYPE_

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <math.h>
#include <ctime>
#include <queue>
#include <algorithm>
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

#define INF 9999999
//#define DEBUG_MATCH
//#define DEBUG_READFILE2
//#define Debug_PO
//#define Debug_PO_2
//#define DEBUG_ChangeNewPO

extern clock_t start_clk;
extern clock_t stop_clk;

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
    string replacename;
    GateType type;
    int cost;
    bool patch_flag;
    vector<int> in;
    vector<int> out;
    vector<int> allpi;
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
    bool readpatch(string fname);
    bool readfile2(string fname);
    bool write_verilog(string cktname);
    bool writefile(char* iname, char* fname, vector<string> candidate, vector<string> patchPI);
    bool readcost(char* fname);
    void print();
	void topology(int start_node_id);
    vector<int> getsort_topology(vector<int>& nodevec);
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
    void findRelatedNode(vector<int> relatedPI, vector<int>& allpatchnode, vector<int>& allcandidate);
    void findReplaceCost(vector<int>& relatedPI, vector<int>& allcandidate, vector<int>& allpatchnode, vector<Node_t>& PatchNode);
    void findReplaceNode(vector<Node_t>& PatchNode);
    bool write_patch(vector<int>& relatedPI);
    void updatePatchPI(vector<int>& relatedPI, vector<string>& replaceName, vector<string>& patchName);
    void update_allpi();
    bool compareRelatedPI(vector<int>& relatedPI, int& node);
    vector<int> ReplaceNode(vector<int>& allcutnode);
    void sortcost(vector<int>& array, int left, int right);
    void printstatus();
    void init_simp(string cktname);
    int minCut(vector<int>& allcutnode);
	//Random simulation
	void random_sim_gene_input(vector<int> &relatedPI);
	void random_sim_patch(vector<int> &topology_order, int p_node);
	void random_sim_candidate(vector<int> &topology_order);
	vector<int> random_sim_compare(vector<int> &relatedPI, vector<int> &topo_order_cand, vector<int> &topo_order_patch, int patch_wire);
	
	
	///////////////////////CCW/////////////////////////////////
	#ifdef DEBUG_MATCH
		void Test();
		void CheckNode(int startNode);
	#endif
	bool write_verilog_after_sm(string cktname);
	void init_simp_after_sm(string cktname);
	void Translate(char* fName);
	vector< pair<int, int> > PseudoPO();
	pair<int, int> POMatching(pair<int, int> TerminationPair, int Fid, int Gid);
	vector<int> newpi;
	void ChangeNewPO(vector< pair<int, int> > CorresPO, Circuit_t &FpG, Circuit_t &G);
	int NewPI(Circuit_t &c2);
	void CompareNode(int node1, int node2, Circuit_t &c2);
	bool IsPiReplace(int node1, vector <int> &New, vector <int> &relateNode, vector <int> &NodetoCheck, vector <int> &visit, vector <int> &ready);
	void ReplacePi(vector <int> &New, vector <int> &RelateNode);
    void updateNewPI();

    vector<int> newpo;
};

#endif

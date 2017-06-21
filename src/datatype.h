#ifndef _DATATYPE_
#define _DATATYPE_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace::std;


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
#define GRAY_ERROR -2
#define GRAY_NO_CHAGNE -3
#define SIM_ALL -1

vector<int> generate_graycode(int n);	
void find_signature(vector< vector<string> > &sig, int target_leng ,int po_diff[]);
void print_signature(const vector< vector<string> > &sig);

//              0     1    2    3    4    5    6    7     8
enum GateType {BUF, NOT, AND, NAND, OR, NOR, XOR, NXOR, PORT};


	

class Node_t
{
private:
    string name;
    GateType type;

public:
    vector<int> in;
    vector<int> out;
    Node_t(string n, GateType t);
    inline void addinput(int i) { in.push_back(i); }
    inline void addoutput(int i) { out.push_back(i); }
    inline void setType(GateType t) { type = t; }
    inline GateType getType() { return type; }
    inline string getName() { return name; }
    void print(vector<Node_t> allnode);
};

class Circuit_t
{
private:
    vector<Node_t> allnodevec;
    map<string, int> allnodemap;
    map<string, int>::iterator iter;
    vector<int> pi;
    vector<int> po;
    vector<int> topology_order;
	vector<int> greycode_diff;
	
public:
    Circuit_t();
	vector<int> target;
	vector<int> po_value;
	vector<int> allnodevalue;
    bool readfile(char* fname);
    bool writefile(char* fname);
    void print();
	void topology(int start_node_id);
	int get_pi_size() { return pi.size(); }
	int get_po_size() { return po.size(); }
	void init_simulation();
	void simulation(int gray_diff);
	void input_target_pattern(int target_value);
	int calculate_gate_out(GateType gate_type, vector<int> in);
	void print_pi();
	void print_po();
    void print_topology();
};

#endif

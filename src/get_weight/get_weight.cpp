#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include "get_weight.h"

using namespace std;

string find_patch_inst(char *fname);
vector<string> find_base_node(string str);
int cal(vector<string> &patch_node, vector<Node_W*> &node_weight);

int calculate_weight(char *fname, vector<Node_W*> node_w)
{
	vector<string>  base_node = find_base_node(find_patch_inst(fname));
	#if EN_DEBUG
	cout << "-----Patch's base nodes-----\n"; 
	for ( int i = 0; i < base_node.size(); i++) {
		cout << base_node[i] <<endl;
	}
	#endif	
	return cal(base_node, node_w);
}

vector<Node_W*> read_weight(char *fname)
{
	ifstream file(fname, ios::in);
	vector<Node_W*> node_w;
	string n;
	int w;
	if (!file.is_open()) {
		cout << "Error : "<< fname << " open fail" << endl;
		exit(1);
	}
	
	while (file >> n >> w) {
		Node_W *tmp = new Node_W;
		tmp->node = n;
		tmp->weight = w;
		node_w.push_back(tmp);
	}
	return node_w;
}

string find_patch_inst(char *fname)
{
	ifstream file(fname, ios::in);
	if (!file.is_open()) {
		cout << "Error : "<< fname << " open fail" << endl;
		exit(1);
	}
	string line;
	while( getline(file,line)) {
		if(line.find("patch") != string::npos) {
			return line;
		}
	}

	cout << "Error : No find patch instance in " << fname << endl;
	exit(1);
}



vector<string> find_base_node(string str)
{
	vector<string> base_nodes;
	string tempstr = "";
	int flag_record = 0;
	for(int i = str.find("(.") + 1; i < str.find(");"); i++) {
		if (str[i] == '(') {
			flag_record = 1;
		} else if (str[i] == ')') {
			if (tempstr.find("t_") == string::npos) {
				base_nodes.push_back(tempstr);
			}	
			tempstr = "";
			flag_record = 0;
		} else if (flag_record) {
			tempstr += str[i];
		}	
	}
	return base_nodes;

}


int cal(vector<string> &patch_node, vector<Node_W*> &node_weight)
{
	int total_weight = 0;
	int flag_inf = 0;
	int found = 0;
	for ( int pn = 0; pn < patch_node.size(); pn++) {
		found = 0;
		for ( int nw = 0; nw < node_weight.size(); nw++) {
			if (patch_node[pn] == node_weight[nw]->node) {
				debug("Find  weight " << node_weight[nw]->weight << " at node " << patch_node[pn] << endl);
				total_weight+= node_weight[nw]->weight;
				found = 1;
				break;
			}
		}
		if (!found) {
			flag_inf = 1;
			cout << "Find  weight " << "INF" << " at node " << patch_node[pn] << endl;
		}
	}
	if (flag_inf) {
		return INF_WEIGHT;
	}
	return total_weight;
}


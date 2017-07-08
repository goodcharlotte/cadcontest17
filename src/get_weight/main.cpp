#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "get_weight.h"
using namespace std;

int main(int argc, char * argv[])
{

	//argv[1] is weight.txt filename
	//argv[2] is out.v filename
	vector<Node_W*> node_w = read_weight(argv[1]); 
	#if EN_DEBUG
	cout << "======read node weight======\n";
	for ( int i = 0; i < node_w.size(); i++) {
		cout << node_w[i]->node << " " <<  node_w[i]->weight << endl;
	} 
	#endif
	debug("======calculate weight======\n");
	int total_weight = calculate_weight(argv[2], node_w);
	
	cout << "======Total weight======\n";
	if (total_weight == INF_WEIGHT) {
		cout << "total weight " << "INF" << endl;		
	} else {
		cout << "total weight " << total_weight << endl;		
	}

	return 0;
}


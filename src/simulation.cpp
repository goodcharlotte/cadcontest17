#include <iostream>
#include <fstream>
#include <queue>
#include <string.h> 
#include <cstdlib>
#include <bitset>
#include <math.h>
#include "datatype.h"
using namespace::std;

#define GET_BIT(num,shift)  ((num >> shift ) & 1)
#define STR_COMPARE_FAIL  "fail"
#define SYMBOL_ERROR      'X'

string compare_minterm(string str1, string str2);
string to_str(int num, int leng);
vector<string> McCluskey(vector<string> minterm);


void print_signature(const vector< vector<string> > &sig)
{
	for (int i = 0; i < sig.size(); i++) {
		for (int j = 0; j < sig[i].size(); j++) {
			cout << sig[i][j] << " , ";
		}
		cout << endl;
	}	
}

string compare_minterm(string str1, string str2) {
	
	if (str1.size() != str2.size()) {
		cout << "Error : in compare_minterm , str1.size() != str2.size()" << endl;
		exit(1);
	}
	int size = str1.size();
	int diff_idx = -1;
	for (int i = 0; i < size; i++) {
		if (str1[i] != str2 [i]) {
			if (diff_idx < 0) {
				diff_idx = i;
			} else {
				return STR_COMPARE_FAIL;				
			}		
		}
	}
	str1[diff_idx]='-';
	return str1;
}

string to_str(int num, int leng)
{
	string str;
	for(int i = 0; i < leng; i++) {
		if (num & 1) {
			str = '1' + str;
		} else {			
			str = '0' + str;
		}
		num = num >> 1;
	}		
	return str;
}


vector<string> McCluskey(vector<string> minterm)
{
	int size = minterm.size();
	vector <string> vector_str_temp;
	string str_temp;
	if (size == 1) {
		debug_print(minterm[0] << " VS " << "...");
		debug_print( " ==> "<< minterm[0]<<endl);
		vector_str_temp.push_back(minterm[0]);
		return vector_str_temp;
	} else {
		for (int i = 0; i < size ; i++) {
			for (int j = i + 1; j < size; j++){
				debug_print(minterm[i] << " VS "<< minterm[j]);
				str_temp = compare_minterm(minterm[i], minterm[j]);
				debug_print(" ==> "<< str_temp <<endl);
				if (str_temp != STR_COMPARE_FAIL) {
					vector_str_temp.push_back(str_temp);
				}	
			}
		}
		if (vector_str_temp.size() > 0) {
			return McCluskey(vector_str_temp);
		} else {
			return minterm;
		}
	}


	
		
}

void find_signature(vector< vector<string> > &sig, int target_size, int pi_size, int po_diff[])
{
	
	
	vector<string> tar_minterm;
	string tar_str;
	int end = 0;
	int tar_leng = (int)pow(2, target_size);
	if (pi_size < OFFSET_BIT) {
		end = 	SHOW_BIT_SET - (int)pow(2, pi_size);
	}
	for (int bit = SHOW_BIT_SET - 1; bit >= end ; bit--) {
		debug_print("bit" << bit);
		debug_print("  minterm = ");
		tar_minterm.clear();
		for (int tar = 0; tar < tar_leng ; tar++) {
			if ( ! GET_BIT(po_diff[tar], bit)) {
				tar_str = to_str(tar, target_size);
				debug_print(tar_str << " ");
				tar_minterm.push_back(tar_str);			
			}			
		}
		debug_print("\n");
		tar_minterm = McCluskey(tar_minterm);
		if (tar_minterm.size() == 0) {
			tar_minterm.push_back(string(target_size, SYMBOL_ERROR));
		}
		sig.push_back(tar_minterm);
		debug_print("Final tar_minterm ");
		for (int i = 0; i < tar_minterm.size(); i++) {
			debug_print(tar_minterm[i] << " , ");
		}
		debug_print("\n=========\n");
	}
	
	
}


vector<int> generate_graycode(int n)
{
	debug_print("start generate_graycode...\n");
	vector<int> greySeq;
	vector<int> graydiff;
	
	greySeq.push_back(0);
	int inc = 1;
	for(int i = 1; i <= n; i++) {
		for(int j = greySeq.size()-1; j >= 0; j--) {
			greySeq.push_back(greySeq[j] + inc);
		}		
		inc <<= 1;
	}
	
	graydiff.push_back(GRAY_INIT);
	for (int i = 1; i <greySeq.size(); i++) {
		graydiff.push_back((int)(log2(greySeq[i-1] ^ greySeq[i])));
	} 
	graydiff.resize(greySeq.size(), GRAY_ERROR);
	
	#if 0
	cout<< "greySeq.size() = " << greySeq.size() <<endl;
	cout<< "graydiff.size() = " << graydiff.size() <<endl;
	for (int i = 0; i < greySeq.size(); i++) {
		
		cout<< bitset<SHOW_BIT_SET>(greySeq[i]) <<endl;
	}
	for (int i = 0; i < graydiff.size(); i++) {
		cout<< graydiff[i];
	}
	cout<<endl;
	#endif 
	
	debug_print("end generate_graycode...\n");
	return graydiff;
}






void Circuit_t::topology(int start_node_id)
{
    topology_order.clear();
    queue<int> wait_sort_node;
    int front_node;
    int input_node;
    bool ready_flag;
    bool find_flag;
    bool all_simulation;
    front_node = -1;
    ready_flag = false;
    all_simulation = false;
    input_node = -1;

	if (start_node_id == SIM_ALL) {
        all_simulation = true;
    } else {
        all_simulation = false;
    }

    //mark nodes ready to sort or not
    vector<bool> visit_flag(allnodevec.size(), !all_simulation);

	if (all_simulation) {
        //topology sort all nodes
        for (int i = 0; i < target.size(); i++) {
            wait_sort_node.push(target[i]);
            visit_flag[target[i]] = true;
        }

       for (int i = 0; i < pi.size(); i++) {
            wait_sort_node.push(pi[i]);
            visit_flag[pi[i]] = true;
       }

       //constant 0 & constant 1
       wait_sort_node.push(0);
       visit_flag[0] = true;
       wait_sort_node.push(1);
       visit_flag[1] = true;

       //find all fanout of start_node_id
       while (wait_sort_node.size() != 0) {
            front_node = wait_sort_node.front();
            for (int i = 0; i < allnodevec.size(); i++) {
                find_flag = false;
                ready_flag = true;
                if ((allnodevec[i].getType() != PORT) && (visit_flag[i] == false)) {
                    for (int in_i = 0; in_i < allnodevec[i].in.size(); in_i++) {
                        if (front_node == allnodevec[i].in[in_i]) {
                            find_flag = true;
                        }
                        if (visit_flag[ allnodevec[i].in[in_i] ] == false) {
                            ready_flag = false;
                            break;
                        }
                    }
                    if (ready_flag && find_flag) {
                        wait_sort_node.push(i);
                        visit_flag[i] = true;
                    }
                }
            }
            topology_order.push_back(front_node);
            wait_sort_node.pop();
       }

    } else {
        // assign start_node_id
        visit_flag[start_node_id] = false;
        wait_sort_node.push(start_node_id);

        // push t_x nodes fanouts
        for (int i = 0; i < target.size(); i++) {
            visit_flag[target[i]] = false;
            wait_sort_node.push(target[i]);
        }

        visit_flag[0] = false;
        wait_sort_node.push(0);
        visit_flag[1] = false;
        wait_sort_node.push(1);


       //find all fanout of start_node_id
       while (wait_sort_node.size() != 0) {
            front_node = wait_sort_node.front();
            for (int i = 0; i < allnodevec.size(); i++) {
                if ((allnodevec[i].getType() != PORT) && (visit_flag[i] == true)) {
                    ready_flag = true;
                    for (int in_i = 0; in_i < allnodevec[i].in.size(); in_i++) {
                        if (front_node == allnodevec[i].in[in_i]) {
                            ready_flag = false;
                            break;
                        }
                    }
                    if (ready_flag == false) {
                        wait_sort_node.push(i);
                    }
                }
            }
            wait_sort_node.pop();
            topology_order.push_back(front_node);
        }
    }
  

	#if 0
    if((start_node_id == -1)||(start_node_id == 12)){
    cout << "Topology(" << start_node_id << "):" << endl;
	for (int i = 0; i < topology_order.size(); i++) {
		cout << allnodevec[topology_order[i]].getName() << endl;
	}
    cout << endl;
    }
	#endif

}

 

void Circuit_t::input_target_pattern(int target_value)
{
	for(int i = 0; i<target.size(); i++) {
		if (target_value & (1 << i)) {
			allnodevalue[target[i]] = -1;   //all bits are 1
		} else {
			allnodevalue[target[i]] = 0;
		}
	}
}


int Circuit_t::calculate_gate_out(GateType gate_type, vector<int> in)
{
	int result = allnodevalue[in[0]];
	if (gate_type == BUF) {
		return result;
	} else if (gate_type == NOT) {
		return ~result; 
	} else { 
		for(int i = 1; i < in.size(); i++) {
			switch(gate_type) {  
				case AND:
				case NAND:
					result = result & allnodevalue[in[i]];  	
					break;
				case OR:
				case NOR:
					result = result | allnodevalue[in[i]];
					break;
				case XOR:
				case NXOR:
					result = result ^ allnodevalue[in[i]];
					break;
				case PORT:
					cout << "Error: calculate_gate_out case PORT, PI & target node should not go there!!" <<endl; 
					exit(1);
					break;
				default:
					cout << "Error: calculate_gate_out case default , should not go there!!" <<endl; 
					exit(1);
					break;					
			}  
		}
		
		if ((gate_type == NAND) || (gate_type == NOR) || (gate_type == NXOR)) {
			result = ~ result;

		}
	}
	
	
	
	
	return result;
}


void Circuit_t::print_pi()
{
	for(int i = 0; i < pi.size(); i++) { 
		cout << allnodevec[pi[i]].getName() <<"  "<< bitset<SHOW_BIT_SET>(allnodevalue[pi[i]]) << endl;
	}	
}


void Circuit_t::print_po()
{
	for(int i = 0; i < po_value.size(); i++) { 
		cout << allnodevec[po[i]].getName() <<"  "<< bitset<SHOW_BIT_SET>(po_value[i]) << endl;
	}
	
}


void Circuit_t::init_simulation() 
{
	
	#if OFFSET_BIT == 5
	int offset_value[5] = {1431655765,
                           858993459 ,
                           252645135 ,
                           16711935  ,
                           65535     };
	#elif OFFSET_BIT == 6
	long long int offset_value[6] = {6148914691236517205,
						   3689348814741910323,
						   1085102592571150095,
						   71777214294589695,
						   281470681808895,
						   4294967295};
	#else 
		int offset_value[5];
		cout << "Error : Please choose OFFSET_BIT = 5 or 6" <<endl;
		exit(1);
	#endif 
	
	input_target_pattern(0);
	int pi_szie = pi.size();
	if (pi_szie <= OFFSET_BIT) {
		for (int i = 0; i < pi_szie; i++) {
			allnodevalue[pi[i]] = offset_value[i];
		}
	}
	else {
		for (int i = 0; i < OFFSET_BIT; i++) {
			allnodevalue[pi[i]] = offset_value[i];
		}
		for (int i = OFFSET_BIT; i < pi_szie; i++) {
			allnodevalue[pi[i]] = 0;
		}
	}

    //constant 0 & constant 1
    if (allnodevalue.size() > 1) {
        allnodevalue[0] = 0;
        allnodevalue[1] = -1;
    }

}


void Circuit_t::simulation(int gray_diff)
{
	
	int gray_to_change = gray_diff + OFFSET_BIT;
	if (gray_diff == GRAY_ERROR) {
		cout <<"Error: Gray code has error!"<<endl;
		exit(1);
	} else if (gray_diff == GRAY_NO_CHAGNE) {
		//cout<<"GRAY_NO_CHAGNE "<<endl;
	} else if (gray_diff == GRAY_INIT) {
		//cout<<"GRAY_INIT "<<endl;
		topology(SIM_ALL);
		init_simulation();
	} else if (gray_diff >= 0) {
		allnodevalue[pi[gray_to_change]] = ~ (allnodevalue[pi[gray_to_change]]);
		topology(pi[gray_to_change]);
	}
	
	//cout<<"Before..."<<endl;
    //for(int i = 0; i < allnodevec.size(); i++) {
	//cout << allnodevec[i].getName() << " " << bitset<32>(allnodevalue[i]) << endl;
    //
    //}
	
	
	GateType type;
	for(int i = 0; i < topology_order.size(); i++) {
		type = allnodevec[topology_order[i]].getType();
		if (type != PORT) {
			//not PI, t_x, constant 0, constant 1
			allnodevalue[topology_order[i]] = calculate_gate_out(type, allnodevec[topology_order[i]].in);
		}
	}
	//cout<<"After..."<<endl;
    //for(int i = 0; i < allnodevec.size(); i++) {
        //cout << allnodevec[i].getName() << "\t" << bitset<32>(allnodevalue[i]) << endl;
    //}
	
	
	po_value.resize(0);
	for(int i = 0; i < po.size(); i++) { 
		po_value.push_back(allnodevalue[po[i]]);
	}
    //print_topology();	
}

void Circuit_t::print_topology()
{
    cout << "Topology value: " << endl;
    for(int i = 0; i < topology_order.size(); i++) {
        cout << allnodevec[topology_order[i]].getName() << " " << bitset<32>(allnodevalue[topology_order[i]]) << endl;
    }
    cout << endl;
}

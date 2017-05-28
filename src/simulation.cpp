#include <iostream>
#include <fstream>
#include <queue>
//#include <string> 
#include <stdlib.h>
#include <bitset>
#include <math.h>
#include "datatype.h"



using namespace::std;

vector<int> generate_graycode(int n)
{
	cout<<"start generate_graycode ..."<<endl;
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
		cout<< bitset<32>(greySeq[i]) <<endl;
	}
	for (int i = 0; i < graydiff.size(); i++) {
		cout<< graydiff[i];
	}
	cout<<endl;
	#endif 
	
	cout<<"end generate_graycode ..."<<endl;
	return graydiff;
}


void Circuit_t::topology(int start_node_id)
{
    topology_order.clear();
    queue<int> wait_sort_node;
    int front_node;
    int input_node;
    bool ready_flag;
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

    for (int i = 0; i < target.size(); i++) {
        topology_order.push_back(target[i]);
        visit_flag[target[i]] = true;
    }
 
	if (all_simulation) {
    //topology sort all nodes
       for (int i = 0; i < pi.size(); i++) {
            topology_order.push_back(pi[i]);
            visit_flag[pi[i]] = true;
        }


        //push all gates (except for: PIs, TARGETs)
        for (int i = 0; i < allnodevec.size(); i++) {
            if (allnodevec[i].getType() != PORT) {
                wait_sort_node.push(i);
            }
        }

        while (wait_sort_node.size() != 0)
        {
            ready_flag = true;
            front_node = wait_sort_node.front();

            for (int in_i = 0; in_i < allnodevec[front_node].in.size(); in_i++) {
                input_node = allnodevec[front_node].in[in_i];

                if ( visit_flag[input_node] == false ) {
                //not ready to push in topology_order
                    wait_sort_node.push(front_node);
                    ready_flag = false;
                    break;
                }
            }

            if (ready_flag == true) {
            //all inputs of the node have already been pushed
                topology_order.push_back(front_node);
                visit_flag[front_node] = true;
            }

            wait_sort_node.pop();
        }

    } else {
        // assign start_node_id
        visit_flag[start_node_id] = false;
        wait_sort_node.push(start_node_id);

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
                        visit_flag[i] = false;
                        wait_sort_node.push(i);
                    }
                }
            }
            wait_sort_node.pop();
            topology_order.push_back(front_node);
        }
    }

	//do by YP
    cout << "Topology(" << start_node_id << "):" << endl;
	for (int i = 0; i < topology_order.size(); i++) {
		cout << allnodevec[topology_order[i]].getName() << endl;
	}
    cout << endl;
}



void Circuit_t::input_target_pattern(int target_value)
{
	for(int i = 0; i<target.size(); i++) {
		allnodevalue[target[i]] = target_value & (1 << i);
	}

}


int Circuit_t::calculate_gate_out(GateType gate_type, vector<int> in)
{
	int result = 0;
	for(int i = 0; i < in.size(); i++) {
		if(i == 0) {
			result = allnodevalue[in[i]];
		}
		else {
			switch(gate_type)
			{  
				case BUF:  
					result = result;
					break;  
				case NOT:  
					result = ~result;  
					break;
				case AND:
					result = result & allnodevalue[in[i]];  	
					break;
				case NAND:
					result = ~(result & allnodevalue[in[i]]); 
					break;
				case OR:
					result = result | allnodevalue[in[i]];
					break;
				case NOR:
					result = ~(result | allnodevalue[in[i]]);
					break;
				case XOR:
					result = result ^ allnodevalue[in[i]];
					break;
				case NXOR:
					result = ~(result ^ allnodevalue[in[i]]);
					break;
				case PORT:
					cout << "Error: calculate_gate_out case PORT, PI & target node should not go there!!" <<endl; 
					exit(1);
					break;
			//	default:  
					  
			}  
		}
	}
	return result;
}


void Circuit_t::print_pi()
{
	for(int i = 0; i < pi.size(); i++) { 
		cout<<  bitset<32>(allnodevalue[pi[i]]) << endl;
	}
	
}


void Circuit_t::print_po()
{
	for(int i = 0; i < po_value.size(); i++) { 
		cout<<  bitset<32>(po_value[i]) << endl;
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
	int offset_value[6] = {6148914691236517205,
						   3689348814741910323,
						   1085102592571150095,
						   71777214294589695,
						   281470681808895,
						   4294967295};
	#else 
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
	
	
	GateType type;
	for(int i = 0; i < topology_order.size(); i++) {
		type = allnodevec[topology_order[i]].getType();
		if (type != PORT) {
			//not PI, t_x
			allnodevalue[topology_order[i]] = calculate_gate_out(type, allnodevec[topology_order[i]].in);
		}
	}
	po_value.resize(0);
	for(int i = 0; i < po.size(); i++) { 
		po_value.push_back(allnodevalue[po[i]]);
	}		
}

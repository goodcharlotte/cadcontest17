#include "datatype.h"
#include "string.h"

clock_t stop_clk;

void spilt_str(string& tmpstr)
{
    int start_str;
    int end_str;
	
	start_str = 0;
	end_str = tmpstr.size() - 1;
	
	for (int i = 0; i < tmpstr.size(); i++) {
		if (tmpstr[i] != '(') {
            start_str = i;
			break;
		}
	}

	for (int i = tmpstr.size() - 1; i >= 0; i--) {
		if ((tmpstr[i] != ';') && (tmpstr[i] != ')') && (tmpstr[i] != ',')) {
			end_str = i;
			break;
		}
	}
	
	tmpstr = tmpstr.substr(start_str, (end_str - start_str + 1));	
}

bool Circuit_t::readpatch(string fname)
{
    int originalsize;
    originalsize = allnodevec.size();

    for (int i = 0; i < allnodevec.size(); i++) {
        allnodevec[i].patch_flag = false;
        allnodevec[i].cost = INF;
    }

    ifstream file(fname.c_str());
    if (!file) return false;
    string tmpstr;
    int outidx;
	bool end_in;

    while (1) {
        file >> tmpstr;
		spilt_str(tmpstr);
        if (tmpstr == "module") {
            while (file >> tmpstr) {
                if (tmpstr[tmpstr.size() -1] == ';') break;
            }
        } else if (tmpstr == "input") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ";") break;
                iter = allnodemap.find(tmpstr);
                if (iter == allnodemap.end()) {
                    cout << "Error!" << tmpstr << "not in F." << endl;
                } else {
                    outidx = iter->second;
                    allnodevec[outidx].patch_flag = true;
                }
            }

        } else if (tmpstr == "output") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ";") break;
                iter = allnodemap.find(tmpstr);
                if (iter == allnodemap.end()) {
                    cout << "Error!" << tmpstr << "not in F." << endl;
                } else {
                    outidx = iter->second;
                    allnodevec[outidx].patch_flag = true;
                }
            }

        } else if (tmpstr == "wire") {
            while (file >> tmpstr) {
                if (tmpstr[tmpstr.size() - 1] == ';') break;
            }

        } else if (tmpstr == "endmodule") {
            break;

        } else {
            GateType tp;
            if (tmpstr == "buf") {
                tp = BUF;
            } else if (tmpstr == "not") {
                tp = NOT;
            } else if (tmpstr == "and") {
                tp = AND;
            } else if (tmpstr == "nand") {
                tp = NAND;
            } else if (tmpstr == "or") {
                tp = OR;
            } else if (tmpstr == "nor") {
                tp = NOR;
            } else if (tmpstr == "xor") {
                tp = XOR;
            } else if (tmpstr == "xnor") {
                tp = NXOR;
            } else {
                continue;
            }
            // gate output
            outidx = -1;
            file >> tmpstr >> tmpstr;
            //file >> tmpstr;
			spilt_str(tmpstr);
            iter = allnodemap.find(tmpstr);
            if (iter == allnodemap.end()) {
                tmpstr = "p_" + tmpstr;
            } else {
				outidx = iter->second;
                if (!allnodevec[outidx].patch_flag) {
                    //not PIs or POs in patch
                    tmpstr = "p_" + tmpstr;
                }
            }

            iter = allnodemap.find(tmpstr);
            if (iter == allnodemap.end()) {
                outidx = allnodevec.size();
                allnodemap[tmpstr] = allnodevec.size();
                allnodevec.push_back(*(new Node_t(tmpstr, tp)));        
                allnodevec[outidx].patch_flag = true;
                allnodevec[outidx].cost = INF;
            } else {
                outidx = iter->second;
                allnodevec[outidx].type = tp;
                allnodevec[outidx].patch_flag = true;
            }

           // gate input
            int inidx;
			end_in = false;
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
				if ((tmpstr.size() > 2) && (tmpstr[tmpstr.size() - 1] == ';')) {
                    end_in = true;
                } else if (tmpstr[tmpstr.size() - 1] == ';') {
                    break;
                }
				spilt_str(tmpstr);
                if (tmpstr == "1'b0") {
                    inidx = 0;
                } else if (tmpstr == "1'b1") {
                    inidx = 1;
                } else {
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
                        tmpstr = "p_" + tmpstr;
                    } else {
                        inidx = iter->second;
                        if (!allnodevec[inidx].patch_flag) {
                            tmpstr = "p_" + tmpstr;
                        }
                    }
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
                        inidx = allnodemap[tmpstr] = allnodevec.size();
                        allnodevec.push_back(*(new Node_t(tmpstr, PORT)));
                        allnodevec[inidx].patch_flag = true;
                        allnodevec[inidx].cost = INF;
                    } else {
                        inidx = iter->second;
                    }
                }
                allnodevec[outidx].in.push_back(inidx);
                allnodevec[inidx].out.push_back(outidx);
				
				if (end_in) break;
            }
        }
    }
    
    file.close();
	allnodevalue.resize(allnodevec.size());

    return true;

}

bool Circuit_t::readfile2(string fname)
{
    int originalsize;
    originalsize = allnodevec.size();

    ifstream file(fname.c_str());
    if (!file) return false;
    string tmpstr;
    int outidx;
	bool end_in;

    while (1) {
        file >> tmpstr;
		spilt_str(tmpstr);
        if (tmpstr == "module") {
            while (file >> tmpstr) {
                if (tmpstr[tmpstr.size() -1] == ';') break;
            }

        } else if (tmpstr == "input") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ";") break;
				iter = allnodemap.find(tmpstr);
				if (iter == allnodemap.end()) {
					#ifdef DEBUG_READFILE2
						cout<<"PI : " <<tmpstr<<" not in F"<<endl;
					#endif
					allnodemap[tmpstr] = allnodevec.size();
					pi.push_back(allnodevec.size());
					allnodevec.push_back(*(new Node_t(tmpstr, PORT)));
				} else {
					
				}
            }

        } else if (tmpstr == "output") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr[tmpstr.size() - 1] == ';') break;
                tmpstr = "g_" + tmpstr;
                allnodemap[tmpstr] = allnodevec.size();
                po.push_back(allnodevec.size());
                allnodevec.push_back(*(new Node_t(tmpstr, PORT)));       
            }

        }else if (tmpstr == "wire") {
            while (file >> tmpstr) {
                if (tmpstr[tmpstr.size() - 1] == ';') break;
            }

        } else if (tmpstr == "endmodule") {
            break;

        } else {
            GateType tp;
            if (tmpstr == "buf") {
                tp = BUF;
            } else if (tmpstr == "not") {
                tp = NOT;
            } else if (tmpstr == "and") {
                tp = AND;
            } else if (tmpstr == "nand") {
                tp = NAND;
            } else if (tmpstr == "or") {
                tp = OR;
            } else if (tmpstr == "nor") {
                tp = NOR;
            } else if (tmpstr == "xor") {
                tp = XOR;
            } else if (tmpstr == "xnor") {
                tp = NXOR;
            } else {
                continue;
            }
            // gate output
            outidx = -1;
            file >> tmpstr >> tmpstr;
			spilt_str(tmpstr);
            tmpstr = "g_" + tmpstr;
            iter = allnodemap.find(tmpstr);

            if (iter == allnodemap.end()) {
                outidx = allnodevec.size();
                allnodemap[tmpstr] = allnodevec.size();
                allnodevec.push_back(*(new Node_t(tmpstr, tp)));        
            } else {
                outidx = iter->second;
                allnodevec[outidx].type = tp;
            }

           // gate input
            int inidx;
			end_in = false;
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ");") break;
				if (tmpstr[tmpstr.size() - 1] == ';') end_in = true;
				spilt_str(tmpstr);
                if (tmpstr == "1'b0") {
                    inidx = 0;
                } else if (tmpstr == "1'b1") {
                    inidx = 1;
                } else {
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
                        tmpstr = "g_" + tmpstr;
                    } else {
                        inidx = iter->second;
                        if (allnodevec[inidx].type == PORT) {
                        } else {
                            tmpstr = "g_" + tmpstr;
                        }
                    }
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
                        inidx = allnodemap[tmpstr] = allnodevec.size();
                        allnodevec.push_back(*(new Node_t(tmpstr, PORT)));
                    } else {
                        inidx = iter->second;
                    }
                }
                allnodevec[outidx].in.push_back(inidx);
                allnodevec[inidx].out.push_back(outidx);
				
				if (end_in) break;
            }
        }
    }
    
    file.close();
	allnodevalue.resize(allnodevec.size());

    return true;

}

#if 0
void Circuit_t::findRelatedNode(vector<int> relatedPO, vector<int>& allpatchnode, vector<int>& allcandidate)
{
    queue<int> nodeque;
    vector<bool> visit_flag(allnodevec.size(), false);
    vector<bool> target_fanout(allnodevec.size(), false);

    int node; 
    for (int i = 0; i < target.size(); i++) {
        nodeque.push(target[i]);
    }

    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].out.size(); i++) {
            if (target_fanout[allnodevec[node].out[i]] == false) {
                nodeque.push(allnodevec[node].out[i]);
            }
        }
        target_fanout[node] = true;
        nodeque.pop();
    }
    
    //////////////// BFS (from POs) ///////////////
    bool ready_bfs;
    for (int i = 0; i < relatedPO.size(); i++) {
        node = relatedPO[i];
        nodeque.push(node);
    }

    allcandidate.resize(0);
    while (nodeque.size() != 0) {
        node = nodeque.front();
        if (visit_flag[node] == false) {
            if ((allnodevec[node].in.size() != 0) || (allnodevec[node].out.size() != 0)) {
                for (int fanin = 0; fanin < allnodevec[node].in.size(); fanin++) {
                    int fanin_node = allnodevec[node].in[fanin];
                    nodeque.push(fanin_node);
                }
                if (((target_fanout[node] == false) && (allnodevec[node].patch_flag == false)) || (allnodevec[node].type == PORT)) {
                    allcandidate.push_back(node);
                }
                if (allnodevec[node].patch_flag == true)
                {
                    allpatchnode.push_back(node);
                }
            }
        }
        visit_flag[node] = true;
        nodeque.pop();
    }
}
#endif

void Circuit_t::findRelatedNode(vector<int> relatedPI, vector<int>& allpatchnode, vector<int>& allcandidate)
{
    queue<int> nodeque;
    vector<bool> visit_flag(allnodevec.size(), false);
    vector<bool> target_fanout(allnodevec.size(), false);

    int node; 
    for (int i = 0; i < target.size(); i++) {
        nodeque.push(target[i]);
    }

    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].out.size(); i++) {
            if (target_fanout[allnodevec[node].out[i]] == false) {
                nodeque.push(allnodevec[node].out[i]);
            }
        }
        target_fanout[node] = true;
        nodeque.pop();
    }
    
    //////////////// BFS (from PIs) ///////////////
    bool ready_bfs;
    for (int i = 0; i < relatedPI.size(); i++) {
        node = relatedPI[i];
        nodeque.push(node);
    }

    allcandidate.resize(0);
    while (nodeque.size() != 0) {
        node = nodeque.front();
        if (visit_flag[node] == false) {
            ready_bfs = compareRelatedPI(relatedPI, node);
            //ready_bfs = true;
            if (ready_bfs == true) {
                if ((allnodevec[node].in.size() != 0) || (allnodevec[node].out.size() != 0)) {
                    for (int fanout = 0; fanout < allnodevec[node].out.size(); fanout++) {
                        int fanout_node = allnodevec[node].out[fanout];
                        nodeque.push(fanout_node);
                    }
                    if (((target_fanout[node] == false) && (allnodevec[node].patch_flag == false)) || (allnodevec[node].type == PORT)) {
                        allcandidate.push_back(node);
                    }
                    if (allnodevec[node].patch_flag == true) {
                        allpatchnode.push_back(node);
                    }
                }
            }
        }
        visit_flag[node] = true;
        nodeque.pop();
    }

    /*
    cout << "===== BFS (from PIs) =====" << endl;
    for (int i = 0; i < allpatchnode.size(); i++) {
        cout << allnodevec[allpatchnode[i]].name << endl;
    }
    */
}




void Circuit_t::sortcost(vector<int>& array, int left, int right)
{
    if (left < right) {
        // divide (partition)
        int pivot = array[(left + right) / 2];
        int i = left - 1, j = right + 1;
        while (i < j) {
            do {
                ++i;
            } while (allnodevec[array[i]].cost < allnodevec[pivot].cost);

            do {
                --j;
            } while (allnodevec[array[j]].cost > allnodevec[pivot].cost);
          
            if (i < j) {
                swap(array[i], array[j]);
            }
        }

        // then conquer
        sortcost(array, left, i - 1);
        sortcost(array, j + 1, right);
    }
}

void Circuit_t::removecostINF(vector<int>& allcandidate)
{
    //cout << "WARNING! If remove, can not topology!" << endl;
    while (allnodevec[allcandidate.back()].cost == INF) {
        allcandidate.pop_back();
        if (allcandidate.size() == 0) {
            break;
        }
    }
}

vector<int> Circuit_t::getsort_topology(vector<int>& nodevec)
{
    vector<int> topology_vec;
    vector<bool> visit_flag(allnodevec.size(), false);
	visit_flag[0] = true;
    visit_flag[1] = true;
    queue<int> nodeque;
    bool ready_sort;

    int node;
    for (int i = 0; i < nodevec.size(); i++) {
        node = nodevec[i];
        nodeque.push(node);
    }
    while (nodeque.size() != 0) {
        node = nodeque.front();
        if (visit_flag[node] == false) {
            ready_sort = true;
            for (int fanin = 0; fanin < allnodevec[node].in.size(); fanin++) {
                int fanin_node = allnodevec[node].in[fanin];
                if (visit_flag[fanin_node] == false) {
                    ready_sort = false;
                    break;
                }
            }

            if (ready_sort == true) {
                topology_vec.push_back(node);
                visit_flag[node] = true;
            } else {
                nodeque.push(node);
            }
        }
        nodeque.pop();
    }
#if 0
    cout << "===== Original vector: =====" << endl;
    for (int i = 0; i < nodevec.size(); i++) {
        node = nodevec[i];
        cout << allnodevec[node].name << endl;
    }
    cout << "===============" << endl;
    cout << "===== Topology-sort vector: =====" << endl;
    for (int i = 0; i < topology_vec.size(); i++) {
        node = topology_vec[i];
        cout << allnodevec[node].name << endl;
    }
    cout << "===============" << endl;
#endif

    if (topology_vec.size() != nodevec.size()) {
        cout << "ERROR! Topology vector is equal to node vector" << endl;
    }
    return topology_vec;
}

void Circuit_t::findReplaceCost(vector<int>& relatedPI, vector<int>& allcandidate, vector<int>& allpatchnode, vector<Node_t>& PatchNode)
{
	vector<int> topo_order_cand = getsort_topology(allcandidate);
	vector<int> topo_order_patch = getsort_topology(allpatchnode);
	//cout << "topo_order_cand size " << topo_order_cand.size() << endl;
	vector<int> possible_candidate;
    vector<int> nobuf_possible_candidate;
    int timeout = 0;
    if (target.size() == 1) {
        timeout = MID_TIME_LIMIT;
    } else {
        timeout = TIME_LIMIT;
    }

	for (int p_wire = 0; p_wire < allpatchnode.size(); p_wire++) {
        int p_node = allpatchnode[p_wire];
        bool find_replace = false;
		possible_candidate = random_sim_compare(relatedPI, topo_order_cand, topo_order_patch, p_node);
        nobuf_possible_candidate = check_cost(possible_candidate);
        sortcost(nobuf_possible_candidate, 0, nobuf_possible_candidate.size() - 1);
		//cout << "===current p wire " << allnodevec[allpatchnode[p_wire]].name << endl;
		//for(int i = 0; i < possible_candidate.size(); i++) {
			//cout << allnodevec[possible_candidate[i]].name << " " << allnodevec[possible_candidate[i]].cost << endl;
		//}
		for (int can_wire = 0; can_wire < nobuf_possible_candidate.size(); can_wire++) {
            //cout << "time: " << time_sec << endl;
            stop_clk = clock();
            double time_sec = double(stop_clk - start_clk)/CLOCKS_PER_SEC;
            if ( time_sec > timeout ) {
                break;
            }
            int can_node = nobuf_possible_candidate[can_wire];
            if (allnodevec[can_node].cost == INF) {
                break;
            }
			//cout << "allnodevec[can_node].name " << allnodevec[can_node].name << endl;
            int check_equal = euqal_ck(can_node, p_node);
            //int check_equal = EQ_SAT;
            if (check_equal == EQ_UNSAT) {
                find_replace = true;
                allnodevec[p_node].cost = allnodevec[can_node].cost;
                allnodevec[p_node].replacename = allnodevec[can_node].name;
                //cout << "check_equal == EQ_UNSAT " << allnodevec[can_node].name << " " <<allnodevec[can_node].cost << endl;
				break;
            } else if (check_equal == EQ_INV_UNSAT) {
                find_replace = true;
                allnodevec[p_node].cost = allnodevec[can_node].cost * (-1);
                allnodevec[p_node].replacename = allnodevec[can_node].name;
               // cout << "check_equal == EQ_INV_UNSAT " << allnodevec[can_node].name <<" " << allnodevec[can_node].cost << endl;
				break;
            }
			/*
			else {
				cout << "check_equal == EQ_SAT " << allnodevec[can_node].name <<" " << allnodevec[can_node].cost << endl;
			}
			*/
        }		
		
		/*
		directly use all F's node as candidates, no do random simulation
		for (int can_wire = 0; can_wire < allcandidate.size(); can_wire++) {
            //cout << "time: " << time_sec << endl;
            stop_clk = clock();
            double time_sec = double(stop_clk - start_clk)/CLOCKS_PER_SEC;
            if ( time_sec > TIME_LIMIT ) {
                break;
            }
            int can_node = allcandidate[can_wire];
            int check_equal = euqal_ck(can_node, p_node);
            //int check_equal = EQ_SAT;
            if (check_equal == EQ_UNSAT) {
                find_replace = true;
                allnodevec[p_node].cost = allnodevec[can_node].cost;
                allnodevec[p_node].replacename = allnodevec[can_node].name;
                break;
            } else if (check_equal == EQ_INV_UNSAT) {
                find_replace = true;
                allnodevec[p_node].cost = allnodevec[can_node].cost * (-1);
                allnodevec[p_node].replacename = allnodevec[can_node].name;
                break;
            }
        }
		*/
		
        if (!find_replace) {
            allnodevec[p_node].replacename = "NONE";
        }
    }

    int patch_node;
    string tmpstr;
    for (int i = 0; i < allpatchnode.size(); i++) {
        patch_node = allpatchnode[i];
        PatchNode.push_back(*(new Node_t(allnodevec[patch_node].name, allnodevec[patch_node].type)));
        PatchNode[i].cost = allnodevec[patch_node].cost;
        PatchNode[i].replacename = allnodevec[patch_node].replacename;
    }
}

void Circuit_t::findReplaceNode(vector<Node_t>& PatchNode)
{
    int node;
    string tmpstr;
    for (int i = 0; i < PatchNode.size(); i++) {
        tmpstr = PatchNode[i].name;
        if (PatchNode[i].type != PORT) {
            if (PatchNode[i].name[0] == 'p') {
                tmpstr = tmpstr.substr(2);
            }
        }
        iter = allnodemap.find(tmpstr);
        if (iter == allnodemap.end()) {
            cout << "Error!" << tmpstr << " not in Patch(only)." << endl;
        } else {
            node = iter->second;
            allnodevec[node].cost = PatchNode[i].cost;
            allnodevec[node].replacename = PatchNode[i].replacename;
        }   
   }
}

vector<int> Circuit_t::ReplaceNode(vector<int>& allcutnode)
{
    vector<bool> cut_flag(allnodevec.size(), false);
	vector<bool> remove_flag(allnodevec.size(), false);
    vector<int> relatedPI;
	queue<int> nodeque;
    int node;
    bool remove;
	
    // mark all cut nodes
    for (int i = 0; i < allcutnode.size(); i++) {
        node = allcutnode[i];
        cut_flag[node] = true;
    }

    for (int i = 0; i < allnodevec.size(); i++) {
        if (cut_flag[i] == true) {
            remove = true;
            for (int fanout = 0; fanout < allnodevec[i].out.size(); fanout++) {
                node = allnodevec[i].out[fanout];
                if (cut_flag[node] == false) {
                    remove = false;
                    break;
                }
            }

            if (remove == true) {
                allnodevec[i].in.clear();
                allnodevec[i].out.clear();
            } else {
                relatedPI.push_back(i);
            }
        }
    }
    
	// find all fanout nodes of relatedPI
    /*
	for (int i = 0; i < relatedPI.size(); i++) {
		nodeque.push(relatedPI[i]);
	}
	
	while (nodeque.size() != 0 ) {
		node = nodeque.front();
		for (int fanin = 0; fanin < allnodevec[node].in.size(); fanin++) {
			nodeque.push(allnodevec[node].in[fanin]);
		}
		remove_flag[node] = true;
		nodeque.pop();
	}
	
	for (int i = 0; i < relatedPI.size(); i++) {
		remove_flag[relatedPI[i]] = false;
	}
	
	for (int i = 0; i < allnodevec.size(); i++) {
		if (remove_flag[i] == true) {
			allnodevec[i].in.clear();
			allnodevec[i].out.clear();
		}
	}
	*/
    return relatedPI;
}

bool Circuit_t::write_patch(vector<int>& relatedPI)
{
    string fname = "mincut_patch.v";
    ofstream file(fname.c_str());
    bool first = true;
    bool skip = true;
    if (!file) return false;
    file << "module " << " patch (" ;
    vector<int> allinput;
    vector<int> alloutput;
    vector<int> addINV;
	
	for (int i = 0; i < allnodevec.size(); i++) {
		allnodevec[i].patch_flag = true;
	}
	
    for (int i = 0; i < po.size(); i++) {
        if (allnodevec[po[i]].in.size() > 0) {
            alloutput.push_back(po[i]);
            file << allnodevec[po[i]].name << " , ";
        }
    }
	
    for (int i = 0; i < relatedPI.size(); i++) {
        if (allnodevec[relatedPI[i]].out.size() > 0) {
            if (allnodevec[relatedPI[i]].cost < 0) {
                addINV.push_back(relatedPI[i]);
            }
			allinput.push_back(relatedPI[i]);        
			allnodevec[relatedPI[i]].in.clear();
			allnodevec[relatedPI[i]].type = PORT;
            
			if (first) {
                file << allnodevec[relatedPI[i]].name;
                first = false;
            } else {
                file << " , " << allnodevec[relatedPI[i]].name;
            }
        }
    }

    file  << " );" << endl;

    file << "input ";
    for (int i = 0; i<allinput.size(); i++) {
        if (i != allinput.size() - 1)
            file <<  allnodevec[allinput[i]].name << " , ";
        else
            file <<  allnodevec[allinput[i]].name << " ;" << endl;
    }

    file << "output ";
    for (int i = 0; i<alloutput.size(); i++) {
        if (i != alloutput.size() - 1)
            file <<  allnodevec[alloutput[i]].name << " , ";
        else
            file <<  allnodevec[alloutput[i]].name << " ;" << endl;
    }

    file << "wire ";
    first = true;
    for (int i = 0; i < allnodevec.size(); i++) {
        Node_t node = allnodevec[i];
        if (node.in.size() == 0) continue; // PORT, useless node
		if (node.name[0] == 't') continue; // t_0, t_1, ...
        if (first) {
            file << node.name;
            first = false;
        } else {
            file << " , " << node.name;
        }
    }
    for (int i = 0; i < addINV.size(); i++) {
        int add_idx = allnodevec.size();
        string tmpstr = "INV_" + allnodevec[addINV[i]].name;
		file << " , " << tmpstr;
		
        allnodevec.push_back(*(new Node_t(tmpstr, NOT)));
		allnodevec[add_idx].in.push_back(addINV[i]);
		
		// replace fanout
		for (int fanout = 0; fanout < allnodevec[addINV[i]].out.size(); fanout++) {
			int fanout_node = allnodevec[addINV[i]].out[fanout];
			for (int fanin = 0; fanin < allnodevec[fanout_node].in.size(); fanin++) {
				int fanin_node = allnodevec[fanout_node].in[fanin];
				if (fanin_node == addINV[i]) {
					allnodevec[fanout_node].in[fanin] = add_idx;
					allnodevec[add_idx].out.push_back(fanout_node);
				}
			}
		}
		allnodevec[addINV[i]].out.clear();
        allnodevec[addINV[i]].out.push_back(add_idx);
    }

    file <<  " ;" << endl;

    for (int i = pi.size()+2; i < allnodevec.size(); i++) {
        Node_t node = allnodevec[i];
        if (node.in.size() == 0) continue; // t_0, t_1, ...
        if (node.type == BUF) {
            file << "buf ( " << node.name; 
        } else if (node.type == NOT) {
            file << "not ( " << node.name; 
        } else if (node.type == AND) {
            file << "and ( " << node.name; 
        } else if (node.type == NAND) {
            file << "nand ( " << node.name; 
        } else if (node.type == OR) {
            file << "or ( " << node.name; 
        } else if (node.type == NOR) {
            file << "nor ( " << node.name; 
        } else if (node.type == XOR) {
            file << "xor ( " << node.name; 
        } else if (node.type == NXOR) {
            file << "xnor ( " << node.name; 
        }
        for (int j=0; j<node.in.size(); j++) {
            file << " , " << allnodevec[node.in[j]].name;
        }
        file << " );" << endl;

    }
    file << "endmodule" << endl;
    file.close();
    return true;
}

void Circuit_t::updatePatchPI(vector<int>& relatedPI, vector<string>& replaceName, vector<string>& patchName)
{
    pi.clear();
    pi.resize(relatedPI.size());
    replaceName.resize(relatedPI.size());
    patchName.resize(relatedPI.size());
    for (int i = 0; i < relatedPI.size(); i++) {
        pi[i] = relatedPI[i];
        if (allnodevec[relatedPI[i]].replacename == "NONE") {
            replaceName[i] = allnodevec[relatedPI[i]].name;
        } else {
            replaceName[i] = allnodevec[relatedPI[i]].replacename;
        }
        patchName[i] = allnodevec[relatedPI[i]].name;
    }
}

bool Circuit_t::compareRelatedPI(vector<int>& relatedPI, int& node)
{
    bool compare_flag = true;

    if (allnodevec[node].allpi.size() > relatedPI.size()) {
        compare_flag = false;
    } else {
        for (int allpi_i = 0; allpi_i < allnodevec[node].allpi.size(); allpi_i++) {
            bool find_flag = false;
            for (int repi_i = 0; repi_i < relatedPI.size(); repi_i++) {
                if (allnodevec[node].allpi[allpi_i] == relatedPI[repi_i]) {
                    find_flag = true;
                    break;
                }
                if (allnodevec[node].allpi[allpi_i] < relatedPI[repi_i]) {
                    find_flag = false;
                    break;
                }
            }
            if (find_flag == false) {
                compare_flag = false;
                break;
            }
        }
    }

    return compare_flag;
}

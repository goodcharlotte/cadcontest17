#include "datatype.h"
#include "string.h"


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
                if (tmpstr[tmpstr.size() -1] == ';') break;
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
    
    //////////////// BFS ///////////////
    bool ready_bfs;
    for (int i = 0; i < relatedPO.size(); i++) {
        node = relatedPO[i];
        nodeque.push(node);
    }

    allcandidate.resize(0);
    
    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].in.size(); i++) {
            int indx = allnodevec[node].in[i];
            if (visit_flag[indx] == false) {
                nodeque.push(allnodevec[node].in[i]);
            }
        }

        ready_bfs = true;
        for (int i = 0; i < allnodevec[node].out.size(); i++) {
            int outdx = allnodevec[node].out[i];
            if ((allnodevec[outdx].patch_flag == true) && (visit_flag[outdx] == false)) {
                ready_bfs = false;
                break;
            }
        }

        if ((ready_bfs == true) && (visit_flag[node] == false)) {
            visit_flag[node] = true;
            if (!target_fanout[node]) {
                if ((!allnodevec[node].patch_flag) || (allnodevec[node].type == PORT)) {
                    allcandidate.push_back(node);
                }
            }
            if (allnodevec[node].patch_flag) {
                allpatchnode.push_back(node);
            }
        }
        nodeque.pop();
    }
}

/*
vector<int> Circuit_t::findPatchNode(vector<int> relatedPO)
{
    //BFS
    queue<int> nodeque;
    vector<bool> visit_flag(allnodevec.size(), false);

    int node; 
    
    for (int i = 0; i < relatedPO.size(); i++) {
        node = relatedPO[i];
        nodeque.push(node);
    }

    vector<int> relatedNode;
    
    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].in.size(); i++) {
            if (visit_flag[allnodevec[node].in[i]] == false) {
                nodeque.push(allnodevec[node].in[i]);
            }
        }
        visit_flag[node] = true;
        if (!target_fanout[node]) {
            if ((!allnodevec[node].patch_flag) || (allnodevec[node].type == PORT)) {
                relatedNode.push_back(node);
            }
        }
        nodeque.pop();
    }

    return relatedNode;
}
*/

void Circuit_t::sortcost(vector<int>& array, int left, int right)
{
    for (int i = 0; i < array.size() - 1; i++) {
        for (int j = i + 1; j < array.size(); j++) {
            if (allnodevec[array[i]].cost > allnodevec[array[j]].cost) {
                swap(array[i], array[j]);
            }
        }
    }
    
    /*
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
        quicksort(array, left, i - 1);
        quicksort(array, j + 1, right);
    }
    */
}


void Circuit_t::findReplaceCost(vector<int>& ReplaceNode, vector<int>& allcandidate, vector<int>& allpatchnode, vector<Node_t>& PatchNode)
{
    clock_t clk_start, clk_stop;
    clk_start = clock();

    for (int p_wire = 0; p_wire < allpatchnode.size(); p_wire++) {
        int p_node = allpatchnode[p_wire];
        bool find_replace = false;
        for (int can_wire = 0; can_wire < allcandidate.size(); can_wire++) {
            clk_stop = clock();
            double time_sec = double(clk_stop - clk_start)/CLOCKS_PER_SEC;
            //cout << "time: " << time_sec << endl;
            if ( time_sec > 1500) {
                break;
            }
            int can_node = allcandidate[can_wire];
            int check_equal = euqal_ck(can_node, p_node);
            if (check_equal == EQ_UNSAT) {
                find_replace = true;
                ReplaceNode[p_wire] = allcandidate[can_wire];
                allnodevec[p_node].cost = allnodevec[can_node].cost;
                break;
            } else if (check_equal == EQ_INV_UNSAT) {
                find_replace = true;
                ReplaceNode[p_wire] = allcandidate[can_wire];
                allnodevec[p_node].cost = allnodevec[can_node].cost * (-1);
                break;
            }
        }
        if (!find_replace) {
            ReplaceNode[p_wire] = -1;
        }
    }

    int patch_node, replace_node;
    for (int i = 0; i < allpatchnode.size(); i++) {
        patch_node = allpatchnode[i];
        replace_node = ReplaceNode[i];
        PatchNode.push_back(*(new Node_t(allnodevec[patch_node].name, allnodevec[patch_node].type)));
        if (replace_node == -1) {
            PatchNode[i].cost = INF;
            PatchNode[i].replacename = "NONE";
        } else {
            PatchNode[i].cost = allnodevec[replace_node].cost;
            PatchNode[i].replacename = allnodevec[replace_node].name;
        } 
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
            //cout << "find id" << node << " in Patch(only)." << endl; 
            allnodevec[node].cost = PatchNode[i].cost;
            allnodevec[node].replacename = PatchNode[i].name;
        }   
   }
}

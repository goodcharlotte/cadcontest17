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

bool Circuit_t::readpatch(char* fname)
{
    int originalsize;
    originalsize = allnodevec.size();

    for (int i = 0; i < allnodevec.size(); i++) {
        allnodevec[i].patch_flag = false;
    }

    ifstream file(fname);
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
            //file >> tmpstr >> tmpstr;
            file >> tmpstr;
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

bool Circuit_t::readfile2(char* fname)
{
    int originalsize;
    originalsize = allnodevec.size();

    ifstream file(fname);
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

vector<int> Circuit_t::findRelatedNode(vector<int> relatedPI)
{
    queue<int> nodeque;
    vector<bool> visit_flag(allnodevec.size(), false);

    int node; 
    for (int i = 0; i < relatedPI.size(); i++) {
        node = relatedPI[i];
        nodeque.push(node);
    }

    vector<int> relatedNode;
    
    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].out.size(); i++) {
            if (visit_flag[allnodevec[node].out[i]] == false) {
                nodeque.push(allnodevec[node].out[i]);
            }
        }
        visit_flag[node] = true;
        nodeque.pop();
    }

    
    for (int i = 0; i < target.size(); i++) {
        nodeque.push(target[i]);
    }

    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].out.size(); i++) {
            if (visit_flag[allnodevec[node].out[i]] == true) {
                nodeque.push(allnodevec[node].out[i]);
            }
        }
        visit_flag[node] = false;
        nodeque.pop();
    }

    for (int i = 0; i < allnodevec.size(); i++) {
        if (visit_flag[i] && !allnodevec[i].patch_flag) {
            relatedNode.push_back(i);
        }
    }

    for (int i = 0; i < relatedPI.size(); i++) {
        node = relatedPI[i];
        if ((allnodevec[node].patch_flag) || (!visit_flag[node])){
            relatedNode.push_back(node);
        }
    }

    return relatedNode;
}


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


void Circuit_t::findReplaceCost(vector<int>& ReplaceNode, vector<int>& ReplaceCost, vector<int>& allcandidate, vector<int>& allpatchnode)
{
    for (int p_wire = 0; p_wire < allpatchnode.size(); p_wire++) {
        int p_node = allpatchnode[p_wire];
        bool find_replace = false;
        for (int can_wire = 0; can_wire < allcandidate.size(); can_wire++) {
            int can_node = allcandidate[can_wire];
            int check_equal = euqal_ck(can_node, p_node);
            if (check_equal == EQ_UNSAT) {
                find_replace = true;
                ReplaceNode[p_wire] = allcandidate[can_wire];
                ReplaceCost[p_wire] = allnodevec[can_node].cost;
                break;
            } else if (check_equal == EQ_INV_UNSAT) {
                find_replace = true;
                //constant 0 (id = 0), constant 1 (id = 1)
                if (allcandidate[can_wire] == 0) {
                    ReplaceNode[p_wire] = 1;
                } else if (allcandidate[can_wire] == 1) {
                    ReplaceNode[p_wire] = 0;
                } else {
                    ReplaceNode[p_wire] = allcandidate[can_wire] * (-1);
                }
                ReplaceCost[p_wire] = allnodevec[can_node].cost * (-1);
                break;
            }
        }
        if (!find_replace) {
            ReplaceNode[p_wire] = -1;
        }
    }
}

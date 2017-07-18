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
        cout << "\n\n1 " << tmpstr << endl;
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
                    cout << "Find input: " << allnodevec[outidx].name << endl;
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
                    cout << "Find output: " << allnodevec[outidx].name << endl;
					cout << "outidx: " << outidx << endl;
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
			cout << "gate out " << tmpstr << endl;
            iter = allnodemap.find(tmpstr);
            if (iter == allnodemap.end()) {
				cout << "not in map: " << tmpstr << endl;
                tmpstr = "p_" + tmpstr;
            } else {
				cout << "outidx: " << outidx << endl;
				outidx = iter->second;
                if (!allnodevec[outidx].patch_flag) {
                    //not PIs or POs in patch
                    tmpstr = "p_" + tmpstr;
                }
            }

            iter = allnodemap.find(tmpstr);
            if (iter == allnodemap.end()) {
				cout << "not in map 2: " << tmpstr << endl;
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
                if (tmpstr == ");") break;
				if (tmpstr[tmpstr.size() - 1] == ';') end_in = true;
				spilt_str(tmpstr);
				cout << "gate in " << tmpstr << endl;
                if (tmpstr == "1'b0") {
                    inidx = 0;
                } else if (tmpstr == "1'b1") {
                    inidx = 1;
                } else {
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
						cout << "not in map: " << tmpstr << endl;
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
                cout << "tmpstr: " << tmpstr << " out:" << allnodevec[outidx].name << " in:" << allnodevec[inidx].name << endl;
				
				if (end_in) break;
            }
        }
    }
    
    file.close();
	allnodevalue.resize(allnodevec.size());

    return true;

}

/*
vector<int> Circuit_t::findRelatedNode(vector<int> relatedPI)
{

}


void Circuit_t::sortcost(vector<int>& array)
{

}
*/

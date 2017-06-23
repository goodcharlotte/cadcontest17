#include <iostream>
#include <fstream>
#include "datatype.h"
using namespace::std;

Circuit_t::Circuit_t()
{
}

bool Circuit_t::readfile(char* fname)
{
    ifstream file(fname);
    if (!file) return false;
    string tmpstr;
    
    allnodevec.push_back(*(new Node_t("1'b0", PORT)));
    allnodevec.push_back(*(new Node_t("1'b1", PORT)));

    while (1) {
        file >> tmpstr;
        if (tmpstr == "input") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ";") break;
                allnodemap[tmpstr] = allnodevec.size();
                pi.push_back(allnodevec.size());
                allnodevec.push_back(*(new Node_t(tmpstr, PORT)));       
            }
        } else if (tmpstr == "output") {
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ";") break;
                allnodemap[tmpstr] = allnodevec.size();
                po.push_back(allnodevec.size());
                allnodevec.push_back(*(new Node_t(tmpstr, PORT)));       
            }
        } else if (tmpstr == "wire") {
            while (file >> tmpstr) {
                if (tmpstr == ";") break;
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
            int outidx;
            file >> tmpstr >> tmpstr;
            iter = allnodemap.find(tmpstr);
            if (iter == allnodemap.end()) {
                outidx = allnodemap[tmpstr] = allnodevec.size();
                allnodevec.push_back(*(new Node_t(tmpstr, tp)));        
            } else {
                outidx = iter->second;
                allnodevec[outidx].setType(tp);
            }
            // gate input
            int inidx;
            while (file >> tmpstr) {
                if (tmpstr == ",") continue;
                if (tmpstr == ");") break;
                if (tmpstr == "1'b0") {
                    inidx = 0;
                } else if (tmpstr == "1'b1") {
                    inidx = 1;
                } else {
                    iter = allnodemap.find(tmpstr);
                    if (iter == allnodemap.end()) {
                        inidx = allnodemap[tmpstr] = allnodevec.size();
                        allnodevec.push_back(*(new Node_t(tmpstr, PORT)));       
                        if (tmpstr[0] == 't') target.push_back(inidx);
                    } else {
                        inidx = iter->second;
                    }
                }
                allnodevec[outidx].addinput(inidx);
                allnodevec[inidx].addoutput(outidx);
            }
        }
    }
    
    file.close();
	allnodevalue.resize(allnodevec.size());


    return true;
}

bool Circuit_t::writefile(char* fname, vector<int> candidate)
{   
    ofstream file(fname);
    if (!file) return false;
    
    file << "module top(" ;
    for (int i = 0; i < pi.size(); i++) {
        file <<  allnodevec[pi[i]].getName() << ", ";
    }
    for (int i = 0; i < po.size(); i++) {
        if (i != po.size() - 1)
            file <<  allnodevec[po[i]].getName() << ", ";
        else
            file <<  allnodevec[po[i]].getName() << ");" << endl;
    }
    
    file << "input "; 
    for (int i = 0; i < pi.size(); i++) {
        if (i != pi.size() - 1)
            file <<  allnodevec[pi[i]].getName() << ", ";
        else
            file <<  allnodevec[pi[i]].getName() << ";" << endl;
    }
    file << "output "; 
    for (int i = 0; i < po.size(); i++) {
        if (i != po.size() - 1)
            file <<  allnodevec[po[i]].getName() << ", ";
        else
            file <<  allnodevec[po[i]].getName() << ";" << endl;
    }
    file << "wire ";
    for (int i = pi.size()+po.size()+2; i < allnodevec.size(); i++) {
        if (i != allnodevec.size() - 1)
            file <<  allnodevec[i].getName() << ", ";
        else
            file <<  allnodevec[i].getName() << ";" << endl;
    }

    for (int i = pi.size()+2; i < allnodevec.size(); i++) {
        Node_t node = allnodevec[i];
        if (node.in.size() == 0) continue; // t_0, t_1, ...
        if (node.getType() == BUF) {
            file << "buf ( " << node.getName(); 
        } else if (node.getType() == NOT) {
            file << "not ( " << node.getName(); 
        } else if (node.getType() == AND) {
            file << "and ( " << node.getName(); 
        } else if (node.getType() == NAND) {
            file << "nand ( " << node.getName(); 
        } else if (node.getType() == OR) {
            file << "or ( " << node.getName(); 
        } else if (node.getType() == NOR) {
            file << "nor ( " << node.getName(); 
        } else if (node.getType() == XOR) {
            file << "xor ( " << node.getName(); 
        } else if (node.getType() == NXOR) {
            file << "xnor ( " << node.getName(); 
        }
        for (int j=0; j<node.in.size(); j++) {
            file << " ," << allnodevec[node.in[j]].getName();
        }
        file << " );" << endl;

    }
    
    if (candidate.size() != 0) {
        file << "patch p0 (";
        for (int i = 0; i < target.size(); i++) {
            if (i == 0) {
                file << "." << allnodevec[target[i]].getName() << "(" 
                << allnodevec[target[i]].getName() << ")";
            } else {
                file << ", ." << allnodevec[target[i]].getName() << "("
                << allnodevec[target[i]].getName() << ")";
            }
        }
        for (int i = 0; i < candidate.size(); i++) {
            file << ", ." << allnodevec[candidate[i]].getName() << "(" 
                 << allnodevec[candidate[i]].getName() 
                 << ")";
        }
        file << ");" << endl;
    }
	
    file << "endmodule" << endl;
    file.close();
    return true;
}

void Circuit_t::print()
{
    for (int i = 0; i < allnodevec.size(); i++) {
        allnodevec[i].print(allnodevec);
    }
    //for (int i = 0; i < target.size(); i++)
    //    allnodevec[target[i]].print(allnodevec);
}

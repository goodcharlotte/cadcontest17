#include <iostream>
#include <fstream>
#include <queue>
#include "datatype.h"
#include<climits>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_alg.h> 
#include <LEDA/graph/templates/max_flow.h>
#include <LEDA/graph/max_flow.h>

using namespace::std;
using leda::graph;
using leda::edge;
using leda::node;
using leda::edge_array;
using leda::list;


Circuit_t::Circuit_t()
{
}

int Circuit_t::minCut(vector<int>& allcutnode)
{
    graph G;
    vector<node> ledanodevec;
    vector<edge> ledaedgevec;
    vector<int> costvec;
    vector<int> targetIdx;
    map<node, int> nodemap;
    map<node, int>::iterator niter;

    // The first and second element in allnodevec are const
    for (int i = 2; i < allnodevec.size(); i++) {
        ledanodevec.push_back(G.new_node());
        if (allnodevec[i].name[0] == 't') targetIdx.push_back(i);
        nodemap[ledanodevec[ledanodevec.size()-1]] = i;
    }
    //cout << "target " << targetIdx.size() << endl;
    for (int i = 2; i < allnodevec.size(); i++) {
        Node_t node = allnodevec[i];
        
        int ncost = node.cost > 0 ? node.cost : -node.cost;
        //cout << node.name << " " << ncost << " " << node.out.size() << endl;
        if (node.out.size() == 0) {
        } else if (node.out.size() == 1) {
            ledaedgevec.push_back(G.new_edge(ledanodevec[i-2], ledanodevec[node.out[0]-2]));
            costvec.push_back(ncost);
        } else {
            ledanodevec.push_back(G.new_node());
            int nodeidx = ledanodevec.size()-1;
            ledaedgevec.push_back(G.new_edge(ledanodevec[i-2], ledanodevec[nodeidx]));
            costvec.push_back(ncost);
            for (int j = 0; j < node.out.size(); j++) {
                ledaedgevec.push_back(G.new_edge(ledanodevec[nodeidx],ledanodevec[node.out[j]-2]));
                costvec.push_back(INF);
            }
        }
    }

    ledanodevec.push_back(G.new_node());
    int sidx = ledanodevec.size()-1;
    for (int i = 0; i < pi.size(); i++) {
        ledaedgevec.push_back(G.new_edge(ledanodevec[sidx], ledanodevec[i]));
        costvec.push_back(INF);
    }
    int tidx;
    if (targetIdx.size() == 1) {
        tidx = targetIdx[0]-2;
    } else {
        ledanodevec.push_back(G.new_node());
        tidx = ledanodevec.size()-1;
        for (int i = 0; i < targetIdx.size(); i++) {
            ledaedgevec.push_back(G.new_edge(ledanodevec[targetIdx[i]-2], ledanodevec[tidx]));
            costvec.push_back(INF);
        }
    }
    
    edge_array<int> weight(G);
    for (int i = 0; i < ledaedgevec.size(); i++) {
        weight[ledaedgevec[i]] = costvec[i];
        //G.print_edge(ledaedgevec[i]);
        //cout << " " << weight[ledaedgevec[i]] << endl;
    }
    
    //cout << "-----------------------" << endl;
    //G.print();
    //cout << "-----------------------" << endl;
    edge_array<int> earray;
    list<node> cut;
    int cut_value = MAX_FLOW_T(G, ledanodevec[sidx], ledanodevec[tidx], weight, earray, cut);
    cout << "min cut " << cut_value << endl;
    //cout << "cut: "; node v; forall(v, cut) G.print_node(v);
    //cout << endl;
    node v;
    forall(v, cut) {
        niter = nodemap.find(v);
        if (niter != nodemap.end()) {
            //cout << niter->second << endl;
            //cout << allnodevec[niter->second].name << endl;
            allcutnode.push_back(niter->second);
        } else {
            // dummy node
        }
    }
    //cout << "edge sz: " << earray.size() << endl;
    edge eg;
    for (int i=0; i<earray.size(); i++) {
        //eg = earray[i];
        //G.print_edge(earray);
    }
    /*
    int cut_value = MIN_CUT(G, weight, cut);
    
    */ 
    return cut_value;
}

void Circuit_t::printstatus()
{
    cout << "---- Circuit Status ----" << endl;
    int pisz = 0;
    int nodesz = 0;
    int c1, c2, c3, c4;
    c1 = c2 = c3 = c4 = 0;
    for (int i = 0; i < pi.size(); i++) {
        if (allnodevec[pi[i]].out.size() != 0) pisz++;
    }
    for (int i = 0; i < allnodevec.size(); i++) {
        if (allnodevec[i].out.size() != 0 && allnodevec[i].out.size() != 0) c1++;
        else if (allnodevec[i].out.size() == 0 && allnodevec[i].out.size() != 0) c2++;
        else if (allnodevec[i].out.size() != 0 && allnodevec[i].out.size() == 0) c3++;
        else if (allnodevec[i].out.size() == 0 && allnodevec[i].out.size() == 0) c4++;
    }
    cout << "PI size: " << pisz << endl;
    //cout << c1 << " " << c2 << " " << c3 << " " << c4 << endl;
    cout << "node size: " << c1 << endl;
}

vector<int> Circuit_t::findRelatedPI(vector<int> relatedPO)
{
    queue<int> nodeque;
    int node; 
        
    //cout << "removeredundant:" << po.size() << " " << relatedPO.size() << endl;
    vector<bool> visit_flag(allnodevec.size(), false);
    for (int i = 0; i < relatedPO.size(); i++) {
        //cout << allnodevec[relatedPO[i]].name << endl;
        nodeque.push(relatedPO[i]);
    }

    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].in.size(); i++) {
            if (visit_flag[allnodevec[node].in[i]] == false) 
                nodeque.push(allnodevec[node].in[i]);
        }
        visit_flag[node] = true;
        nodeque.pop();
    }
   /* 
    for (int i = 0; i < visit_flag.size(); i++) {
        if (visit_flag[i] == false) { 
            //cout << "remove " <<  allnodevec[i].name << endl;
            allnodevec[i].in.clear();
            allnodevec[i].out.clear();
        }
    }
    */
    vector<int> relatedPI;
    for (int i = 2; i < 2 + pi.size(); i++) {
        if (visit_flag[i] == true)
            relatedPI.push_back(i);
    }
    return relatedPI;
}


vector<int> Circuit_t::findRelatedPO()
{
    queue<int> nodeque;
    vector<bool> visit_flag(allnodevec.size(), false);
    for (int i = 0; i < target.size(); i++) {
        nodeque.push(target[i]);    
    }
   
    int node; 
    vector<int> relatedPO;
    
    while (nodeque.size() != 0) {
        node = nodeque.front();
        bool find = false;
        if (allnodevec[node].out.size() == 0) {
            for (int i = 0; i < relatedPO.size(); i++) {
                if (relatedPO[i] == node) {
                    find = true;
                    break;
                }
            }
            if (!find) relatedPO.push_back(node);
        } else {
            for (int i = 0; i < allnodevec[node].out.size(); i++) {
                if (visit_flag[allnodevec[node].out[i]] == false) 
                    nodeque.push(allnodevec[node].out[i]);
            }
        }
        visit_flag[node] = true;
        nodeque.pop();    
    }
    return relatedPO;
}

void Circuit_t::removeredundant(vector<int> relatedPO)
{
    queue<int> nodeque;
    int node; 
        
    //cout << "removeredundant:" << po.size() << " " << relatedPO.size() << endl;
    vector<bool> visit_flag(allnodevec.size(), false);
    for (int i = 0; i < relatedPO.size(); i++) {
        //cout << allnodevec[relatedPO[i]].name << endl;
        nodeque.push(relatedPO[i]);
    }

    while (nodeque.size() != 0) {
        node = nodeque.front();
        for (int i = 0; i < allnodevec[node].in.size(); i++) {
            if (visit_flag[allnodevec[node].in[i]] == false) 
                nodeque.push(allnodevec[node].in[i]);
        }
        visit_flag[node] = true;
        nodeque.pop();
    }
    
    for (int i = 0; i < visit_flag.size(); i++) {
        if (visit_flag[i] == false) { 
            //cout << "remove " <<  allnodevec[i].name << endl;
            allnodevec[i].in.clear();
            allnodevec[i].out.clear();
        }
    }
}

int Circuit_t::removebuffer()
{
    int removecnt = 0;
    for (int i = 0; i < allnodevec.size(); i++) {
        Node_t * node = &allnodevec[i];
        if (node->type == BUF && node->out.size() > 0 && node->in.size() > 0) {
            Node_t *input = &allnodevec[node->in[0]];
            for (int j = 0; j < input->out.size(); j++) {
                if (allnodevec[input->out[j]].name == node->name) {
                    input->out.erase(input->out.begin()+j);
                    break;
                }
            }
            for (int j = 0; j < node->out.size(); j++) {
                input->out.push_back(node->out[j]);
                Node_t *output = &allnodevec[node->out[j]];
                for (int k = 0; k < output->in.size(); k++) {
                    if (allnodevec[output->in[k]].name == node->name) {
                        output->in.erase(output->in.begin()+k);
                        break;
                    }
                }
                output->in.push_back(node->in[0]);
            }
            if (node->cost < input->cost && input->type != PORT){
                input->name = node->name;    
            }
            node->in.clear();
            node->out.clear();
            removecnt++;
        }
    }
    return removecnt;
}

bool Circuit_t::readcost(char* fname)
{
    ifstream file(fname);
    string nodename;
    int nodecost;
    int outidx;
    while (file >> nodename >> nodecost) {
        iter = allnodemap.find(nodename);
        if (iter == allnodemap.end()) {
            cout << "read cost error" << endl;
            return false;
        } else {
            outidx = iter->second;
            allnodevec[outidx].cost = nodecost;
        }
    }
    return true;
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
                allnodevec[outidx].type = tp;
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
                allnodevec[outidx].in.push_back(inidx);
                allnodevec[inidx].out.push_back(outidx);
            }
        }
    }
    
    file.close();
	allnodevalue.resize(allnodevec.size());

    return true;
}

void Circuit_t::init_simp(string cktname)
{
    string fname0 = "tmp0_";
    fname0 += cktname;
    fname0 += ".v";
    
    string fname1 = "tmp1_";
    fname1 += cktname;
    fname1 += ".v";

    ofstream w_file;
    w_file.open("simp.script",ios::out);
    w_file << "read_verilog " << fname0  << endl;
    //w_file << "resyn2" << endl;
    w_file << "resyn2rs" << endl;
    w_file << "compress2rs" << endl;
    //w_file << "resyn2rs" << endl;
    //w_file << "compress2rs" << endl;
    //w_file << "resyn2rs" << endl;
    //w_file << "compress2rs" << endl;
    w_file << "read_library mcnc_2input.genlib" << endl;
    w_file << "map" << endl;
    w_file << "write_verilog " << fname1 << endl;
    w_file.close();

    system("./abc -f simp.script");

}

bool Circuit_t::write_verilog(string cktname)
{
    string fname = "tmp0_";
    fname += cktname;
    fname += ".v";
    ofstream file(fname.c_str());
    bool first = true;
    if (!file) return false;
    file << "module " <<  cktname << " (" ;
    vector<int> allinput;
    vector<int> alloutput;
    for (int i = 0; i < pi.size(); i++) {
        if (allnodevec[pi[i]].out.size() > 0) {
            if (first) {
                file <<  allnodevec[pi[i]].name;
                first = false;
            } else {
                file << ", " << allnodevec[pi[i]].name;
            }
            allinput.push_back(pi[i]);
        }
    }
    if (cktname == "F") {
        for (int i = 0; i < target.size(); i++) {
            file << ", " << allnodevec[target[i]].name;
            allinput.push_back(target[i]);
        }
    }
    for (int i = 0; i < po.size(); i++) {
        if (allnodevec[po[i]].in.size() > 0) {
                file << ", " << allnodevec[po[i]].name;
                alloutput.push_back(po[i]);
        }
    }
    file  << ");" << endl;

    file << "input ";
    for (int i = 0; i<allinput.size(); i++) {
        if (i != allinput.size() - 1)
            file <<  allnodevec[allinput[i]].name << ", ";
        else
            file <<  allnodevec[allinput[i]].name << ";" << endl;
    }

    file << "output ";
    for (int i = 0; i<alloutput.size(); i++) {
        if (i != alloutput.size() - 1)
            file <<  allnodevec[alloutput[i]].name << ", ";
        else
            file <<  allnodevec[alloutput[i]].name << ";" << endl;
    }
    file << "wire ";
    first = true;
    for (int i = pi.size()+po.size()+2; i < allnodevec.size(); i++) {
        Node_t node = allnodevec[i];
        if (node.in.size() == 0) continue; // t_0, t_1, ...
        if (first) {
            file << node.name;
            first = false;
        } else {
            file << ", " << node.name;
        }
    }
    file <<  ";" << endl;

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
            file << " ," << allnodevec[node.in[j]].name;
        }
        file << " );" << endl;

    }
    file << "endmodule" << endl;
    file.close();
    return true;
}

bool Circuit_t::writefile(char* fname, vector<string> candidate, vector<string> patchPI)
{   
    ofstream file(fname);
    if (!file) return false;
    
    file << "module top(" ;
    for (int i = 0; i < pi.size(); i++) {
        file <<  allnodevec[pi[i]].name << ", ";
    }
    for (int i = 0; i < po.size(); i++) {
        if (i != po.size() - 1)
            file <<  allnodevec[po[i]].name << ", ";
        else
            file <<  allnodevec[po[i]].name << ");" << endl;
    }
    
    file << "input "; 
    for (int i = 0; i < pi.size(); i++) {
        if (i != pi.size() - 1)
            file <<  allnodevec[pi[i]].name << ", ";
        else
            file <<  allnodevec[pi[i]].name << ";" << endl;
    }
    file << "output "; 
    for (int i = 0; i < po.size(); i++) {
        if (i != po.size() - 1)
            file <<  allnodevec[po[i]].name << ", ";
        else
            file <<  allnodevec[po[i]].name << ";" << endl;
    }
    file << "wire ";
    for (int i = pi.size()+po.size()+2; i < allnodevec.size(); i++) {
        if (i != allnodevec.size() - 1)
            file <<  allnodevec[i].name << ", ";
        else
            file <<  allnodevec[i].name << ";" << endl;
    }

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
            file << " ," << allnodevec[node.in[j]].name;
        }
        file << " );" << endl;

    }
    
    if (candidate.size() != 0) {
        file << "patch p0 (";
        for (int i = 0; i < target.size(); i++) {
            if (i == 0) {
                file << "." << allnodevec[target[i]].name << "(" 
                << allnodevec[target[i]].name << ")";
            } else {
                file << ", ." << allnodevec[target[i]].name << "("
                << allnodevec[target[i]].name << ")";
            }
        }
        for (int i = 0; i < candidate.size(); i++) {
            file << ", ." << patchPI[i] << "(" 
                 << candidate[i] << ")";
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

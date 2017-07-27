#include <iostream>
#include "datatype.h"
#include <bitset>
#include <math.h>
#include <string>

using namespace std;


int main(int argc, char * argv[])
{
    if (argc != 6) {
        cout << "Usage: ./rpgen <F.v> <G.v> <weight.txt> <patch.v> <out.v> " << endl;
        return 0;
    } 

    Circuit_t cktf, cktg;
    if (!cktf.readfile(argv[1]) || !cktg.readfile(argv[2]) || !cktf.readcost(argv[3])) {
        cout << "Read file error" << endl;
        return 0; 
    }

    //====================================
    //  Preprocessing
    //====================================
    vector<int> relatedPO;
    vector<int> relatedPI;
    //cktf.printstatus();
    relatedPO = cktf.findRelatedPO();
    relatedPI = cktg.findRelatedPI(relatedPO);
    cktf.removeredundant(relatedPO);	
    cktg.removeredundant(relatedPO);	
    cktf.write_verilog("F");
    cktg.write_verilog("G");
    cktf.init_simp("F");
    cktg.init_simp("G");
    
    //====================================
    //  Construct patch
    //====================================
    constructPatch("tmp1_F.v", "tmp1_G.v");
    //patchckt = cktf + patch
    
    //====================================
    //  Find equivalent wire
    //====================================
    Circuit_t patchckt;
    vector<int> allpatchnode;
    vector<Node_t> PatchNode;
    patchckt.readfile(argv[1]);
    patchckt.readpatch("patch.v");
    patchckt.readcost(argv[3]);
    patchckt.removeredundant(relatedPO);
    vector<int> allcandidate;
    patchckt.findRelatedNode(relatedPO, allpatchnode, allcandidate);
    patchckt.sortcost(allcandidate, 0, allcandidate.size() - 1);
    //ReplaceNode: (UNSAT & INV_UNSAT) id, (No replaced node) -1
    //ReplaceCost: (UNSAT) cost, (INV_UNSAT) cost * (-1), (No replaced node) INF
    patchckt.findReplaceCost(allcandidate, allpatchnode, PatchNode);
    //====================================
    //  Copy cost info. to cktp
    //====================================
    Circuit_t cktp;
    cktp.readfile("patch.v");
    cktp.findReplaceNode(PatchNode);
    //====================================
    //  Find min-cut 
    //====================================
    cktp.minCut();
    //====================================
    //  Write final result 
    //====================================
    Circuit_t finalckt;
    finalckt.readfile(argv[1]);
    finalckt.writefile(argv[5], relatedPI);

    return 0;
}

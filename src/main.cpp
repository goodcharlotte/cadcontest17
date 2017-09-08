#include <iostream>
#include "datatype.h"
#include <bitset>
#include <math.h>
#include <string>

using namespace std;
clock_t start_clk;

int main(int argc, char * argv[])
{    
    start_clk = clock();
    bool INTER_FLAG = false;
    bool MINCUT_FLAG = false;
    vector<int> patchPI;
    if (argc != 6) {
        cout << "Usage: ./rpgen <F.v> <G.v> <weight.txt> <patch.v> <out.v> " << endl;
        return 0;
    } 

    Circuit_t cktf, cktg;
    if (!cktf.readfile(argv[1]) || !cktg.readfile(argv[2]) || !cktf.readcost(argv[3])) {
        cout << "Read file error" << endl;
        return 0; 
    }

    if (cktf.target.size() == 1) {
        INTER_FLAG = true;
        MINCUT_FLAG = false;
    } else if (cktf.target.size() > 1) {
        INTER_FLAG = false;
        MINCUT_FLAG = true;
    } else {
        cout << "ERROR! NO TARGET IN CKTF!" << endl;
    }

    //====================================
    //  Preprocessing
    //====================================
    vector<int> relatedPO;
    vector<int> relatedPI;
    vector<string> cktfWireName;
    vector<string> patchPIName;
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
    patchckt.update_allpi();
    vector<int> allcandidate;
    patchckt.findRelatedNode(relatedPI, allpatchnode, allcandidate);
    patchckt.sortcost(allcandidate, 0, allcandidate.size() - 1);

    if (INTER_FLAG == true) {
        cout << "*** INTERPOLATION ***" << endl;
        vector<int> choosebase;
        Circuit_t F_v_ckt;// F.v
        F_v_ckt.readfile(argv[1]);
        choosebase = patchckt.getbaseset(relatedPI, allcandidate, F_v_ckt);
        patchPI = F_v_ckt.inteporlation(argv[4]);
        patchckt.updateName(patchPI, cktfWireName);
        patchckt.updateName(patchPI, patchPIName);
        cout << "*** Interpolation cost sum : " << patchckt.getCostSum(patchPI, patchPI.size()-1) << " ***" << endl;
    } //end interpolation

    if (MINCUT_FLAG == true) {
        cout << "*** MINCUT ***" << endl;
        //ReplaceNode: (UNSAT & INV_UNSAT) id, (No replaced node) -1
        //ReplaceCost: (UNSAT) cost, (INV_UNSAT) cost * (-1), (No replaced node) INF
        patchckt.findReplaceCost(relatedPI, allcandidate, allpatchnode, PatchNode);
        //====================================
        //  Copy cost info. to cktp
        //====================================
        Circuit_t cktp;
        string patchStr = "patch.v";
        char patchName[1024];
        strcpy(patchName, patchStr.c_str());
        cktp.readfile(patchName);
        cktp.findReplaceNode(PatchNode);
        //====================================
        //  Find min-cut 
        //====================================
        vector<int> allcutnode;
        vector<int> patchRelatedPI;
        cktp.minCut(allcutnode);
        patchRelatedPI = cktp.ReplaceNode(allcutnode);
        cktp.write_patch(patchRelatedPI);
        cktp.updatePatchPI(patchRelatedPI, cktfWireName, patchPIName);
    }//end mincut

    //====================================
    //  Write final result (out.v) 
    //====================================
    Circuit_t finalckt;
    finalckt.readfile(argv[1]);
    finalckt.writefile(argv[1], argv[5], cktfWireName, patchPIName);
    //====================================
    //  Remove temporary files 
    //====================================
    system("rm -f *cnf tmp*v resynPatch.script resyntemp_Patch_Fix.script proof.log partition.log simp.script");
    return 0;
}

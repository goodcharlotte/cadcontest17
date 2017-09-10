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
    int first_timeout = MID_TIME_LIMIT;
    int final_timeout = TIME_LIMIT;
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
        MINCUT_FLAG = true;
        INTER_FLAG = true;
    } else if (cktf.target.size() > 1) {
        MINCUT_FLAG = true;
        INTER_FLAG = false;
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

    int mincut_cost = INF;
    int inter_cost = INF;
    string cmdstr;
    char cmdchar[1024];
 
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
        mincut_cost = cktp.getCostSum(patchRelatedPI, patchRelatedPI.size() - 1);
        cout << "*** Mincut cost sum : " << mincut_cost << " ***" << endl;
    }//end mincut

    if (INTER_FLAG == true) {
        patchckt.check_INV_cost(allcandidate);
        cout << "*** INTERPOLATION ***" << endl;
        vector<int> choosebase;
        Circuit_t F_v_ckt;// F.v
        F_v_ckt.readfile(argv[1]);
        choosebase = patchckt.getbaseset(relatedPI, allcandidate, F_v_ckt);
        if (choosebase.size() > 0) {
            patchPI = F_v_ckt.inteporlation(argv[4]);
            inter_cost = patchckt.getCostSum(patchPI, patchPI.size()-1);
        }

        cout << "*** InteIrpolation cost sum : " << inter_cost << " ***" << endl;
        
        if (inter_cost < mincut_cost) {
            patchckt.updateName(patchPI, cktfWireName);
            patchckt.updateName(patchPI, patchPIName);
            //cout << "*** Update Patch I/O ***" << endl;
        }
    } //end interpolation

    if (inter_cost > mincut_cost) {
        cmdstr = "cp mincut_patch.v " + string(argv[4]);
        strcpy(cmdchar, cmdstr.c_str());
        system(cmdchar);
        //cout << "Use patch in mincut: " << cmdstr << endl;
    }


    //====================================
    //  Write final result (out.v) 
    //====================================
    Circuit_t finalckt;
    finalckt.readfile(argv[1]);
    finalckt.writefile(argv[1], argv[5], cktfWireName, patchPIName);
    //====================================
    //  Remove temporary files 
    //====================================
    system("rm -f *cnf tmp*v mincut_patch.v patch2.v temp.v resynPatch.script resynPatch2.script resyntemp_Patch_Fix.script proof.log partition.log simp.script");
    return 0;
}

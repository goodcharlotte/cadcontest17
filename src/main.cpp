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
	
#ifdef DEBUG_MATCH
	//====================================
    // TEST
    //====================================
	
	Circuit_t TEST, TEST2;
	
	
	TEST.readfile("tmp2_F.v");
	TEST2.readfile("tmp2_G.v");
	cout<<"F input num: "<<TEST.pi.size()<<" G input num: "<<TEST2.pi.size()<<endl;
	TEST.readfile2("tmp2_G.v");
	cout<<"F input num: "<<TEST.pi.size()<<endl;
	TEST.Test();
	TEST.write_verilog("WHY");
	
#endif
	
    
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
    //patchckt.findRelatedNode(relatedPO, allpatchnode, allcandidate);
    //patchckt.sortcost(allcandidate, 0, allcandidate.size() - 1);
    //ReplaceNode: (UNSAT & INV_UNSAT) id, (No replaced node) -1
    //ReplaceCost: (UNSAT) cost, (INV_UNSAT) cost * (-1), (No replaced node) INF
#if DEBUG_GETBASE
    vector<int> choosebase;
	Circuit_t F_v_ckt;// F.v
	F_v_ckt.readfile(argv[1]);
    //Circuit_t patchckt_off; // F.v + patch2.v
    //vector<int> allpatchnode_off;
   // patchckt_off.readfile(argv[1]);
    //patchckt_off.readpatch("patch2.v");
    //patchckt_off.update_allpi();
    vector<int> allcandidate_off;
    //patchckt_off.findRelatedNode(relatedPI, allpatchnode_off, allcandidate_off);
    choosebase = patchckt.getbaseset(allcandidate, F_v_ckt/* allpatchnode, patchckt_off, allpatchnode_off*/);
#endif

#if DEBUG_GETSUMSET
    patchckt.sortcost(allcandidate, 0, (allcandidate.size()-1) );
    cout << "All candidate (ignore INF):" << endl;
    for (int i = 0; i < allcandidate.size(); i++) {
        if (patchckt.allnodevec[allcandidate[i]].cost == INF) {
            continue;
        }
        cout << patchckt.allnodevec[allcandidate[i]].name 
            << " (" << patchckt.allnodevec[allcandidate[i]].cost << ")" << endl;
    }
    int MAX_SUM = patchckt.getMaxSum(allcandidate);
    vector< vector<int> > allsumset;
    for (int i = 0; i < MAX_SUM; i++) {
        allsumset = patchckt.getSumSet(allcandidate, i);
        cout << "SUM " << i << ": set " << allsumset.size() << endl;
        clock_t temp_clk = clock();
        double time_sec = double(temp_clk - start_clk)/CLOCKS_PER_SEC;
        if ( time_sec > TIME_LIMIT) {
            cout << "time out:" << time_sec << endl;
            break;
        }
    }
#endif
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
    vector<string> replaceName;
    vector<string> patchPIName;
    cktp.minCut(allcutnode);
    patchRelatedPI = cktp.ReplaceNode(allcutnode);
    cktp.write_patch(patchRelatedPI);
    cktp.updatePatchPI(patchRelatedPI, replaceName, patchPIName);
    //====================================
    //  Write final result 
    //====================================
    Circuit_t finalckt;
    finalckt.readfile(argv[1]);
    finalckt.writefile(argv[1], argv[5], replaceName, patchPIName);

    //====================================
    //  Remove temporary files 
    //====================================
    system("rm -f tmp*v resynPatch.script simp.script");
    return 0;
}

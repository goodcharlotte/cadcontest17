#include <iostream>
#include "datatype.h"
#include <bitset>
#include <math.h>
#include <string>
//#include "core/minisat.h"

using namespace std;


int main(int argc, char * argv[])
{
	Solver S;  
	
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
    Circuit_t patchckt;
    vector<int> allpatchnode;
    vector<int> ReplaceNode;
    patchckt.readfile(argv[1]);
    patchckt.readpatch("patch.v");
    patchckt.readcost(argv[3]);
    allpatchnode = patchckt.removeredundant(relatedPO);
    ReplaceNode.resize(allpatchnode.size());
    
    //====================================
    //  Find replaced wire
    //====================================
    vector<int> allcandidate;
    allcandidate = patchckt.findRelatedNode(relatedPI);
    patchckt.sortcost(allcandidate, 0, allcandidate.size() - 1);
    //ReplaceNode: (UNSAT) id, (INV_UNSAT) id * (-1), (No replaced node) -1
    //ReplaceCost: (UNSAT) cost, (INV_UNSAT) cost * (-1), (No replaced node) INF
    patchckt.findReplaceCost(ReplaceNode, allcandidate, allpatchnode);

    //====================================
    //  Derive signature
    //====================================
    /*
	int newpi_size = cktf.newpi.size();
	vector<int> graydiff = gene_graycode(newpi_size - OFFSET_BIT);
	int target_size = (cktf.target).size();
	int target_leng = (int)pow(2, target_size);
	int xor_f_g[target_leng];
	vector< vector<string> > signature;
	

	for(int i = 0; i < graydiff.size(); i++) {
		cktg.simulation(graydiff[i]);
		for(int j = 0; j < target_leng; j++) {
			cktf.input_target_pattern(j);
			if (j == 0) {
				cktf.simulation(graydiff[i]);
			} else {
				cktf.simulation(GRAY_NO_CHAGNE);
			}
			#if EN_DEBUG_SIM
			cout<< "Iput pattern--- "<<endl;
			cout<< "target ="<<" "<< j <<endl;
			cktf.print_pi(); //old pi
			cout<< "*****cktf output*****"<<endl;
			cktf.print_po();
			cout<< "*****cktg output*****" <<endl;
			cktg.print_po();
			cout<< "*****cktf xor cktg *****" <<endl;
			#endif
			xor_f_g[j] = 0;
			for(int k = 0; k < cktf.po_value.size(); k++) { 
				debug_print( bitset<SHOW_BIT_SET>(cktf.po_value[k] ^ cktg.po_value[k]) << endl);
				xor_f_g[j] = xor_f_g[j] | (cktf.po_value[k] ^ cktg.po_value[k]);
				//cout<<"xor_f_g["<<j<<"] "<< bitset<SHOW_BIT_SET>(xor_f_g[j])<<endl;
			}
			
			
		}
		#if EN_DEBUG_SIM
		cout<< "*****xor_f_g*****" <<endl;	
		for (int i = 0; i < target_leng; i++) {
			cout<<"target = "<< i <<" , "<<bitset<SHOW_BIT_SET>(xor_f_g[i]) <<endl;
		}
		#endif
		find_signature(signature, target_size, newpi_size, xor_f_g);
		
	}

	//print_signature(signature);
    vector<string> piName;
    vector<string> targetName;

    piName.resize(cktf.newpi.size());
    for(int i = 0; i < cktf.newpi.size(); i++) {
        int node_id;
        node_id = cktf.newpi[i];
        piName[i] = cktf.allnodevec[node_id].name;
    }

    targetName.resize(cktf.target.size());
    for(int i = 0; i < cktf.target.size(); i++) {
        int node_id;
        node_id = cktf.target[i];
        targetName[i] = cktf.allnodevec[node_id].name;
    }
    
    write_pla(signature, piName, targetName, graydiff);
    ofstream w_file;
    w_file.open("map.script",ios::out);
    w_file << "read patch.pla"  << endl;
    w_file << "read_library mcnc.genlib" << endl;
    w_file << "map" << endl;
    w_file << "write_verilog " << argv[4] << endl;
    w_file.close();

    system("./abc -f map.script");
    */
    Circuit_t finalckt;
    finalckt.readfile(argv[1]);
    finalckt.writefile(argv[5], relatedPI);


    

    return 0;
	
}

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
    if (!cktf.readfile(argv[1]) || !cktg.readfile(argv[2])) {
        cout << "Read file error" << endl;
        return 0; 
    }
	
	cktf.NewPI(cktg);
	//cktf.print();
	

	int pi_size = cktf.get_pi_size();
	vector<int> graydiff = generate_graycode(pi_size - OFFSET_BIT);
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
			cktf.print_pi();
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
		
		find_signature(signature, target_size, pi_size, xor_f_g);
		
	}

	print_signature(signature);
	
    vector<string> piName;
    vector<string> targetName;

    piName.resize(cktf.get_pi_size());
    for(int i = 0; i < cktf.get_pi_size(); i++) {
        int node_id;
        node_id = cktf.pi[i];
        piName[i] = cktf.allnodevec[node_id].getName();
    }

    targetName.resize(cktf.target.size());
    for(int i = 0; i < cktf.target.size(); i++) {
        int node_id;
        node_id = cktf.target[i];
        targetName[i] = cktf.allnodevec[node_id].getName();
    }
    
    write_pla(signature, piName, targetName, graydiff);

    cktf.writefile(argv[5], cktf.pi);
  
    return 0;
}

#include <iostream>
#include "datatype.h"
#include <bitset>
#include <math.h>
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
	
	//cktf.print();
	
	if (cktf.get_pi_size() != cktg.get_pi_size()) {
		cout << "Error: PI number different!" << endl;
		return 0;
	} else {
		vector<int> graydiff = generate_graycode(cktf.get_pi_size() - OFFSET_BIT);
		int total_target_value_number = (int)pow(2, ((cktf.target).size()));
		for(int i = 0; i < graydiff.size(); i++) {
			cktg.simulation(graydiff[i]);
			for(int j = 0; j < total_target_value_number; j++) {
				cktf.input_target_pattern(j);
				if (j == 0) {
					cktf.simulation(graydiff[i]);
				} else {
					cktf.simulation(GRAY_NO_CHAGNE);
				}
				/*
				#if EN_DEBUG_SIM
				cout<< "Iput pattern--- "<<endl;
				cout<< "target ="<<" "<< j <<endl;
				cktf.print_pi();
				cout<< "*****cktf output*****"<<endl;
				cktf.print_po();
				cout<< "*****cktg output*****" <<endl;
				cktg.print_po();
				cout<< "*****cktf xor cktg *****" <<endl;
				for(int i = 0; i < cktf.po_value.size(); i++) { 
					cout<< bitset<32>(cktf.po_value[i] ^ cktg.po_value[i]) <<endl;
				}
				cout<< "------------"<<endl;
				#endif
                */
			}	
		}
		
	}
		
	
   // cktg.writefile(argv[5]);
    return 0;
}

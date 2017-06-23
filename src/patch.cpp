#include <iostream>
#include <fstream>
#include <queue>
#include <string.h> 
#include <cstdlib>
#include <bitset>
#include <math.h>
#include "datatype.h"
#include "stdio.h"
using namespace::std;

#define INT_TYPE int


void write_pla(const vector< vector<string> > &sig, vector<string> &piName, vector<string> &targetName,vector<int> graydiff)
{
    string gray_pattern;
    string offset_pattern;
    INT_TYPE gray_i;
    INT_TYPE offset_i;
    INT_TYPE sig_i;
    int diff_bit;
    int input_num;
    int target_num;
    int io_i;
    int node_id;

    input_num = piName.size();
    target_num = targetName.size();


    ofstream w_file;
    w_file.open("patch.pla", ios::out);

    w_file << ".i " << input_num << endl;
    w_file << ".ilb";

    for(io_i = (input_num - 1); io_i >= 0; io_i--) {
        w_file << " " << piName[io_i];
    }

    w_file << endl;
    w_file << ".o " << target_num;
    w_file << endl; 
    w_file << ".ob";

    for(io_i = (target_num - 1); io_i >= 0; io_i--) {
        w_file << " " << targetName[io_i];
    }

    w_file << endl;
    w_file << ".p " << sig.size() << endl;
 
    //initial string
    for (gray_i = 0; gray_i < (input_num - OFFSET_BIT); gray_i++) {
        if (gray_i == 0) {
            gray_pattern = '0';
        } else {
            gray_pattern += '0';
        }
    }   
    
    if (input_num > OFFSET_BIT) {
        sig_i = 0;
        for (gray_i = 0; gray_i < graydiff.size(); gray_i++) {
            //reverse the bit
            diff_bit = gray_pattern.size() - 1 - graydiff[gray_i];
            if (diff_bit == GRAY_INIT) {
                gray_pattern = gray_pattern;
            } else {
                if (gray_pattern[diff_bit] == '0') {
                    gray_pattern[diff_bit] = '1';
                } else {
                    gray_pattern[diff_bit] = '0';
                }
            }

            for (offset_i = 0; offset_i < SHOW_BIT_SET; offset_i++) {
                bitset <OFFSET_BIT> bit_num(offset_i);

                w_file << gray_pattern  << bit_num << " ";

                if (sig[sig_i].size() > 0) {
                    w_file << sig[sig_i][0] << endl;
                }

                sig_i++;
                if (sig_i == sig.size()) {
                    break;
                }
            }
        }
    } else {
        //input_num < OFFSET_BIT
        for (sig_i = 0; sig_i < sig.size(); sig_i++) {
            
            bitset <OFFSET_BIT> bit_num(sig_i);
            string bit_str;
            bit_str = bit_num.to_string<char,std::string::traits_type,std::string::allocator_type>();

            w_file << gray_pattern << bit_str.substr((OFFSET_BIT - input_num),(OFFSET_BIT - 1)) << " ";


            if (sig[sig_i].size() > 0) {
                    w_file << sig[sig_i][0] << endl;
            }
        }
    }

    w_file.close();
}

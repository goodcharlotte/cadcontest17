#include "datatype.h"

#define ALL_ONE -1
#define MAX_INT_SIZE 32
#define ONSET 1
#define OFFSET 0

vector<string> targetName;
vector<string> F_piName;
vector<string> F_poName;
vector<string> G_piName;
vector<string> G_poName;

void parse(string &cktF_name, string &cktG_name);
void constructMux(string &outputName);
void constructMiter(string &outputName);
void constructTop(string &outputName, int mode);
void constructPatch(string cktF_name, string cktG_name);


void constructPatch(string cktF_name, string cktG_name)
{
    int mode = ONSET;
    //Construct Patch_onset
    string patchName = "patch.v";
    string tempName = "tmp_" + patchName;
    string systemCmd;

    parse(cktF_name, cktG_name);

    constructMux(tempName);
    
    //make sure modelNames are module F & module G
    systemCmd = "cat " + cktF_name + " " + cktG_name + " >> " + tempName;
    system(systemCmd.c_str());

    constructMiter(tempName);
    constructTop(tempName, mode);

    //optimization
    ofstream w_file;
    w_file.open("resynPatch.script",ios::out);
    w_file << "read_verilog " << tempName  << endl;
    //w_file << "resyn2" << endl;
    //w_file << "resyn2rs" << endl;
    //w_file << "compress2rs" << endl;
    //w_file << "resyn2rs" << endl;
    //w_file << "compress2rs" << endl;
    w_file << "resyn2rs" << endl;
    w_file << "compress2rs" << endl;
    w_file << "read_library mcnc.genlib" << endl;
    w_file << "map" << endl;
    w_file << "write_verilog " << patchName << endl;
    w_file.close();

    system("./abc -f resynPatch.script");

    //Construct Patch_offset
    mode = OFFSET;
    string patchName2 = "patch2.v";
    string tempName2 = "tmp_" + patchName2;

    constructMux(tempName2);
    
    //make sure modelNames are module F & module G
    systemCmd = "cat " + cktF_name + " " + cktG_name + " >> " + tempName2;
    system(systemCmd.c_str());

    constructMiter(tempName2);
    constructTop(tempName2, mode);

    //optimization
    ofstream w_file2;
    w_file2.open("resynPatch2.script",ios::out);
    w_file2 << "read_verilog " << tempName2  << endl;
    //w_file2 << "resyn2" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    w_file2 << "resyn2rs" << endl;
    w_file2 << "compress2rs" << endl;
    w_file2 << "read_library mcnc.genlib" << endl;
    w_file2 << "map" << endl;
    w_file2 << "write_verilog " << patchName2 << endl;
    w_file2.close();

    system("./abc -f resynPatch2.script");
}

void parse(string &cktF_name, string &cktG_name)
{
    targetName.clear();
    F_piName.clear();
    G_piName.clear();
    F_poName.clear();
	G_poName.clear();

    ifstream file(cktF_name.c_str(), ios::in);
    string tmpstr;
    bool fin;
    bool find_t;
    fin = false;
    find_t = false;
    while((file >> tmpstr) && (!fin)) {
        if (tmpstr == "input") {
            while (file >> tmpstr) {
                if (tmpstr == ",") {
                    continue;
                }
                if (tmpstr == ";") {
                    if (!find_t) {
                        cout << "ERROR! Target wires should be in PI.\n\n";
                    }
                    break;
                }
                if (tmpstr[0] == 't') {
                    find_t = true;
                    targetName.push_back(tmpstr);
                } else {
                    F_piName.push_back(tmpstr);
                }
            }
        } else if (tmpstr == "output") {
            while (file >> tmpstr) {
                if (tmpstr == ",") {
                    continue;
                }
                if (tmpstr == ";") {
                    fin = true;
                    break;
                }
                F_poName.push_back(tmpstr);
            }
        }
    }
    file.close();
    
    file.open(cktG_name.c_str());
    fin = false;
    while((file >> tmpstr) && (!fin)) {
        if (tmpstr == "input") {
            while (file >> tmpstr) {
                if (tmpstr == ",") {
                    continue;
                }
                if (tmpstr == ";") {
                
                    break;
                }
                G_piName.push_back(tmpstr);
            }
        } else if (tmpstr == "output") {
            while (file >> tmpstr) {
                if (tmpstr == ",") {
                    continue;
                }
                if (tmpstr == ";") {
                    fin = true;
                    break;
                }
                G_poName.push_back(tmpstr);
            }
        }
    }
    file.close();
}

void constructMux(string &outputName)
{
    ofstream file(outputName.c_str(), ios::out);

    ////////////////////
    //       mux      //
    ////////////////////
    
    file << "module mux ( ";
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i] << " , ";
    }
    for (int i = 0; i < targetName.size(); i++) {
        file << "a_" << i << " , b_" << i << " , ";
    }
    file << "sel );\n";

    file << "input ";
    for (int i = 0; i < targetName.size(); i++) {
        file << "a_" << i << " , b_" << i << " , ";
    }
    file << "sel;\n";

    file << "output ";
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i];
        if ( i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << ";\n";

    file << "wire inv_sel, ";
    for (int i = 0; i < (targetName.size() * 2); i++) {
        file << "n" << i;
        if (i < ((targetName.size() * 2) - 1)) {
            file << " , ";
        }
    }
    file << ";\n";

    file << "not ( inv_sel, sel );\n";
    for (int i = 0; i < targetName.size(); i++) {
        file << "and ( n" << i << " , a_" << i << " , sel );\n";
    }

    for (int i = 0; i < targetName.size(); i++) {
        file << "and ( n" << (targetName.size() + i) << " , b_" << i << " , inv_sel );\n";
    }

    for (int i = 0; i < targetName.size(); i++) {
        file << "or ( " << targetName[i] << " , n" << i << " , n" << (targetName.size() + i) << " );\n";
    }

    file << "endmodule\n\n";
    file.close();
}

void constructMiter(string &outputName)
{
    ////////////////////
    //     miter      //
    ////////////////////
    
    ofstream file(outputName.c_str(), ios::app);
    file << "\nmodule miter ( miterOutput, " ;
    for (int i = 0; i < G_piName.size(); i++) {
        file << G_piName[i] << " , ";
    }
    for (int i = 0; i < G_poName.size(); i++) {
        file << "g_" << G_poName[i] << " , ";
    }
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i];
        if (i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\n";

    file << "input ";
    for (int i = 0; i < G_piName.size(); i++) {
        file << G_piName[i] << " , ";
    }

    for (int i = 0; i < G_poName.size(); i++) {
        file << "g_" << G_poName[i] << " , ";
    }
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i];
        if (i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";

    file << "output miterOutput;\n";

    file << "wire ";
    for (int i = 0; i < F_poName.size(); i++) {
        file << "f_" << F_poName[i] << " , ";
    }
    for (int i = 0; i < F_poName.size(); i++) {
        file << "n" << i;
        if (i < (F_poName.size() - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";

    for (int i = 0; i < G_poName.size(); i++) {
        file << "xor ( n" << i << " , f_" << F_poName[i] << " , g_" << G_poName[i] << " );\n";
    }

    file << "or ( miterOutput , ";
    for (int i = 0; i < F_poName.size(); i++) {
        file << "n" << i;
        if (i < (F_poName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\n";

    file << "F f0( ";
    for (int i = 0; i < F_poName.size(); i++) {
        file << "." << F_poName[i] << "(f_" << F_poName[i] << "), ";
    }
     for (int i = 0; i < F_piName.size(); i++) {
        file << "." << F_piName[i] << "(" << F_piName[i] << "), ";
    }
    for (int i = 0; i < targetName.size(); i++) {
        file << "." << targetName[i] << "(" << targetName[i] << ")";
        if (i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\nendmodule\n\n";
    file.close();
}

void constructTop(string &outputName, int mode)
{
    ofstream file(outputName.c_str(), ios::app);
    file << "module patch ( ";
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i] << " , ";
    }
    for (int i = 0; i < G_piName.size(); i++) {
        file << G_piName[i];
        if (i < G_piName.size() - 1) {
            file << " , ";
        }
    }
    file << ");\n";

    file << "input ";
    for (int i = 0; i < G_piName.size(); i++) {
        file << G_piName[i];
        if (i < (G_piName.size() - 1)) {
            file << " , ";
        }
    }
    file << ";\n";
    file << "output ";
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i];
        if (i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";
    
    //[ miterOut(2^n-1) ] + muxConnect(n*(2^n-2))
    //[ n0 ~ n(2^n-2)]
    int totalWire = (pow(2, targetName.size()) - 1) + (targetName.size() * (pow(2, targetName.size()) - 2 ));
    file << "wire ";
    for (int i = 0; i < G_poName.size(); i++) {
        file << "g_" << G_poName[i] << " , ";
    } 
    for (int i = 0; i < totalWire; i++) {
        file << "n" << i;
        if (i < (totalWire - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";
    file << "G g0( ";
    for (int i = 0; i < G_poName.size(); i++) {
        file << "." << G_poName[i] << "(g_" << G_poName[i] << "), ";
    }
    for (int i = 0; i < (G_piName.size()); i++) {
        file << "." << G_piName[i] << "(" << G_piName[i] << ")";
        if (i < (G_piName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\n";
    //miter: w[0]~ w[(2^n-2)]
    for (int i = 0; i < (pow(2, targetName.size()) - 1); i++) {
        bitset<MAX_INT_SIZE> bit_str(i);
        string targetValue = bit_str.to_string();
        file << "miter p" << i << " ( .miterOutput(n" << i << "), ";
        for (int j = 0; j < (G_piName.size()); j++) {
            file << "." << G_piName[j] << "(" << G_piName[j] << "), ";
        }
        for (int i = 0; i < G_poName.size(); i++) {
            file << "." << "g_" << G_poName[i] << "(g_" << G_poName[i] << ") , ";
        }
        if (mode == ONSET) {
            for (int j = 0; j < targetName.size(); j++) {
                file << "." << targetName[j] << "(1'b" << targetValue[MAX_INT_SIZE - j - 1] << ")";
                if (j < (targetName.size() - 1)) {
                    file << " , ";
                }
            }
        } else {
            for (int j = 0; j < targetName.size(); j++) {
                file << "." << targetName[j] << "(1'b" << (targetValue[MAX_INT_SIZE - j - 1] == '0'? '1': '0') << ")";
                if (j < (targetName.size() - 1)) {
                    file << " , ";
                }
            }
        }
        file << " );\n";
    }

    //mux: w[2^n-1] ~ 
    int startWire = (pow(2, targetName.size()) - 1);
    for (int i = 0; i < (pow(2, targetName.size()) - 1); i++) {
        bitset<MAX_INT_SIZE> bit_str(i);
        string targetValue = bit_str.to_string();
        file << "mux m" << i << " ( ";
        for (int j = 0; j < targetName.size(); j++) {
            if ( i == 0) {
                file << "." << targetName[j] << "(" << targetName[j] << "), ";
            } else {
                //from last ".a"
                file << "." << targetName[j] << "(n" << (startWire + ((i-1) * targetName.size()) + j) << "), ";
            }
        }
        for (int j = 0; j < targetName.size(); j++) {
            if ( i < (pow(2, targetName.size()) - 2)) {
                file << ".a_" << j << "(n" << (startWire + (i * targetName.size()) + j) << "), ";
            } else {
                file << ".a_" << j << "(1'b1), ";
            }
        }

        for (int j = 0; j < (targetName.size()); j++) {
            file << ".b_" << j << "(1'b" << targetValue[MAX_INT_SIZE - j - 1] << "), ";
        }

        file << ".sel(n" << i << "));\n"; 
    }

    file << "endmodule\n";
    file.close();
}


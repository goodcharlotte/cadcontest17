#include "datatype.h"

#define ALL_ONE -1
#define MAX_INT_SIZE 32

vector<string> targetName;
vector<string> F_piName;
vector<string> poName;
vector<string> G_piName;

void parse(string &cktF_name, string &cktG_name);
void constructMux(string &outputName);
void constructMiter(string &outputName);
void constructTop(string &outputName);
void constructPatch(string cktF_name, string cktG_name);


void constructPatch(string cktF_name, string cktG_name)
{
    string outputName = "patch.v";
    string systemCmd;

    parse(cktF_name, cktG_name);

    constructMux(outputName);
    
    //make sure modelNames are module F & module G
    systemCmd = "cat " + cktF_name + " " + cktG_name + " >> " + outputName;
    system(systemCmd.c_str());

    constructMiter(outputName);
    constructTop(outputName);
}

//void parse(string &cktF_name, vector<string> &targetName, vector<string> &piName, vector<string> &poName)
void parse(string &cktF_name, string &cktG_name)
{
    targetName.clear();
    F_piName.clear();
    G_piName.clear();
    poName.clear();

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
                poName.push_back(tmpstr);
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
                    fin = true;
                    break;
                }
                G_piName.push_back(tmpstr);
            }
        }
    }
    file.close();
}

//void constructMux(string &outputName, vector<string> &targetName)
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

//void constructMiter(string &outputName, vector<string> &targetName, vector<string> &piName, vector<string> &poName)
void constructMiter(string &outputName)
{
    ofstream file(outputName.c_str(), ios::app);
    file << "\nmodule miter ( miterOutput, " ;
    for (int i = 0; i < G_piName.size(); i++) {
        file << G_piName[i] << " , ";
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
    for (int i = 0; i < targetName.size(); i++) {
        file << targetName[i];
        if (i < (targetName.size() - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";

    file << "output miterOutput;\n";

    file << "wire ";
    for (int i = 0; i < poName.size(); i++) {
        file << "f_" << poName[i] << " , ";
    }
    for (int i = 0; i < poName.size(); i++) {
        file << "g_" << poName[i] << " , ";
    }
    for (int i = 0; i < poName.size(); i++) {
        file << "n" << i;
        if (i < (poName.size() - 1)) {
            file << " , ";
        }
    }
    file << " ;\n";

    for (int i = 0; i < poName.size(); i++) {
        file << "xor ( n" << i << " , f_" << poName[i] << " , g_" << poName[i] << " );\n";
    }

    file << "or ( miterOutput , ";
    for (int i = 0; i < poName.size(); i++) {
        file << "n" << i;
        if (i < (poName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\n";

    file << "F f0( ";
    for (int i = 0; i < poName.size(); i++) {
        file << "." << poName[i] << "(f_" << poName[i] << "), ";
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
    file << " );\n";

    file << "G g0( ";
    for (int i = 0; i < poName.size(); i++) {
        file << "." << poName[i] << "(g_" << poName[i] << "), ";
    }
    for (int i = 0; i < (G_piName.size()); i++) {
        file << "." << G_piName[i] << "(" << G_piName[i] << ")";
        if (i < (G_piName.size() - 1)) {
            file << " , ";
        }
    }
    file << " );\nendmodule\n\n";
    file.close();
}

//void constructTop(string &outputName, vector<string> &targetName, vector<string> &piName, vector<string> &poName)
void constructTop(string &outputName)
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
    
    //[ miterOut(2n-1) ] + muxConnect(2*((2n-1)-1)) = 6n-5
    //[ n0 ~ n(2n-2)]
    file << "wire ";
    for (int i = 0; i < (6 * targetName.size() - 5); i++) {
        file << "n" << i;
        if (i < (6 * targetName.size() - 6)) {
            file << " , ";
        }
    }
    file << " ;\n";

    //miter: 2n-1
    for (int i = 0; i < ((2 * targetName.size()) - 1); i++) {
        bitset<MAX_INT_SIZE> bit_str(i);
        string targetValue = bit_str.to_string();
        file << "miter p" << i << " ( .miterOutput(n" << i << "), ";
        for (int j = 0; j < (G_piName.size()); j++) {
            file << "." << G_piName[j] << "(" << G_piName[j] << "), ";
        }
        for (int j = 0; j < targetName.size(); j++) {
            file << "." << targetName[j] << "(1'b" << targetValue[MAX_INT_SIZE - j - 1] << ")";
            if (j < (targetName.size() - 1)) {
                file << " , ";
            }
        }      
        file << " );\n";
    }

    for (int i = 0; i < ((2 * targetName.size()) - 1); i++) {
        bitset<MAX_INT_SIZE> bit_str(i);
        string targetValue = bit_str.to_string();
        file << "mux m" << i << " ( ";
        for (int j = 0; j < targetName.size(); j++) {
            if ( i == 0) {
        file << "." << targetName[j] << "(" << targetName[j] << "), ";
            } else {
                file << "." << targetName[j] << "(n" << ((2 * targetName.size()) - 3 + (i * 2) + j) << "), ";
            }
        }
        for (int j = 0; j < targetName.size(); j++) {
            if ( i < ((2 * targetName.size()) - 2)) {
                file << ".a_" << j << "(n" << ((2 * targetName.size()) - 1 + (i * 2) + j) << "), ";
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


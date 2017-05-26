#include <iostream>
#include "datatype.h"
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
    cktg.writefile(argv[5]);
    return 0;
}

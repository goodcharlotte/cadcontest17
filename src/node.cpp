#include <iostream>
//#include <string>
#include <fstream>
#include "datatype.h"
using namespace::std;

Node_t::Node_t(string n, GateType t) : name(n), type(t) 
{ 
}

void Node_t::print(vector<Node_t> allnode)
{
    cout << "----> " << name << " " << type << endl;
    cout << "input: ";
    for (int i = 0; i < in.size(); i++) {
        cout << allnode[in[i]].name << " ";
    }
    cout << endl;
    cout << "output: ";
    for (int i = 0; i < out.size(); i++) {
        cout << allnode[out[i]].name << " ";
    }
    cout << endl;
}


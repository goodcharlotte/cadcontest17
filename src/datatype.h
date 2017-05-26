#ifndef _DATATYPE_
#define _DATATYPE_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace::std;
//              0     1    2    3    4    5    6    7     8
enum GateType {BUF, NOT, AND, NAND, OR, NOR, XOR, NXOR, PORT};


class Node_t
{
private:
    string name;
    GateType type;

public:
    vector<int> in;
    vector<int> out;
    Node_t(string n, GateType t);
    inline void addinput(int i) { in.push_back(i); }
    inline void addoutput(int i) { out.push_back(i); }
    inline void setType(GateType t) { type = t; }
    inline GateType getType() { return type; }
    inline string getName() { return name; }
    void print(vector<Node_t> allnode);
};

class Circuit_t
{
private:
    vector<Node_t> allnodevec;
    map<string, int> allnodemap;
    map<string, int>::iterator iter;
    vector<int> pi;
    vector<int> po;
    vector<int> target;
    
public:
    Circuit_t();
    bool readfile(char* fname);
    bool writefile(char* fname);
    void print();
};

#endif

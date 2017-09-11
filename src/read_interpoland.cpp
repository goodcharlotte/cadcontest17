#include <iostream>
#include <fstream>
#include <queue>
#include<climits>
#include <string> 
#define ROOT 1
#define CHAIN 0
#include <sstream>
#include <math.h>  
#include <algorithm>
#include "datatype.h"

using namespace::std;



int StringToInt(string number)
{
	
	int result;
	stringstream ss;
	ss.str(number);
	ss >> result;
	
	return result;
	
}

string IntToString(int number)
{
	stringstream result;
	result << number;
	return result.str();
}


struct Term{
	vector<int> literals;
	vector<int> OperateTermID;    //  "A" [C] "B" => D E 
	vector<int> Operateliteral;   //  A "[C]" B => D E 
	vector<int> CHAINLast;  //  A [C] B => "D" "E" 
	bool emp; // Empty
	bool root; //	chain = 0	root = 1
	int AorB; //	ROOT: A = 0  B = 1    CHAIN: -1  Initial: -1
	string ans;
	string name;
	vector<int> Operator; // 0 == buffer 1 == OR 2 == AND
	vector<string> outputWire;
	vector<string> OperandName;
	vector<int> PIID;
	
};
//int tmpwire = 0;
vector<Term> T;
vector<int> VariableInA;
vector<int> VariableInB;
vector<int> AllVariables;
vector<int> PIforPatch;
vector<string> PIforPatchName;
vector<string> AlltempWire;

int IsInAllVar(int number){
	for(int i = 0; i < AllVariables.size(); i++){
		if(number == AllVariables[i])
			return i;
		else;
	}
	return -1;
}

int IsInPI(int number){
	for(int i = 0; i < PIforPatch.size(); i++){
		if(number == PIforPatch[i])
			return i;
		else;
	}
	return -1;
}

bool IsInB(int variable) {
	
	if (variable < 0)
		variable = variable * (-1);
	
	for (int i = 0; i < VariableInB.size(); i++){
		
		if (variable == VariableInB[i])
			return true;
		
	}
	
	return false;
	
	
}

void SortAB()
{
	vector<int>::iterator eraseit;
	for(int i = 0; i < VariableInA.size(); i++){
		if(VariableInA[i] < 0){
			VariableInA[i] = VariableInA[i]*(-1);
		}
	}
	for(int i = 0; i < VariableInB.size(); i++){
		if(VariableInB[i] < 0){
			VariableInB[i] = VariableInB[i]*(-1);
		}
	}
	
	sort(VariableInA.begin(), VariableInA.end());
	sort(VariableInB.begin(), VariableInB.end());
	eraseit = unique(VariableInA.begin(), VariableInA.end());
	VariableInA.erase(eraseit, VariableInA.end());
	eraseit = unique(VariableInB.begin(), VariableInB.end());
	VariableInB.erase(eraseit, VariableInB.end());

	
	
}


void Circuit_t::SolveROOT_Fix() {
	
	
	int literalNum = 0;
	string tmpROOTName, tmpInternalName;
	string tmpName;
	string ROOT_tmpnamehead = "tmpwire_";
	
	
	
	
	for (int i = 0; i < T.size(); i++) {
		if(!T[i].root)
			continue;

		
		
		literalNum = 0;
		tmpROOTName.clear();
		tmpROOTName += ROOT_tmpnamehead;
		tmpROOTName += IntToString(i);
		AlltempWire.push_back(tmpROOTName);
		
		
		if(T[i].AorB == 0){  //A
			for(int j = 0; j < T[i].literals.size(); j++) {
				
				if ( IsInB(T[i].literals[j]) ){
					literalNum++;
					
					tmpName.clear();
					if( T[i].literals[j] < 0 ){
						tmpName += "~";
						tmpName += allnodevec[ (T[i].literals[j]*(-1))-1 ].name;
						T[i].PIID.push_back((T[i].literals[j]*(-1))-1);
						T[i].OperandName.push_back(tmpName);
					} else {
						tmpName += allnodevec[(T[i].literals[j])-1].name;
						T[i].PIID.push_back((T[i].literals[j])-1);
						T[i].OperandName.push_back(tmpName);
					}
					
					if(literalNum == 1) ;
					else if(literalNum == 2) {
						T[i].Operator.push_back(1);
					} else if(literalNum > 2) {
						tmpInternalName.clear();
						tmpInternalName += tmpROOTName;
						tmpInternalName += "_";
						tmpInternalName += IntToString(literalNum-3);
						AlltempWire.push_back(tmpInternalName);
						T[i].Operator.push_back(1);
						T[i].outputWire.push_back(tmpInternalName);
						T[i].OperandName.push_back(tmpInternalName);
						
					}
				}
				
				
				
			}
			
			if(literalNum == 0) {
				T[i].outputWire.push_back(tmpROOTName);
				T[i].Operator.push_back(0);
				T[i].OperandName.push_back("1'b0");
			} else if(literalNum == 1) {
				T[i].outputWire.push_back(tmpROOTName);
				T[i].Operator.push_back(0);
			}else {
				T[i].outputWire.push_back(tmpROOTName);
			}
			
			
		
		} else if (T[i].AorB == 1) { // B
			T[i].outputWire.push_back(tmpROOTName);
			T[i].Operator.push_back(0);
			T[i].OperandName.push_back("1'b1");
		} else {
			cout<<"ERROR SHOULD BE A OR B (ROOT) CHECK IF THIS IS A CHAIN"<<i<<endl;
			
		}
	}
		
	
}



void SolveCHAIN_Fix(int CHAINId) {
	
	int interwirenum = 0;
	string Clause_tmpoutputname;
	string tmpName;
	string Clause_tmpName = "tmpwire_";
	tmpName += "tmpwire_";
	Clause_tmpName += IntToString(CHAINId);
	Clause_tmpoutputname += Clause_tmpName;
	Clause_tmpoutputname += "_";
	
	AlltempWire.push_back(Clause_tmpName);
	for (int i = 0; i < T[CHAINId].Operateliteral.size(); i++) {
		if(i == 0) {
			T[CHAINId].OperandName.push_back(tmpName+IntToString(T[CHAINId].OperateTermID[0]));
			T[CHAINId].OperandName.push_back(tmpName+IntToString(T[CHAINId].OperateTermID[1]));
		} else {
			T[CHAINId].OperandName.push_back(tmpName+IntToString(T[CHAINId].OperateTermID[i+1]));
			T[CHAINId].OperandName.push_back(Clause_tmpoutputname+IntToString(interwirenum));
			T[CHAINId].outputWire.push_back(Clause_tmpoutputname+IntToString(interwirenum));
			AlltempWire.push_back(Clause_tmpoutputname+IntToString(interwirenum));
			interwirenum++;
		}
		
		if(IsInB(T[CHAINId].Operateliteral[i])) {
			T[CHAINId].Operator.push_back(2);
		} else {
			T[CHAINId].Operator.push_back(1);
		}
		
	}
	T[CHAINId].outputWire.push_back(Clause_tmpName);
	
	
	
}


void FindInterpoland(int CHAINId) {
	
	for(int i = 0; i < T.size(); i++) {
		
		if(T[i].root)
			continue;
		else {
			SolveCHAIN_Fix(i);
		}
		
		
	}
	
	
}




void IncreaseTerm(string Tname, bool RootOrChain)
{
	bool IDorLIT = false;   // ID = false LIT = true
	bool CLast = false;  // after "=>" CLast = true
	Term NewT;
	NewT.emp = 0;
	NewT.root = RootOrChain;
	NewT.name += Tname;
	NewT.AorB = -1;
	string tmplit;
	
	if(RootOrChain){ // ROOT
		tmplit.clear();

		for(int i = 0; i < Tname.size(); i++){
			if(Tname[i] != ' ')
				tmplit.push_back(Tname[i]);
			else if(Tname[i] == ' '){
				NewT.literals.push_back(StringToInt(tmplit));
				tmplit.clear();
			}
			
			if(i == Tname.size()-1 ){
				NewT.literals.push_back(StringToInt(tmplit));
			}
			
		}

		
	} else { // CHAIN

		
		
		CLast = false;
		for(int i = 0; i < Tname.size(); i++){
			
						
			if (Tname[i] == '[')
				IDorLIT = true;
			else if (Tname[i] == ']') ;
			else if (Tname[i] == '=') {
				CLast = true;
				i = i+2; // ignore "> ""
			}
			else if (Tname[i] == ' ') {
				
				
				if(IDorLIT && !CLast){  // LIT
					NewT.Operateliteral.push_back(StringToInt(tmplit));
				} else if(CLast){
					NewT.CHAINLast.push_back(StringToInt(tmplit));
				} else{   // ID
					NewT.OperateTermID.push_back(StringToInt(tmplit));
				}
				
				IDorLIT = false;
				tmplit.clear();
				
			}
			else {
				tmplit.push_back(Tname[i]);
			}
			
			
			
			
			if(i == Tname.size()-1 ){
				
				if(IDorLIT) {
					cout<<"ERROR: LAST ONE SHOULD NOT BE LIT"<<endl;
					
				}
				
				if(!CLast) {
					cout<<"CLast SHOULD BE TRUE IN THE END"<<endl;
					
				}
				
				NewT.CHAINLast.push_back(StringToInt(tmplit));
			}
			
		}
		

		
		
	}
	
	T.push_back(NewT);
	return ;
}

void readPartition(char* filename)    
{
	int ABflag = -1;   // 2 == PI
	ifstream fin;
    fin.open(filename,ios::in);
	string tmpstr, cmpstr, tmpcmp;
	string bufstr;
	vector<int> tmpLi;
	int Intmp;
	while(1) {
		tmpLi.clear();
		tmpstr.clear();
		cmpstr.clear();
		tmpcmp.clear();

		getline(fin, tmpstr, '\n');
		

		
		if (tmpstr.size() == 0)
			break;
		else if ((tmpstr.size() == 1) && (ABflag != 2)){
			if (tmpstr[0] == 'A') {

				ABflag = 0;
				continue;
			}
			else if (tmpstr[0] == 'B'){
				ABflag = 1;
				continue;
			}
			else {
				cout<<"ERROR: SHOULD BE A OR B"<<endl;
				cout<<"tmpstr = "<<tmpstr<<endl;
				
			}
		} else if(tmpstr.find("PI") != -1){
				
				break;
				ABflag = 2;
		} else {

			if(ABflag == 2){
				cout<<"ERROR ABflag SHOULD 0 or 1"<<endl;				
			} else {
				cmpstr.assign(tmpstr, 0, tmpstr.size()-2);

				bufstr.clear();
				for(int i = 0; i < cmpstr.size(); i++){
					if(cmpstr[i] == ' '){
						tmpLi.push_back(StringToInt(bufstr));
						bufstr.clear();
					} else if(i == cmpstr.size()-1) {
						bufstr+=cmpstr[i];
						tmpLi.push_back(StringToInt(bufstr));
						bufstr.clear();
					} else {
						bufstr+=cmpstr[i];
					}
				}
				
				for(int i = 0; i < tmpLi.size(); i++){
					for(int j = 1; j < tmpLi.size()-i; j++){
						if(abs(tmpLi[j]) < abs(tmpLi[j-1]) ){
							Intmp = tmpLi[j];
							tmpLi[j] = tmpLi[j-1];
							tmpLi[j-1] = Intmp;
						}
					}
				}
				cmpstr.clear();
				for(int i = 0; i < tmpLi.size()-1; i++){
					cmpstr += IntToString(tmpLi[i]);
					cmpstr += " ";
				}
				cmpstr += IntToString(tmpLi[tmpLi.size()-1]);
				
				

				
				for (int i = 0; i < T.size(); i++){
					if(cmpstr == T[i].name) {
						if (T[i].AorB == -1) {
							T[i].AorB = ABflag;
							
							if(ABflag == 0){ // A
								VariableInA.insert(VariableInA.begin(), T[i].literals.begin(), T[i].literals.end());

								
							} else if (ABflag == 1) { // B
								VariableInB.insert(VariableInB.begin(), T[i].literals.begin(), T[i].literals.end());

							}
							
						}
						else {
							cout<<"ERROR: T[i].AorB SHOULD BE NULL (-1) BUT IT IS "<<T[i].AorB<<endl;
							
						}
						break;
					}
				}
			}
		}
	}
	
	SortAB();

	
	
	
	fin.close();
	return ;
}



int readMinisatLog(char* filename)   //return First Chain Id
{   
	ifstream fin;
    fin.open(filename,ios::in);
	

	
	
	int rootCount = 0;
	int chainPos = -1, rootPos = -1, proof = -1;
	string tmpstr, tmpname;
	
	
	while(1) {
		getline(fin, tmpstr, '\n');
		proof = tmpstr.find("proof");
		
		if(proof != -1)
			break;
	}
	
	
	while(1) {
		tmpstr.clear();
		getline(fin, tmpstr, '\n');

		if(tmpstr.size() == 0)
			break;
		rootPos = tmpstr.find("ROOT");
		chainPos = tmpstr.find("CHAIN"); 
		if(rootPos != -1) {
			tmpname.clear();
			rootCount++ ;
			tmpname.assign(tmpstr, rootPos+5, tmpstr.size()-rootPos-5);
			IncreaseTerm(tmpname, ROOT);
			
			
			
		} else if (chainPos != -1) {
			
			tmpname.assign(tmpstr, chainPos+6, tmpstr.size()-chainPos-6);
			IncreaseTerm(tmpname, CHAIN);
			
			
		}
	}
	
	
	
	fin.close();
	return rootCount;
}

void Circuit_t::WriteInterpoland_Fix(char* patchname) {
	
	
	
	vector<int>::iterator eraseit;
	vector<bool> Visit_Clause;
	for(int i = 0; i < T.size(); i++)
		Visit_Clause.push_back(false);
	
	vector<int> Q;
	vector<int> Collect_ROOT;
	string tmpnamehead = "tmpwire_";
	string tmpname;
	ofstream fout;
	ifstream fin;
	fout.open("tmp_inte0_patch.v",ios::out);
	fout<<"assign t_0 = "<<tmpnamehead+(IntToString(T.size()-1))<<" ;"<<endl;
	Q.push_back(T.size()-1);
	while(!Q.empty()){
		if(Visit_Clause[Q[0]] == true) {
			Q.erase(Q.begin());
		}
		else {
			Visit_Clause[Q[0]] = true;
			
			if(!T[Q[0]].root){
				for (int i = 0; i < T[Q[0]].OperateTermID.size(); i++){
					Q.push_back(T[Q[0]].OperateTermID[i]);
				}
			} else{
				Collect_ROOT.push_back(Q[0]);
			}
			
			for(int i = 0; i < T[Q[0]].Operator.size(); i++) {
				fout<<"assign "<<T[Q[0]].outputWire[i]<<" = "<<T[Q[0]].OperandName[i*2]<<" ";
				if(T[Q[0]].Operator[i] == 0){
					fout<<";"<<endl;
				} else if(T[Q[0]].Operator[i] == 1) {
					fout<<"| "<<T[Q[0]].OperandName[i*2+1]<<" ;"<<endl;
				} else if(T[Q[0]].Operator[i] == 2) {
					fout<<"& "<<T[Q[0]].OperandName[i*2+1]<<" ;"<<endl;
				} else {
					cout<<"ERROR Operator SHOULD NOT BE "<<T[0].Operator[i]<<endl;
				}
			}
			Q.erase(Q.begin());
		}
	}
	fout<<"endmodule"<<endl<<endl;
	fout.close();
	
	sort(Collect_ROOT.begin(), Collect_ROOT.end());
	
	eraseit = unique(Collect_ROOT.begin(), Collect_ROOT.end());
	Collect_ROOT.erase(eraseit, Collect_ROOT.end());
	
	for(int i = 0; i < Collect_ROOT.size(); i++){
		
		for(int j =0; j < T[Collect_ROOT[i]].PIID.size(); j++)
			PIforPatch.push_back(T[Collect_ROOT[i]].PIID[j]);
	}
	sort(PIforPatch.begin(), PIforPatch.end());
	
	eraseit = unique(PIforPatch.begin(), PIforPatch.end());
	PIforPatch.erase(eraseit, PIforPatch.end());
	
	fin.open("tmp_inte0_patch.v",ios::in);
	string tmpstr;
	fout.open("tmp_inte1_patch.v",ios::out);
	fout<<"module  patch ( ";
	for(int i = 0; i < PIforPatch.size(); i++)
		fout<<allnodevec[PIforPatch[i]].name<<" , ";
	fout<<"t_0 );"<<endl;
	fout<<"input ";
	for(int i = 0; i < PIforPatch.size()-1; i++)
		fout<<allnodevec[PIforPatch[i]].name<<" , ";
	fout<<allnodevec[PIforPatch[PIforPatch.size()-1]].name<<" ;"<<endl;
	fout<<"output t_0 ;"<<endl;
	fout<<"wire ";
	for(int i = 0; i < AlltempWire.size()-1; i++)
		fout<<AlltempWire[i]<<" , ";
	fout<<AlltempWire[AlltempWire.size()-1]<<" ;"<<endl;
	while(1) {
		tmpstr.clear();
		getline(fin, tmpstr, '\n');
		if(tmpstr.find("endmodule")!=-1){
			fout<<tmpstr<<endl<<endl;
			break;
		} else{
			fout<<tmpstr<<endl;
		}
	}
	
	
	ofstream w_file2;
    w_file2.open("resyntemp_Patch_Fix.script",ios::out);
    w_file2 << "read_verilog tmp_inte1_patch.v"<< endl;
    w_file2 << "resyn2rs" << endl;
    w_file2 << "compress2rs" << endl;
    w_file2 << "read_library mcnc.genlib" << endl;
    w_file2 << "map" << endl;
    w_file2 << "write_verilog " << patchname << endl;
    w_file2.close();

    system("./abc -f resyntemp_Patch_Fix.script");
	
	
	
	
	
}

vector<int> Circuit_t::inteporlation(char* patchname)
{ 

	int CHAINfirstID;
	char  minisatname[30] = "proof.log";
	char  partitionname[30] = "partition.log";

	CHAINfirstID = readMinisatLog(minisatname);

	readPartition(partitionname);
	

	SolveROOT_Fix();

	

	FindInterpoland(CHAINfirstID);
	WriteInterpoland_Fix(patchname);
	

	return PIforPatch;
}






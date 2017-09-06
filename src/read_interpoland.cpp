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
//#define INTERPOLANT
//#define INTERPOLANT2
//#define INTERPOLANT3
//#define INTERPOLANT4
//#define INTERPOLANT5
//#define INTERPOLANT6
//#define INTERPOLANT7
//#define INTERPOLANT14
//#define TESTINSERT
//#define INTERPOLANT8
//#define INTERPOLANT9
//#define INTERPOLANT10
//#define INTERPOLANT11
//#define INTERPOLANT100
//#define SolveCHAINF
//#define SolveROOT_F
//#define INTERPOLANT22
using namespace::std;



int StringToInt(string number)
{
	
	int result;
	stringstream ss;
	ss.str(number);
	ss >> result;
	
	#ifdef INTERPOLANT2
		cout<<result<<" ";
	#endif
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
int tmpwire = 0;
vector<Term> T;
vector<int> VariableInA;
vector<int> VariableInB;
vector<int> AllVariables;
vector<string> AllVariablesName;
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
	//vector<int> tempvariables;
	for(int i = 0; i < VariableInA.size(); i++){
		if(VariableInA[i] < 0){
			VariableInA[i] = VariableInA[i]*(-1);
			//tempvariables.push_back(VariableInA[i]);
		}
	}
	for(int i = 0; i < VariableInB.size(); i++){
		if(VariableInB[i] < 0){
			VariableInB[i] = VariableInB[i]*(-1);
			//tempvariables.push_back(VariableInB[i]);
		}
	}
	
	sort(VariableInA.begin(), VariableInA.end());
	sort(VariableInB.begin(), VariableInB.end());
	// sort(tempvariables.begin(), tempvariables.end());
	eraseit = unique(VariableInA.begin(), VariableInA.end());
	VariableInA.erase(eraseit, VariableInA.end());
	eraseit = unique(VariableInB.begin(), VariableInB.end());
	VariableInB.erase(eraseit, VariableInB.end());
	// eraseit = unique(tempvariables.begin(), tempvariables.end());
	// tempvariables.erase(eraseit, tempvariables.end());
	
	// bool findInPI = false;
	// for(int i = 0; i < tempvariables.size(); i++){
		// findInPI = false;
		// for(int j = 0; j < PIforPatch.size(); j++){
			// if(tempvariables[i] == PIforPatch[j]){
				// findInPI = true;
				// break;
			// }
			// else ;
		// }
		// if(!findInPI)
			// AllVariables.push_back(tempvariables[i]);
	// }
	// string tmpnamehead = "tmpwire_";
	// string tmpname;
	// for(int i = 0; i < AllVariables.size(); i++) {
		// tmpname.clear();
		// tmpname += tmpnamehead;
		// tmpname += IntToString(tmpwire);
		// tmpwire++;
		// AllVariablesName.push_back(tmpname);
	// }
	
	
	#ifdef INTERPOLANT7
		cout<<endl<<"Final A is "<<endl;
		for(int i = 0; i < VariableInA.size(); i++)
			cout<<VariableInA[i]<<" ";
		cout<<endl<<"Final B is "<<endl;
		for(int i = 0; i < VariableInB.size(); i++)
			cout<<VariableInB[i]<<" ";
		cout<<endl;
		 // cout<<endl<<"ALL is "<<endl;
		 // for(int i = 0; i < AllVariables.size(); i++)
			 // cout<<AllVariables[i]<<" ";
		 // cout<<endl;
	#endif
	
	
}
/*
void SolveROOT() {
	
	#ifdef TESTINSERT
		string TEST;
		TEST += "( A or B )";
		string TEST1;
		TEST1 += "( A and B )";
		TEST1.insert(0, TEST);
		TEST1.insert(0, "( ");
		TEST1 += " )";
		cout<<"TEST1 : "<<TEST1<<endl;
	#endif
	
	bool Nonflag = true;
	bool Orflag = false;
	bool secondflag = false;
	
	for (int i = 0; i < T.size(); i++) {
		if(!T[i].root)
			break;
		
		Nonflag = true;
		Orflag = false;
		secondflag = false;
		if(T[i].AorB == 0){  //A
			for(int j = 0; j < T[i].literals.size(); j++) {
				
				if ( IsInB(T[i].literals[j]) ){
					if(!Orflag)
						Orflag = true;
					else {
						if(!secondflag) {
							secondflag = true;
							T[i].ans += "| ";
						}
						else {
							T[i].ans.insert(0, "( ");
							T[i].ans += ") | ";
						}
						
						
						
					}
					Nonflag = false;
					T[i].ans += "( ";
					T[i].ans += IntToString(T[i].literals[j]);
					T[i].ans += " ) ";
				}
				
				if( (j == T[i].literals.size()-1) && (Nonflag) )
					T[i].ans += "( 1'b0 ) ";
			}
			
			if(secondflag) {
				T[i].ans.insert(0, "( ");
				T[i].ans += ")";
			}
		
		} else if (T[i].AorB == 1) { // B
			T[i].ans.insert(0, "( 1'b1 ) ");
		} else {
			cout<<"ERROR SHOULD BE A OR B (ROOT) CHECK IF THIS IS A CHAIN"<<i<<endl;
			
		}
	}
	
	#ifdef INTERPOLANT8
		for (int i = 0; i < T.size(); i++) {
			if(!T[i].root)
				break;
			
			cout<<i<<" : "<<T[i].ans<<endl;
			
		}
	#endif
	
	
}

*/


void Circuit_t::SolveROOT_Fix() {
	
	
	int literalNum = 0;
	string tmpROOTName, tmpInternalName;
	string tmpName;
	string ROOT_tmpnamehead = "tmpwire_";
	
	
	
	
	for (int i = 0; i < T.size(); i++) {
		if(!T[i].root)
			break;
		#ifdef INTERPOLANT19
			cout<<"Solving ROOT "<<i<<endl;
			cout<<"T[i].AorB = "<<T[i].AorB<<endl;
		#endif
		
		
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
						#ifdef INTERPOLANT19
							cout<<"literal = "<<T[i].literals[j]<<endl;
						#endif
						tmpName += "~";
						tmpName += allnodevec[ (T[i].literals[j]*(-1))-1 ].name;
						T[i].PIID.push_back((T[i].literals[j]*(-1))-1);
						T[i].OperandName.push_back(tmpName);
						#ifdef INTERPOLANT19
							cout<<"Operand Name = "<<tmpName<<endl;
						#endif
					} else {
						#ifdef INTERPOLANT19
							cout<<"literal = "<<T[i].literals[j]<<endl;
						#endif
						tmpName += allnodevec[(T[i].literals[j])-1].name;
						T[i].PIID.push_back((T[i].literals[j])-1);
						T[i].OperandName.push_back(tmpName);
						#ifdef INTERPOLANT19
							cout<<"Operand Name = "<<tmpName<<endl;
						#endif
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
	
	#ifdef INTERPOLANT20
		for (int i = 0; i < T.size(); i++) {
			if(!T[i].root)
				break;
			
			cout<<endl<<"Final A is "<<endl;
			for(int j = 0; j < VariableInA.size(); j++)
				cout<<VariableInA[j]<<" ";
			cout<<endl<<"Final B is "<<endl;
			for(int j = 0; j < VariableInB.size(); j++)
				cout<<VariableInB[j]<<" ";
			cout<<endl;
			
			cout<<i<<" name is "<<T[i].name<<endl<<endl;
			for (int j = 0; j < T[i].Operator.size(); j++) {
				cout<<"assign "<<T[i].outputWire[j]<<" = "<<T[i].OperandName[j*2]<<" ";
				if(T[i].Operator[j] == 0)
					cout<<";"<<endl;
				else if(T[i].Operator[j] == 1){
					cout<<"| "<<T[i].OperandName[j*2+1]<<" ;"<<endl;
				}
				else{
					cout<<"ERROR Operator SHOULD NOT BE "<<T[i].Operator[j]<<endl;
				}
			}
			cout<<i<<" end"<<endl<<endl;
			
		}
	#endif
	
	
	
}


/*
void SolveCHAIN(int CHAINId) {
	
	// First Two Clause
	if(IsInB(T[CHAINId].Operateliteral[0])) {  // c1 and c2
		T[CHAINId].ans.insert(0, "( ");
		T[CHAINId].ans += T[T[CHAINId].OperateTermID[0]].ans;
		T[CHAINId].ans += "& ";
		T[CHAINId].ans += T[T[CHAINId].OperateTermID[1]].ans;
		T[CHAINId].ans += " ) ";
		T[CHAINId].Operateliteral.erase(T[CHAINId].Operateliteral.begin());
		T[CHAINId].OperateTermID.erase(T[CHAINId].OperateTermID.begin(), T[CHAINId].OperateTermID.begin()+2);
	} else { // c1 or c2
		T[CHAINId].ans.insert(0, "( ");
		T[CHAINId].ans += T[T[CHAINId].OperateTermID[0]].ans;
		T[CHAINId].ans += "| ";
		T[CHAINId].ans += T[T[CHAINId].OperateTermID[1]].ans;
		T[CHAINId].ans += " ) ";
		T[CHAINId].Operateliteral.erase(T[CHAINId].Operateliteral.begin());
		T[CHAINId].OperateTermID.erase(T[CHAINId].OperateTermID.begin(), T[CHAINId].OperateTermID.begin()+2);		
	}

	
	
	while(!T[CHAINId].Operateliteral.empty()) {
		
		#ifdef SolveCHAINF
			cout<<"CHAINId"<<CHAINId<<endl;
		#endif
		
		#ifdef SolveCHAINF
			cout<<"ans size = "<<T[CHAINId].ans.size()<<endl;
		#endif
		
		T[CHAINId].ans.insert(0, "( ");
		if(IsInB(T[CHAINId].Operateliteral[0])) {  // c1 and c2
			T[CHAINId].ans += "& ";
			T[CHAINId].ans += T[T[CHAINId].OperateTermID[0]].ans;
			T[CHAINId].ans += " ) ";		
		} else { // c1 or c2
			T[CHAINId].ans += "| ";
			T[CHAINId].ans += T[T[CHAINId].OperateTermID[0]].ans;
			T[CHAINId].ans += " ) ";		
		}
		
		
		
		T[CHAINId].Operateliteral.erase(T[CHAINId].Operateliteral.begin());
		T[CHAINId].OperateTermID.erase(T[CHAINId].OperateTermID.begin());
	}
	#ifdef SolveCHAINF
		cout<<"CHAINId end"<<endl;
	#endif
	
}
*/
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
	
	for(int i = CHAINId; i < T.size(); i++) {
		
		#ifdef INTERPOLANT5
			cout<<"Now Id : "<<i<<endl;
		#endif
		SolveCHAIN_Fix(i);
	//	SolveCHAIN(i);
		
		#ifdef INTERPOLANT21
			cout<<endl<<"Final A is "<<endl;
			for(int j = 0; j < VariableInA.size(); j++)
				cout<<VariableInA[j]<<" ";
			cout<<endl<<"Final B is "<<endl;
			for(int j = 0; j < VariableInB.size(); j++)
				cout<<VariableInB[j]<<" ";
			cout<<endl<<endl;
			
			cout<<i<<" name is "<<T[i].name<<endl;
			for (int j = 0; j < T[i].Operator.size(); j++) {
				cout<<"assign "<<T[i].outputWire[j]<<" = "<<T[i].OperandName[j*2]<<" ";
				if(T[i].Operator[j] == 0)
					cout<<";"<<endl;
				else if(T[i].Operator[j] == 1){
					cout<<"| "<<T[i].OperandName[j*2+1]<<" ;"<<endl;
				}
				else if(T[i].Operator[j] == 2){
					cout<<"& "<<T[i].OperandName[j*2+1]<<" ;"<<endl;
				}
				else {
					cout<<"ERROR Operator SHOULD NOT BE "<<T[i].Operator[j]<<endl;
				}
			}
			cout<<i<<" end"<<endl<<endl;
		#endif
		
		
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
		#ifdef INTERPOLANT4
			cout<<Tname<<endl;
		#endif
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
		#ifdef INTERPOLANT2
			cout<<endl;
		#endif
		
	} else { // CHAIN
		#ifdef INTERPOLANT3
			cout<<"Tname = "<<Tname<<endl;
		#endif
		
		
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
		
		#ifdef INTERPOLANT3
			cout<<"        ";
			for(int i = 0; i < NewT.OperateTermID.size(); i++) {
				
				cout<<NewT.OperateTermID[i]<<" ";
				if(i != NewT.OperateTermID.size()-1)
					cout<<"["<<NewT.Operateliteral[i]<<"] ";
				
			}
			cout<<"=> ";
			for (int i = 0; i < NewT.CHAINLast.size(); i++) {
				cout<<NewT.CHAINLast[i]<<" ";
			}
			cout<<endl;
		#endif
		
		
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
		
		#ifdef INTERPOLANT100
			cout<<"tmpstr = "<<tmpstr<<endl;
			cout<<"tmpstr size = "<<tmpstr.size();
		#endif
		
		if (tmpstr.size() == 0)
			break;
		else if ((tmpstr.size() == 1) && (ABflag != 2)){
			if (tmpstr[0] == 'A') {
				#ifdef INTERPOLANT100
					cout<<"Here!?"<<tmpstr<<endl;
				#endif
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
			#ifdef INTERPOLANT100
				cout<<"tmpstr"<<endl;
				for(int i = 0; i < tmpstr.size(); i++)
					cout<<tmpstr[i];
				cout<<endl;
				cout<<"tmpstr size "<<tmpstr.size()<<endl;
				cout<<"ABflag = "<<ABflag<<endl;
			#endif
			if(ABflag == 2){
				cout<<"ERROR ABflag SHOULD 0 or 1"<<endl;				
			} else {
				cmpstr.assign(tmpstr, 0, tmpstr.size()-2);
				#ifdef INTERPOLANT100
					cout<<"cmp ="<<endl;
					cout<<cmpstr<<endl;
				#endif
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
				
				
				#ifdef INTERPOLANT100
					cout<<cmpstr<<endl;
				#endif
				
				for (int i = 0; i < T.size(); i++){
					if(cmpstr == T[i].name) {
						if (T[i].AorB == -1) {
							T[i].AorB = ABflag;
							
							if(ABflag == 0){ // A
								VariableInA.insert(VariableInA.begin(), T[i].literals.begin(), T[i].literals.end());
								#ifdef INTERPOLANT6
									cout<<"A: "<<endl;
									for(int z = 0; z < VariableInA.size(); z++)
										cout<<VariableInA[z]<<" ";
									cout<<endl<<endl;
								#endif
								
							} else if (ABflag == 1) { // B
								VariableInB.insert(VariableInB.begin(), T[i].literals.begin(), T[i].literals.end());
								#ifdef INTERPOLANT6
									cout<<"B: "<<endl;
									for(int z = 0; z < VariableInB.size(); z++)
										cout<<VariableInB[z]<<" ";
									cout<<endl<<endl;
								#endif
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
	//cout<<"SortAB"<<endl;
	//SolveROOT();
	
	
	
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
	
	// solve the blank line
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
/*
void Circuit_t::WriteInterpoland()
{
	string tmpnamehead = "tmpwire_";
	string tmpname;
	ofstream fout;
	fout.open("temp_patch0.v",ios::out);
	
	
	#ifndef INTERPOLANT11
		int last = T.size()-1;
	#endif
	string checkstr, tmpcheck;
	vector<char> tmpstr;
	#ifdef INTERPOLANT11
		int last = 49;
	#endif
	int OperateInvolve = -1;  //  -1 : do nothing 0 : and  1: or
	vector<char> OperandA, OperandB;
	#ifdef INTERPOLANT10
		vector<char> temp;
		char tmp;
		for(int i = 0; i < T[36].ans.size(); i++)
			temp.push_back(T[36].ans[i]);		
		
		while(!temp.empty()) {
			tmp = temp.back();
			temp.pop_back();
			cout<<tmp;
			
		}
		cout<<endl;
	#endif
	//cout<<"last = "<<last<<endl;
	#ifdef INTERPOLANT100
		cout<<"ans ="<<endl;
		cout<<T[last].ans<<endl;
	#endif
	for (int i = 0; i < T[last].ans.size(); i++) {
		tmpstr.push_back(T[last].ans[i]);
		
		if ( tmpstr.back() != ')' ) ;
		else {
			OperateInvolve = -1;
			OperandA.clear();
			OperandB.clear();
			while ( tmpstr.back() != '(' ) {
				
				if(tmpstr.back() == '|' && OperateInvolve == -1){
					OperateInvolve = 1;
				} else if (tmpstr.back() == '&' && OperateInvolve == -1) {
					OperateInvolve = 0;
				} else if ( (tmpstr.back() == '|' || tmpstr.back() == '&') && OperateInvolve != -1) {
					cout<<"ERROR OperateInvolve SHOULD BE -1"<<endl;
					
				} else if (tmpstr.back() == ' ' || tmpstr.back() == ')'){
					
				} else if (tmpstr.back() == '-') {
					if (OperateInvolve == -1) {
						OperandA.push_back('~');
					} else {
						OperandB.push_back('~');
					}
				} else {
					if (OperateInvolve == -1) {
						OperandA.push_back(tmpstr.back());
					} else {
						OperandB.push_back(tmpstr.back());
					}
				}
				
				tmpstr.pop_back();
				
				
			}
			
			tmpstr.pop_back();
			
			#ifdef INTERPOLANT11
				cout<<"OperandA = ";
				for(int i = OperandA.size()-1 ; i >= 0; i-- )
					cout<<OperandA[i];
				cout<<endl;
				
				cout<<"OperandB = ";
				for(int i = OperandB.size()-1 ; i >= 0; i-- )
					cout<<OperandB[i];
				cout<<endl;
			#endif
			
			
			if(OperateInvolve == -1) {
				checkstr.clear();
				tmpcheck.clear();
				for(int i = OperandA.size()-1 ; i >= 0; i-- )
					checkstr += OperandA[i];
				if (checkstr.find("tmpwire_")!=-1 || checkstr.find("1'b")!=-1){
					for(int i = 0; i <checkstr.size(); i++)
						tmpstr.push_back(checkstr[i]);
				} else{
					
					if(checkstr[0] == '~'){
						tmpcheck +=checkstr;
						tmpcheck.erase(tmpcheck.begin());
						#ifdef INTERPOLANT14
							cout<<"original is : "<<checkstr<<endl;
						#endif
					/*	if(IsInAllVar(StringToInt(tmpcheck))!=-1){
							tmpstr.push_back(checkstr[0]);
							checkstr.clear();
							checkstr += AllVariablesName[IsInAllVar(StringToInt(tmpcheck))];
							#ifdef INTERPOLANT14
								cout<<"in variables now is : ~"<<checkstr<<endl;
							#endif
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
						} else */ /*
						if(IsInPI(StringToInt(tmpcheck)-1)!=-1) {
							
							tmpstr.push_back(checkstr[0]);
							checkstr.clear();
							checkstr += PIforPatchName[IsInPI(StringToInt(tmpcheck)-1)];
							
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
							#ifdef INTERPOLANT100
								cout<<endl;
								cout<<"ID: "<<StringToInt(tmpcheck)-1<<endl;
								cout<<"tmpstr is: ";
								for(int i = 0; i < tmpstr.size();i++){
									cout<<tmpstr[i];
								}
								cout<<endl;
							#endif
						} else {
							
							PIforPatch.push_back(StringToInt(tmpcheck)-1);
							PIforPatchName.push_back(allnodevec[StringToInt(tmpcheck)-1].name);
							tmpstr.push_back(checkstr[0]);
							checkstr.clear();
							checkstr += allnodevec[StringToInt(tmpcheck)-1].name;
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
							
							#ifdef INTERPOLANT100
								cout<<endl;
								cout<<"ID: "<<StringToInt(tmpcheck)-1<<endl;
								cout<<"in PI now is : ~"<<allnodevec[StringToInt(tmpcheck)-1].name<<endl;
								cout<<"tmpstr is: ";
								for(int i = 0; i < tmpstr.size();i++){
									cout<<tmpstr[i];
								}
								cout<<endl;
							#endif
						}
						
					} else{
						tmpcheck +=checkstr;
						#ifdef INTERPOLANT14
							cout<<"original is : "<<tmpcheck<<endl;
						#endif
					/*	if(IsInAllVar(StringToInt(tmpcheck))!=-1){
							checkstr.clear();
							checkstr += AllVariablesName[IsInAllVar(StringToInt(tmpcheck))];
							#ifdef INTERPOLANT14
								cout<<"in variables now is : "<<checkstr<<endl;
							#endif
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
						} else */ /*
						if(IsInPI(StringToInt(tmpcheck)-1)!=-1) {
							
							checkstr.clear();
							checkstr += PIforPatchName[IsInPI(StringToInt(tmpcheck)-1)];
							
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
							#ifdef INTERPOLANT100
								cout<<endl;
								cout<<"ID: "<<StringToInt(tmpcheck)-1<<endl;
								cout<<"tmpstr is: ";
								for(int i = 0; i < tmpstr.size();i++){
									cout<<tmpstr[i];
								}
								cout<<endl;
							#endif
						} else {
							PIforPatch.push_back(StringToInt(tmpcheck)-1);
							PIforPatchName.push_back(allnodevec[StringToInt(tmpcheck)-1].name);
							checkstr.clear();
							checkstr += allnodevec[StringToInt(tmpcheck)-1].name;
							for(int i = 0; i < checkstr.size();i++){
								tmpstr.push_back(checkstr[i]);
							}
							
							#ifdef INTERPOLANT100
								cout<<endl;
								cout<<"ID: "<<StringToInt(tmpcheck)-1<<endl;
								cout<<"in PI now is : "<<allnodevec[StringToInt(tmpcheck)-1].name<<endl;
								cout<<"tmpstr is: ";
								for(int i = 0; i < tmpstr.size();i++){
									cout<<tmpstr[i];
								}
								cout<<endl;
							#endif
						}
					}
					
				}
				#ifdef INTERPOLANT11
					cout<<"Now tmpstr is: ";
					for(int i = 0; i < tmpstr.size(); i++)
						cout<<tmpstr[i];
					cout<<endl<<endl;
				#endif
			} else if (OperateInvolve == 0) {  // AND
				tmpname.clear();
				tmpname += tmpnamehead;
				tmpname += IntToString(tmpwire);
				tmpwire++;
				tmpstr.insert(tmpstr.end(), tmpname.begin(), tmpname.end());
				#ifdef INTERPOLANT11
					cout<<"Now tmpstr is: ";
					for(int i = 0; i < tmpstr.size(); i++)
						cout<<tmpstr[i];
					cout<<endl<<endl;
				#endif
				//fout<<"and ( "<<tmpname<<" , ";
				fout<<"assign "<<tmpname<<" = ";
				for(int i = OperandA.size()-1 ; i >= 0; i-- )
					fout<<OperandA[i];
				fout<<" & ";
				for(int i = OperandB.size()-1 ; i >= 0; i-- )
					fout<<OperandB[i];
				fout<<" ;"<<endl;
				
			} else if (OperateInvolve == 1) {  // OR
				tmpname.clear();
				tmpname += tmpnamehead;
				tmpname += IntToString(tmpwire);
				tmpwire++;
				tmpstr.insert(tmpstr.end(), tmpname.begin(), tmpname.end());
				#ifdef INTERPOLANT11
					cout<<"Now tmpstr is: ";
					for(int i = 0; i < tmpstr.size(); i++)
						cout<<tmpstr[i];
					cout<<endl<<endl;
				#endif
				//fout<<"or ( "<<tmpname<<" , ";
				fout<<"assign "<<tmpname<<" = ";
				for(int i = OperandA.size()-1 ; i >= 0; i-- )
					fout<<OperandA[i];
				fout<<" | ";
				for(int i = OperandB.size()-1 ; i >= 0; i-- )
					fout<<OperandB[i];
				fout<<" ;"<<endl;
				
			}
			
			
		}
		
	}
	fout<<"buf ( t_0 , tmpwire_"<<tmpwire-1<<" );"<<endl;
	fout<<"endmodule"<<endl;
	//cout<<"tmpstr size = "<<tmpstr.size()<<endl;
	//cout<<tmpstr[0]<<endl;
	
	fout.close();
	string bufferstr;
	ifstream fin;
	ofstream ffout;
	fin.open("temp_patch0.v",ios::in);
	ffout.open("temp_patch.v",ios::out);
	ffout<<"module  patch (";
	 for(int i = 0; i < PIforPatchName.size(); i++)
		 ffout<<PIforPatchName[i]<<" , ";
	ffout<<"t_0 );"<<endl;
	ffout<<"input ";
	
	for(int i = 0; i < PIforPatch.size()-1; i++)
		ffout<<PIforPatchName[i]<<" , ";
	ffout<<PIforPatchName[PIforPatch.size()-1]<<" ;"<<endl; 
	ffout<<"output t_0 ;"<<endl<<"wire ";
	for(int i = 0; i < tmpwire -1; i++)
		ffout<<"tmpwire_"<<i<<" , ";
	ffout<<"tmpwire_"<<tmpwire-1<<" ;"<<endl;
	while(1){
		bufferstr.clear();
		getline(fin, bufferstr, '\n');
		#ifdef INTERPOLANT14
			cout<<"bufferstr = "<<bufferstr<<endl;
			
		#endif
		if(bufferstr.find("endmodule")!=-1){
			ffout<<bufferstr<<endl<<endl;
			break;
		} else{
			ffout<<bufferstr<<endl;
		}
	}
	fin.close();
	ffout.close();
	
	#ifdef INTERPOLANT100
			cout<<"start ABC"<<endl;
			
	#endif
	
	
	ofstream w_file2;
    w_file2.open("resyntemp_Patch.script",ios::out);
    w_file2 << "read_verilog temp_patch.v"<< endl;
    //w_file2 << "resyn2" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    w_file2 << "resyn2rs" << endl;
    w_file2 << "compress2rs" << endl;
    w_file2 << "read_library mcnc.genlib" << endl;
    w_file2 << "map" << endl;
    w_file2 << "write_verilog " << "patch.v" << endl;
    w_file2.close();

    system("./abc -f resyntemp_Patch.script");
	
	#ifdef INTERPOLANT100
			cout<<"end ABC"<<endl;
			
	#endif
	
	
	
	return ;
	
	
}

*/
void Circuit_t::WriteInterpoland_Fix() {
	#ifdef INTERPOLANT22
		cout<<"T size is: "<<T.size()<<endl;
	#endif
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
    //w_file2 << "resyn2" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    //w_file2 << "resyn2rs" << endl;
    //w_file2 << "compress2rs" << endl;
    w_file2 << "resyn2rs" << endl;
    w_file2 << "compress2rs" << endl;
    w_file2 << "read_library mcnc.genlib" << endl;
    w_file2 << "map" << endl;
    w_file2 << "write_verilog " << "patch.v" << endl;
    w_file2.close();

    system("./abc -f resyntemp_Patch_Fix.script");
	
	
	
	
	
}

vector<int> Circuit_t::inteporlation()
{ 
	#ifdef INTERPOLANT100
		cout<<"start inteporlation"<<endl;
	#endif
	int CHAINfirstID;
	char  minisatname[30] = "proof.log";
	char  partitionname[30] = "partition.log";

	CHAINfirstID = readMinisatLog(minisatname);

	readPartition(partitionname);
	
	#ifdef SolveROOT_F
		cout<<"SolveROOT_Fix"<<endl;
	#endif
	SolveROOT_Fix();
	#ifdef SolveROOT_F
		cout<<"end SolveROOT_Fix"<<endl;
	#endif
	
	
	#ifdef INTERPOLANT
		for (int i = 0; i < T.size(); i++) {
			cout<<i<<": ";
			if (T[i].root){
				cout<<"ROOT ";
				for(int j = 0; j < T[i].literals.size(); j++)
					cout<<T[i].literals[j]<<" ";
				if (T[i].AorB == 0)
					cout<<"A"<<endl;
				else if (T[i].AorB == 1)
					cout<<"B"<<endl;
				else
					cout<<"BUG SHOULD BE A OR B"<<endl;
			}
			else {
				cout<<"CHAIN ";
				for(int j = 0; j < T[i].OperateTermID.size(); j++){
					cout<<T[i].OperateTermID[j]<<" ";
					if(j != T[i].OperateTermID.size()-1)
						cout<<"["<<T[i].Operateliteral[j]<<"] ";
				}
				cout<<"=> ";
				for(int j = 0; j < T[i].CHAINLast.size(); j++){
					cout<<T[i].CHAINLast[j]<<" ";
				}
				if (T[i].AorB == -1)
					cout<<"NULL"<<endl;
				else
					cout<<"BUG SHOULD NOT BE A OR BE BUT "<<T[i].AorB<<endl;
			}
		}
	#endif
	FindInterpoland(CHAINfirstID);
	WriteInterpoland_Fix();
	#ifdef INTERPOLANT9
		for (int i = 0; i < T.size(); i++)
			cout<<i<<" ans : "<<T[i].ans<<endl;
	#endif
	
	//WriteInterpoland();
	
	#ifdef INTERPOLANT100
		cout<<"end inteporlation"<<endl;
	#endif

	return PIforPatch;
}






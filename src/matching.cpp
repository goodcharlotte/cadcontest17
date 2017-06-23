#include <iostream>
#include <fstream>
#include <algorithm>
#include "datatype.h"
using namespace::std;

//vector<int> VisitNodeC1, VisitNodeC2;      // check if visit
vector<int>	WireMapC1, WireMapC2;

vector<int> compareNumC1, compareNumC2;    // C1 : compareNumC1[0] = 3  C2 : compareNumC2[0] = 4   C1 compareC1[0~2] compare with compareC2[0~3]
vector<int> compareC1, compareC2;
vector<int> visitNodC1, visitNodC2;

int Circuit_t::NewPI(Circuit_t &c2)        //c2 == cktg
{

	vector<int>::iterator eraseit;
	
	vector< vector<int> > group;
	vector<int> tempgroup;
	vector<int> temp;
	vector <int> visit ;
	vector <int> New;
	vector <int> NodetoCheck;
	vector <int> ready;
	vector <int> relateNode;

	

	WireMapC1.assign(allnodevec.size(), -1);
	WireMapC2.assign(c2.allnodevec.size(), -1);

	
	
	for (int i = 0; i < pi.size(); i++) {
		temp.clear();
		tempgroup.clear();
		WireMapC1[pi[i]] = c2.pi[i];   // PI mapping
		WireMapC2[c2.pi[i]] = pi[i];   // PI mapping
		compareNumC1.push_back(allnodevec[pi[i]].out.size());
		compareNumC2.push_back(c2.allnodevec[c2.pi[i]].out.size());
		tempgroup.push_back(pi[i]);
		for (int j = 0; j < allnodevec[pi[i]].out.size(); j++) {
			compareC1.push_back(allnodevec[pi[i]].out[j]);
			visitNodC1.push_back(allnodevec[pi[i]].out[j]);
			temp.push_back(allnodevec[pi[i]].out[j]);
		}
		for (int j = 0; j < c2.allnodevec[c2.pi[i]].out.size(); j++) {
			compareC2.push_back(c2.allnodevec[c2.pi[i]].out[j]);
			visitNodC2.push_back(c2.allnodevec[c2.pi[i]].out[j]);
		}
	}

	while (!compareC1.empty() && !compareC2.empty()) {
		
		for (int i = 0; i < compareNumC1[0]; i++) {
			if (WireMapC1[compareC1[i]] == -1) {
				for (int j = 0; j < compareNumC2[0]; j++) {
					if (WireMapC2[compareC2[j]] == -1) {
						CompareNode(compareC1[i], compareC2[j], c2);
					}
					else ;
				}
			}
			else ;
		}
		compareC1.erase(compareC1.begin(), compareC1.begin() + compareNumC1[0]);
		compareC2.erase(compareC2.begin(), compareC2.begin() + compareNumC2[0]);
		compareNumC1.erase(compareNumC1.begin());
		compareNumC2.erase(compareNumC2.begin());
	}
	
	
	

	for (int i = 0; i < allnodevec.size(); i++) {
		if (WireMapC1[i] != -1) {
//			cout<<"C1: "<<allnodevec[i].getName()<<" map C2: "<<c2.allnodevec[WireMapC1[i]].getName()<<endl;
		}
	}

	
	
	
	visit.assign(allnodevec.size(), 0);
	ready.assign(allnodevec.size(), 0);
	NodetoCheck.assign(pi.begin(), pi.end());

	for (int i = 0 ; i < NodetoCheck.size(); i++) {
		ready[NodetoCheck[i]] = 1;
	}
	
	newpi.assign(pi.begin(), pi.end());
	
	while ( !NodetoCheck.empty() ) {
		New.clear();
		relateNode.clear();
//		cout<<"Doing: "<<allnodevec[NodetoCheck[0]].getName()<<endl<<endl;
		if ( visit[NodetoCheck[0]] == 0) {

			if(IsPiReplace(NodetoCheck[0], New, relateNode, NodetoCheck, visit, ready)) {
				NodetoCheck.insert(NodetoCheck.end(), New.begin(), New.end());
				ReplacePi(New, relateNode);
//				cout<<endl<<endl;
//				cout<<"new round pi :";
//				for (int i = 0; i < newpi.size(); i++){
//					cout<<allnodevec[newpi[i]].getName()<<" ";
//				}
//				cout<<endl<<endl;
				
			}
		}
		else ;
		NodetoCheck.erase(NodetoCheck.begin());
		
		
		
		
	}
	
	sort(newpi.begin(), newpi.end());
	eraseit = unique(newpi.begin(), newpi.end());
	newpi.erase(eraseit, newpi.end());
	
	

	if ( newpi.size() >= pi.size() ){
//		cout<<"newpi.size >= pi.size"<<endl;
//		cout<<"newpi: "<<newpi.size()<<" pi.size: "<<pi.size()<<endl;
		newpi.clear();
		newpi.assign(pi.begin(), pi.end());
		c2.newpi.clear();
		c2.newpi.assign(c2.pi.begin(), c2.pi.end());
		return 0;
	}
	else {
		c2.newpi.clear();
		for(int i = 0; i < newpi.size(); i++) {
			c2.newpi.push_back(WireMapC1[newpi[i]]);
		}
	}
/*	
	cout<<"newpi: "<<newpi.size()<<" pi.size: "<<pi.size()<<endl;
	
	cout<<endl<<endl<<"Final pi C1: ";
	for(int i = 0; i < newpi.size(); i++) {
		cout<<allnodevec[newpi[i]].getName()<<" ";
	}
	cout<<endl;
	
	cout<<endl<<endl<<"Final pi C2: ";
	for(int i = 0; i < c2.newpi.size(); i++) {
		cout<<c2.allnodevec[c2.newpi[i]].getName()<<" ";
	}
	cout<<endl;
	*/
	return 1;
	
	
	
}

void Circuit_t::ReplacePi(vector <int> &New, vector <int> &RelateNode){
	
	
	
	for (int i = 0; i < New.size(); i++) {
		newpi.push_back(New[i]);
	}
	
	for (int i = 0; i < RelateNode.size(); i++) {
		
		for (int j = 0; j < newpi.size(); j++) {
			
			if ( RelateNode[i] == newpi[j] ) {
				newpi.erase( (newpi.begin()+j) );
				break;
			}
			
		}
		
	}
	
	return ;
}

bool Circuit_t::IsPiReplace(int node, vector <int> &New, vector <int> &relateNode, vector <int> &NodetoCheck, vector <int> &visit, vector <int> &ready)
{

	
	
	int lastRelateNodeNum = -1, lastNextlevelNodeNum = -1;
	int currentRelateNodeNum , currentNextlevelNodeNum;
	vector <int> nextlevelNode;
	vector<int>::iterator eraseit;
	
	nextlevelNode.clear();
	
	
	relateNode.push_back(node);
	currentRelateNodeNum = 1;
	nextlevelNode.assign(allnodevec[node].out.begin(), allnodevec[node].out.end());
	currentNextlevelNodeNum = nextlevelNode.size();
	
	if (currentNextlevelNodeNum == 0) {
//		cout<<allnodevec[node].getName()<<" is PO"<<endl;
		return 0;
	}
	
	while (lastRelateNodeNum < currentRelateNodeNum || lastNextlevelNodeNum < currentNextlevelNodeNum) {
		lastRelateNodeNum = currentRelateNodeNum;
		lastNextlevelNodeNum = currentNextlevelNodeNum;
	//	cout<<allnodevec[relateNode[0]].getName()<<endl;
	//	cout<<"nextlevelNode : \n";
		for (int i = 0; i < nextlevelNode.size(); i++) {
	//		cout<<allnodevec[nextlevelNode[i]].getName()<<" ";
			relateNode.insert(relateNode.end(), allnodevec[nextlevelNode[i]].in.begin(), allnodevec[nextlevelNode[i]].in.end());
		}
	//	cout<<endl;
		
		sort(relateNode.begin(), relateNode.end());
		eraseit = unique(relateNode.begin(), relateNode.end());
		relateNode.erase(eraseit, relateNode.end());
		
		currentRelateNodeNum = relateNode.size();
		
	//	cout<<"new related node: \n";
		for (int i = 0; i < relateNode.size(); i++) {
	//		cout<<allnodevec[relateNode[i]].getName()<<" ";

			nextlevelNode.insert(nextlevelNode.end(), allnodevec[relateNode[i]].out.begin(), allnodevec[relateNode[i]].out.end());
		}
	//	cout<<endl;
		
		sort(nextlevelNode.begin(), nextlevelNode.end());
		eraseit = unique(nextlevelNode.begin(), nextlevelNode.end());
		nextlevelNode.erase(eraseit, nextlevelNode.end());
		
		currentNextlevelNodeNum = nextlevelNode.size();
		
	}
	
	for (int i = 0; i < relateNode.size(); i++) {
		if (ready[relateNode[i]] == 1) {
			visit[relateNode[i]] = 1;
		}
		else {
	//		cout<<endl<<"not ready node: "<<allnodevec[relateNode[i]].getName()<<" must add in check node and check"<<endl;
		}
	}
	
	for (int i = 0; i < relateNode.size(); i++) {
		if (WireMapC1[relateNode[i]] == -1) {
	//		cout<<"related node : "<<allnodevec[relateNode[i]].getName()<<" not mapped any other!"<<endl;
			return 0;
		}
		else {
		}
	}
	
	
	for (int i = 0; i < nextlevelNode.size(); i++) {
		ready[nextlevelNode[i]] = 1;
		if (WireMapC1[nextlevelNode[i]] == -1) {
	//		cout<<"nextlevelNode node : "<<allnodevec[nextlevelNode[i]].getName()<<" not mapped any other!"<<endl;
			return 0;
		}
		else {
			
		}
	}
	
	New.assign(nextlevelNode.begin(), nextlevelNode.end());
	
//	cout<<endl<<allnodevec[node].getName()<<" done!!"<<endl;
	return 1;
	
}

void Circuit_t::CompareNode(int node1, int node2, Circuit_t &c2)
{
	int match;
	int mismatch = 0;
	if ( (allnodevec[node1].getType() != c2.allnodevec[node2].getType()) || (allnodevec[node1].in.size() != c2.allnodevec[node2].in.size()) ) {
		mismatch = 1;
		return ;
	}    // type or number of fanins are different  == mismatch
	else {
		for (int i = 0; i < allnodevec[node1].in.size(); i++) {
			if (WireMapC1[allnodevec[node1].in[i]] == -1) {
				mismatch = 2;
				return ;  // fanin not match any wire == mismatch
			}
			else {
				match = 0; // inital match
				
				for (int j = 0; j < c2.allnodevec[node2].in.size(); j++) {
					if (WireMapC1[allnodevec[node1].in[i]] == c2.allnodevec[node2].in[j]) {
						match = 1;
						break ;
					}
					else ;
				}
				
				if (match == 0) {
					mismatch = 3;
					return ;  // fanin i of c1 not match any fanin of c2
				}
				else ;
			}
		}
		
		WireMapC1[node1] = node2;
		WireMapC2[node2] = node1;
		compareNumC1.push_back(allnodevec[node1].out.size());
		compareNumC2.push_back(c2.allnodevec[node2].out.size());
		for (int i = 0; i < allnodevec[node1].out.size(); i++) {
			compareC1.push_back(allnodevec[node1].out[i]);
			visitNodC1.push_back(allnodevec[node1].out[i]);
		}
		for (int i = 0; i < c2.allnodevec[node2].out.size(); i++) {
			compareC2.push_back(c2.allnodevec[node2].out[i]);
			visitNodC2.push_back(c2.allnodevec[node2].out[i]);
		}
		
	}

	return ;
}
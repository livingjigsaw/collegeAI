#include<iostream>
#include<fstream>
#include<sstream>
#include"bst.h"
#include"containers.h"
#include"greedy.h"
#include"solvingMethods.h"
using namespace std;

bool parseFile(vector<knapItem*>* outList, int* outCost ){
	string filename;
	cout << "Please enter the filename of the input file\n";
	cin >> filename;
	ifstream infile(filename.c_str());
	//cout << "attempted to open file\n";
	if (infile.is_open())
	{
		string currentLine;
		stringstream ss;
		getline(infile, currentLine);
		ss << currentLine;
		ss >> *outCost;
	
		//cout << "about to get lines\n";
		while(getline(infile, currentLine)){
			//cout << currentLine;
			ss << currentLine;
			char parse;
			string tempName="";
			int tempCost;
			int tempValue;
			ss >> parse;
			while(parse!=','){
				//cout << "parse = " << parse << endl;
				tempName += parse;
				ss >> parse;
			}
			ss >> tempCost;
			ss >> parse;
			ss >> tempValue;
			knapItem* tempItem = new knapItem();
			tempItem->name=tempName;
			tempItem->cost=tempCost;
			tempItem->value=tempValue;
			tempItem->ratio= (double)tempValue / (double)tempCost;
			outList->insert(outList->end(), tempItem);		
		//	cout << "looping\n";
		}
		//cout << "out of parsing\n";
		infile.close();

		return true;
	}
	else{
		return false; 
	}
}

void printList(vector<knapItem*> inList){
	for(int i=0;i<inList.size();i++){
		cout << inList[i]->name << ", ";
		cout << inList[i]->cost << ", ";
		cout << inList[i]->value << ", ";
		cout << inList[i]->ratio << endl;
	}
}



int main(){
	int costLimit;
	vector<knapItem*> itemList; //pointers, as the operators in a vector should already exist for pointers

	BST<set<knapItem*> >* myTree= new BST<set<knapItem*> >(); //by storing a set of pointers in the leaf nodes, the set will still stop identical insertion without having to overload comparison operators, hopefully
	BST<set<knapItem*> >* bruteTree= new BST<set<knapItem*> >(); //by storing a set of pointers in the leaf nodes, the set will still stop identical insertion without having to overload comparison operators, hopefully

	bool success= parseFile(&itemList, &costLimit);
	
	if(!success){
		cout << "Error: a file by the name given was not able to be opened\n";
	}
	else{
		gAnswers greedy;
		greedyAnswers(itemList, costLimit, greedy);
		cout << "Greedy cost soln:\n";
		greedy.cost.print();
		cout << "Greedy value soln:\n";
		greedy.value.print();
		cout << "Greedy ratio soln:\n";
		greedy.ratio.print();
		cout << "Greedy partial soln, the last item was only partially included:\n";
		greedy.partial.print();

		greedy.findBest();
		int depthMeter =0;
		treeData info;
		info.reset();
		info.costLimit=costLimit;
		info.upperBound=greedy.partial.totalValue;
		info.lowerBound=greedy.bestVal;
		info.optims[0]=true;
		info.optims[1]=true;
		for(int i=0;i<itemList.size();i++){
			info.currentPotential += itemList[i]->value; 
		}
		Answer solution;
		solution.reset();
		bool buildSuccess = solveKnap(myTree, &itemList, NULL, info, solution, 0);
		if(!buildSuccess){
			cout << "Error: build failed by trying to overwrite a child";
		}
		else{
		
			cout << endl << "The solveKnap of items:\n";
			solution.print();

			solution.reset();
			info.optims[0]=false;
			info.optims[1]=false;			
			bool buildtwo = solveKnap(bruteTree, &itemList, NULL, info, solution, 0);
			if(!buildtwo){
				cout << "Error: build failed by trying to overwrite a child";
			}
			else{
				cout << endl << "The bruteForce of items:\n";
				solution.print();
			}
			
		}
	}
}
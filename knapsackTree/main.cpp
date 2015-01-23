#include<iostream>
#include<fstream>
#include<sstream>
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
	
	double upperbound, lowerbound;

	BST<set<knapItem*> >* myTree= new BST<set<knapItem*> >(); //by storing a set of pointers in the leaf nodes, the set will still stop identical insertion without having to overload comparison operators, hopefully

	bool success= parseFile(&itemList, &costLimit);
	
	if(!success){
		cout << "Error: a file by the name given was not able to be opened\n";
	}
	else{
		int depthMeter =0;
		bool buildSuccess = buildTree(myTree, &itemList, NULL, depthMeter);
		if(!buildSuccess){
			cout << "Error: build failed by trying to overwrite a child";
		}
		else{
		Answer solution;
		solution.totalCost=0;solution.totalValue=0;
		getAnswer(myTree, myTree->get_root(), costLimit, solution);
		cout << endl << "The best combination of items:\n";
		set<string>::iterator it;
		for(it=solution.itemNames.begin();it!=solution.itemNames.end();it++){
			cout << *it << ", ";
		}
		cout << endl;
		cout << "the cost = " << solution.totalCost << endl; 
		cout << "the value = " << solution.totalValue << endl <<endl; 
		}

		greedyAnswers(itemList, costLimit);
	}
}

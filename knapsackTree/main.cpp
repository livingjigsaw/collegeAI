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

bool handleCommand(string input, BST<set<knapItem*> >* inTree, gAnswers greedy, treeData inData, int costLimit, vector<knapItem*> itemList, Answer solution){
	cout << "\nWhat would you like to do? Type \"help\" to see possible commands\n \n";
	cin >> input;
	int inSwitch = -1; 
	if(input=="exit"){inSwitch=0;}
	else if(input=="help"){inSwitch=1;}
	else if(input=="greedy"){inSwitch=2;}
	else if(input=="brute"){inSwitch=3;}
	else if(input=="lhoptim"){inSwitch=4;}
	else if(input=="rhoptim"){inSwitch=5;}
	else if(input=="bothoptim"){inSwitch=6;}
	else if(input=="newfile"){inSwitch=7;}
	else{inSwitch=8;}
	switch (inSwitch){
		case 0:
			return true; // if program returns true, we leave command loop, false will return if end of code is reached
			break;
		case 1:
		{
			cout << "Accepted commands are:\n";
			cout << "help - shows list of valid commands \n";
			cout << "greedy - shows solutions from all of the greedy algorithms\n";
			cout << "brute - performs the exhaustive search for the optimal \n";
			cout << "lhoptim - performs search with the lower bound optimization \n";
			cout << "rhoptim - performs search with the cost limit optimization \n";
			cout << "bothoptim - performs search using both optimizations \n";
			cout << "newfile - read in a different data file \n";
			cout << "exit - ends the program \n";
			break;
		}
		case 2:
		{
			cout << "Greedy cost soln:\n";
			greedy.cost.print(0); //0 for false, to tell the solution not to print number of nodes
			cout << "Greedy value soln:\n";
			greedy.value.print(0);
			cout << "Greedy ratio soln:\n";
			greedy.ratio.print(0);
			cout << "Greedy partial soln:\n";
			greedy.partial.print(0);
			break;
		}
		case 3:
		{
			inData.optims[0]=false;
			inData.optims[1]=false;
			solution.reset();
			bool buildSuccess = solveKnap(inTree, &itemList, NULL, inData, solution, 0);
			if(!buildSuccess){
				cout << "Error: tree failed to build";
			}
			else{
				cout << endl << "The bruteforce solution:\n";
				solution.print(1);
			}
			inTree->rebuild();
			break;
		}
		case 4:
		{
			inData.optims[0]=true;
			inData.optims[1]=false;
			solution.reset();
			bool buildSuccess = solveKnap(inTree, &itemList, NULL, inData, solution, 0);
			if(!buildSuccess){
				cout << "Error: tree failed to build";
			}
			else{
				cout << endl << "The solution with the lower bound optimization:\n";
				solution.print(1);
			}
			inTree->rebuild();
			break;
		}
		case 5:
		{
			inData.optims[0]=false;
			inData.optims[1]=true;
			solution.reset();
			bool buildSuccess = solveKnap(inTree, &itemList, NULL, inData, solution, 0);
			if(!buildSuccess){
				cout << "Error: tree failed to build";
			}
			else{
				cout << endl << "The solution with the costLimit optimization:\n";
				solution.print(1);
			}
			inTree->rebuild();
			break;
		}
		case 6:
		{
			inData.optims[0]=true;
			inData.optims[1]=true;
			Answer solution;	
			solution.reset();
			bool buildSuccess = solveKnap(inTree, &itemList, NULL, inData, solution, 0);
			if(!buildSuccess){
				cout << "Error: tree failed to build";
			}
			else{
				cout << endl << "The solution with both optimizations:\n";
				solution.print(1);
			}
			inTree->rebuild();
			break;
		}
		case 7:
		{
			bool success=parseFile(&itemList, &costLimit);
			while(!success){ 	
				cout << "Sorry, a file by that name was not found\n";
				success=parseFile(&itemList, &costLimit);		
			}
			break;
		}
		default:
			cout << "sorry, that command is not valid. please try again\n";
			break;
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

	bool success=parseFile(&itemList, &costLimit);
	while(!success){ 	
		cout << "Sorry, a file by that name was not found\n";
		success=parseFile(&itemList, &costLimit);		
	}
	//execute greedy algs here, commands will simply ask for solution
	gAnswers greedy;
	greedyAnswers(itemList, costLimit, greedy);
	greedy.findBest();

	//setup treeData
	int depthMeter =0;
	treeData info;
	info.reset();
	info.costLimit=costLimit;
	info.upperBound=greedy.partial.totalValue;
	info.lowerBound=greedy.bestVal;
	for(int i=0;i<itemList.size();i++){
		info.currentPotential += itemList[i]->value; 
	}
	Answer solution;
	solution.reset();	
	bool isExit=false;
	string command="";
	while(!isExit){
		isExit=handleCommand(command, myTree, greedy, info, costLimit, itemList, solution);
	}
			
	
}
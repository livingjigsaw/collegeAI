#include<iostream>
#include<fstream>
#include<set>
#include<vector>
#include<sstream>
#include"bst.h"
using namespace std;

struct knapItem{
	string name;
	int cost;
	int value;
	double ratio;
};

struct Answer{
	double totalCost;
	double totalValue;
	set<string> itemNames;

	void reset(){
		totalCost = 0;
		totalValue = 0;
		itemNames.clear();
	}

	void print(){
		set<string>::iterator it;
		cout<< "items in answer:\n";
		for(it=itemNames.begin();it!=itemNames.end();it++){
			cout << *it << ", ";
		}
		cout << endl;
		cout << "the cost = " << totalCost << endl; 
		cout << "the value = " << totalValue << endl <<endl; 
	}
};

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

void sortCost(vector<knapItem*>& costsort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts LOW to HIGH
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(costsort[current]->cost < costsort[pivot]->cost){
			//swap wall and current values
			temp=costsort[wall];
			costsort[wall]=costsort[current];
			costsort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=costsort[wall];
	costsort[wall]=costsort[pivot];
	costsort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortCost(costsort, leftBound, wall-1);
	sortCost(costsort, wall+1, rightBound);
}

void sortValue(vector<knapItem*>& valuesort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts HIGH to LOW
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(valuesort[current]->value > valuesort[pivot]->value){
			//swap wall and current values
			temp=valuesort[wall];
			valuesort[wall]=valuesort[current];
			valuesort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=valuesort[wall];
	valuesort[wall]=valuesort[pivot];
	valuesort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortValue(valuesort, leftBound, wall-1);
	sortValue(valuesort, wall+1, rightBound);
}

void sortRatio(vector<knapItem*>& ratiosort, int leftBound, int rightBound){ //recursive, call with 0 and size - 1 the first time
	//NOTE: sorts HIGH to LOW
	if(leftBound>=rightBound){
		return; // single length array, trivially sorted
	}

	int wall=leftBound; //this is the first element to the right of pivot
	int pivot=rightBound; //pick rightmost as pivot
	int current=leftBound;
	knapItem* temp; //for swapping
	while(current!=pivot){
		if(ratiosort[current]->ratio > ratiosort[pivot]->ratio){
			//swap wall and current ratios
			temp=ratiosort[wall];
			ratiosort[wall]=ratiosort[current];
			ratiosort[current]=temp;
			//wall moves right
			wall++;
		}
		current++;
	}
	//now swap pivot and wall
	temp=ratiosort[wall];
	ratiosort[wall]=ratiosort[pivot];
	ratiosort[pivot]=temp;
	//val at wall is now pivot and in the correct place
	//recursively call on subarrays
	sortRatio(ratiosort, leftBound, wall-1);
	sortRatio(ratiosort, wall+1, rightBound);
}


//tree is built here to allow non-standard tree creation, as our parameters are not standard
//depth dictates which item from vector to insert into next layer
// always call from main with depth=0
bool buildTree(BST<set<knapItem*> >* outTree, vector<knapItem*>* inList, Node<set<knapItem*> >* current, int depth){	
	// need to find answer here as well, so that the tree need not be traversed again
	if(depth>=inList->size()){
		return true; 	//base case
	}
	else{
		//if root, current needs to be set
		if(depth==0){
			current = outTree->get_root();
		}
		//use current set for left node. exact same set, why make new ones?
		//if root is current, it should already have a new set from constructor
		depth++;
		bool leftSuccess=outTree->addLeftNode(current, current->get_data() );
		if(!leftSuccess){
			return false;
		}
		buildTree(outTree, inList, current->get_lchild(), depth);
		//copy current set into new set, then add item into that new set based on depth for right node
		set<knapItem*>* tempSet = new set<knapItem*>;
		*tempSet = current->get_data();
		tempSet->insert( (*inList)[depth-1] );
		bool rightSuccess = outTree->addRightNode(current, *tempSet);
		if(!rightSuccess){
			return false;
		}
		buildTree(outTree, inList, current->get_rchild(), depth);

	}
}

void getAnswer(BST<set<knapItem*> >* outTree,  Node<set<knapItem*> >* current, int costLimit, Answer& currentSoln){ //recursive, call with current as root
	if(current->get_lchild()==NULL && current->get_rchild()==NULL){ // i'm in a leaf node

		set<knapItem*>::iterator it;
		int costSum=0, valSum=0;
		for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
			costSum+=(*it)->cost;
			valSum+=(*it)->value;
		}
		if(costSum <= costLimit && valSum > currentSoln.totalValue){
			currentSoln.itemNames.clear();
			currentSoln.totalCost=costSum;
			currentSoln.totalValue=valSum;
			for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
				currentSoln.itemNames.insert( (*it)->name );
			}
		}
	}
	else{
		getAnswer(outTree, current->get_lchild(), costLimit, currentSoln);
		getAnswer(outTree, current->get_rchild(), costLimit, currentSoln);
	}
}

void greedyAnswers(vector<knapItem*> original, int costLimit){
	vector<knapItem*> costList = original;
	vector<knapItem*> valList = original;
	vector<knapItem*> ratList = original; //to be sorted variations of the original list

	//cout << "now sorting\n";

	sortCost(costList, 0, costList.size()-1);
	sortValue(valList, 0, valList.size()-1);
	sortRatio(ratList, 0, ratList.size()-1);

	Answer sol;	
	sol.reset();
	//cost greedy
	for(int i=0;i<costList.size();i++){
		if(sol.totalCost+costList[i]->cost <=costLimit){
			sol.totalCost+=costList[i]->cost;
			sol.totalValue+=costList[i]->value;
			sol.itemNames.insert(costList[i]->name);
		}
	}
	cout << "Greedy cost soln:\n";
	sol.print();
	// clear solution
	sol.reset();
	//value greedy
	for(int i=0;i<valList.size();i++){
		if(sol.totalCost+valList[i]->cost <=costLimit){
			sol.totalCost+=valList[i]->cost;
			sol.totalValue+=valList[i]->value;
			sol.itemNames.insert(valList[i]->name);
		}
	}
	cout << "Greedy val soln\n";
	sol.print();
	// clear solution
	sol.reset();
	//ratio greedy
	for(int i=0;i<ratList.size();i++){
		if(sol.totalCost+ratList[i]->cost <=costLimit){
			sol.totalCost+=ratList[i]->cost;
			sol.totalValue+=ratList[i]->value;
			sol.itemNames.insert(ratList[i]->name);
		}
	}
	cout << "Greedy ratio soln:\n";
	sol.print(); 
	// clear solution
	sol.reset();
	//partial knapsack
	for(int i=0;i<ratList.size();i++){
		if(sol.totalCost==costLimit){
			break;
		}
		else if(sol.totalCost+ratList[i]->cost <= costLimit){
			sol.totalCost+=ratList[i]->cost;
			sol.totalValue+=ratList[i]->value;
			sol.itemNames.insert(ratList[i]->name);
		}
		else if(sol.totalCost+ratList[i]->cost > costLimit){
			double remains = costLimit - sol.totalCost;
			sol.totalValue+= remains * (double)ratList[i]->ratio;

			sol.totalCost+=remains;
			cout << "item "<< ratList[i]->name << " was partially included\n";
		}
	}
	cout << "upper bound from partial knapsack:\n";
	sol.print(); 
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

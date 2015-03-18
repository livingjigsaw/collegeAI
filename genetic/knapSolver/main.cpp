#include<iostream>
#include<fstream>
#include<sstream>
#include"containers.h"
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
			outList->insert(outList->end(), tempItem);	
			ss.clear();	
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


int main(){
	int costLimit;
	int fitCount;
	vector<knapItem*> itemList;

	bool success=parseFile(&itemList, &costLimit);
	while(!success){ 	
		cout << "Sorry, a file by that name was not found\n";
		success=parseFile(&itemList, &costLimit);		
	}
		
	srand(time(NULL));	//initialize the seed for pseudo-randomness
	cout << "what size do you want the population to be?\n";
	int popSize;
	cin >> popSize;
	Population genitor(popSize);
	genitor.set_costLimit(costLimit);
	genitor.set_itemList(itemList);
	genitor.randInit();
	genitor.genitor();
}

#include"bst.h"
#include<set>
#include<vector>
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


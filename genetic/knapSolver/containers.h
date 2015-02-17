#include<set>
#include<vector>
using namespace std;

struct knapItem{
	string name;
	int cost;
	int value;
	//double ratio; no need, since greedy alds are not planned on being used
};

struct Answer{
	double totalCost;
	double totalValue;
	set<string> itemNames;

	void reset(){
		totalCost = 0;
		totalValue = 0;
		nodeCount = 0;
		itemNames.clear();
	}

	void print(bool nodes){
		set<string>::iterator it;
		cout<< "items in answer:\n";
		for(it=itemNames.begin();it!=itemNames.end();it++){
			cout << *it << ", ";
		}
		cout << endl;
		cout << "the cost = " << totalCost << endl; 
		cout << "the value = " << totalValue << endl <<endl; 
		if(nodes){
			cout << "the number of nodes = " << nodeCount << endl <<endl; 	
		}
		
	}
};

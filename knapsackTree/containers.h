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
	long long int nodeCount;

	void reset(){
		totalCost = 0;
		totalValue = 0;
		nodeCount = 0;
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
		cout << "the number of nodes = " << nodeCount << endl <<endl; 
	}
};

struct treeData{ // the data that guides tree creation
	int costLimit;
	int upperBound; //from greedy algs
	int lowerBound; //from greedy algs
	int currentPotential; // for tracking what's left with a lhoptim
	bool optims[2]; //array locations: 0 =lh opt, 1 = rh opt

	void reset(){
		costLimit=0;
		lowerBound=0;
		upperBound=0;
		currentPotential=0;
		for(int i=0;i<2;i++){
			optims[i]=false;
		}
	}
};

struct gAnswers{
	Answer cost;
	Answer value;
	Answer ratio;
	Answer partial;

	int bestVal;

	void findBest(){
		bestVal = cost.totalValue;
		if(bestVal < value.totalValue){
			bestVal=value.totalValue;
		}
		if(bestVal < ratio.totalValue){
			bestVal = ratio.totalValue;
		}
	}
};


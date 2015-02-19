#include<set>
#include<vector>
#include<cstdlib>	//need rand and srand functions
#include<ctime>		//time function used to initialize seed in main
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

class Candidate{
	private:
		//members
		double fitness;
		vector<bool> sack;
	
	public:
		//constructors/destructors
		Candidate(){};	//so the array isn't created for child generation
		Candidate(int inSize){
			for(int i=0;i<inSize;i++){
				sack.insert(sack.end(), rand()%2);
			}
		};

		~Candidate(){};

		//methods
		int get_sackSize(){return sack.size();};
		int get_item(int index){return sack[index];};
		double get_fit(){return fitness;};

		void set_fit(double inFit){fitness=inFit;};
		void print(){
			for(int i=0;i<sack.size();i++){
				cout<<sack[i];
			}
		};
		//void becomeChild(){}; //weird, but it will take two parents and crossover self
};

class Population{
	private:
		vector<Candidate*> current;
	public:
		Population(){};
		Population(int inSize){
			for(int i=0;i<inSize;i++){ 
				current.insert(current.end(), NULL);
			}
		};
		~Population(){};

		void insertMem(Candidate* inCan){
			for(vector<Candidate*>::iterator it=current.begin();it<current.end();it++){
				if( (*it)==NULL || inCan->get_fit()> (*it)->get_fit()){
					current.insert( it , inCan);
					delete *current.end();
					current.pop_back();
					return;
				}
			}
			delete inCan;
		};

		void print(){
			for(int i=0;i<current.size();i++){ 
				cout << current[i]->get_fit() << endl;
			}
		};

		void printMember(int index){
			current[index]->print();
		};
		double getMemberFit(int index){
			return current[index]->get_fit();
		};
};

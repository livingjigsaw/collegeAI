#include<set>
#include<vector>
#include<cstdlib>	//need rand and srand functions
#include<ctime>		//time function used to initialize seed in main
#include<cmath>		//for the parent selection process
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
		void insert(bool in){sack.insert(sack.end(), in);};

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
		int costLimit;
		vector<Candidate*> current;
		vector<knapItem*> itemList;
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

		void set_itemList(vector<knapItem*> inList){
			itemList=inList;
		};

		void set_costLimit(int inLimit){costLimit=inLimit;};

		void fitEval(Candidate* inCan){
			Answer temp;
			temp.reset();
			for(int i=0;i<inCan->get_sackSize();i++){
				if(inCan->get_item(i)){
					temp.totalCost+=itemList[i]->cost;
					temp.totalValue+=itemList[i]->value;
					temp.itemNames.insert(itemList[i]->name);
				}
			}
			if(temp.totalCost>costLimit){
				inCan->set_fit(0);
			}
			else{
				double fitness=temp.totalValue;
				inCan->set_fit(fitness);		//may do more math to fitness, not decided yet
			}
		}

		void randInit(){
			for(int i=0;i<current.size();i++){
				Candidate* soln= new Candidate(itemList.size());
				fitEval(soln);
				insertMem(soln);
			}
		};

		void printAllFit(){
			for(int i=0;i<current.size();i++){ 
				cout << current[i]->get_fit() << endl;
			}
		};

		void printAll(){
			for(int i=0;i<current.size();i++){ 
				cout << "Rank " << i+1 << " = ";
				current[i]->print();
				cout << " fitness = " << current[i]->get_fit() << endl;
			}
		};

		void printMember(int index){
			current[index]->print();
		};
		double getMemberFit(int index){
			return current[index]->get_fit();
		};

		void createChild(){
			int maxChance = (double)(current.size()-1)*(double)(current.size())/2; //equivalent to the sum of 1 to 100, but shifted to be the sum of 1 to 99 (trivially 0 to 99)
			double p[2] = {0};
			//select the parents
			p[0]=(99 - floor(sqrt(2*(rand()%maxChance))));
			p[1]=(99 - floor(sqrt(2*(rand()%maxChance))));
			cout << "parent Cand 1 = " << p[0] << endl;
			cout << "parent Cand 2 = " << p[1] << endl;
			while(p[0]==p[1]){ 			//this should only fire if the same parent was chosen twice
			p[1]=(99 - floor(sqrt(2*(rand()%maxChance))));
				cout << "member " << p[0] << " was picked again!\n";
				cout << "parent Cand 2 = " << p[1] << endl;
			}
			cout << "\nParent 1 = ";
			current[p[0]]->print();
			cout << " fitness = " << current[p[0]]->get_fit() << endl;

			cout << "Parent 2 = ";
			current[p[1]]->print();
			cout << " fitness = " << current[p[1]]->get_fit() << endl;

			/*
			breakdown of the math:
			let n be an integer denoting a possible parent position in the vector
			let X be the sum of 1 to n
			X = (n/2)*(n+1), which is maxchance if n= vector size
			after generating a random n no larger than size, we solve for X:
			n^2 + n = X*2
			using the quadratic formula:
			-1 (+ or -) sqrt(1-4*1*(-X*2))/2
			a and b will always be 1, so let's approximate by removing the ones to simplify
			sqrt(-X*2) the four can come out of the sqrt and cancel the denominator
			this ranges to above the desired range, so the floor keeps the approximation in bounds
			therefore, the range of numbers that equate to larger numbers should be higher, and we simply have to additively inverse it with 100
			*/

			//create child
			Candidate* child= new Candidate;
			int tempSackSize = current[ p[0] ]->get_sackSize();
			for(int i=0; i<tempSackSize;i++){
				int parent = rand()%2;
				child->insert(current[p[parent]]->get_item(i));
			}
			//eval fitness
			fitEval(child);
			cout << "Child = ";
			child->print();
			cout << " fitness = " << child->get_fit() << endl << endl;
			//insert child into population
			insertMem(child);
		}
};

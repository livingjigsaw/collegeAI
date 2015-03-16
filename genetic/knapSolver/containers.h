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
		Candidate(){}	//so the array isn't created for child generation
		Candidate(int inSize){
			for(int i=0;i<inSize;i++){
				sack.insert(sack.end(), rand()%2);
			}
		}

		~Candidate(){}

		//methods
		int get_sackSize(){return sack.size();}
		bool get_item(int index){return sack[index];}
		double get_fit(){return fitness;}
		void insert(bool in){sack.insert(sack.end(), in);}

		void set_fit(double inFit){fitness=inFit;}
		void reset(){sack.clear();fitness=0;}
		void print(){
			for(int i=0;i<sack.size();i++){
				cout<<sack[i];
			}
			cout<< "  Fitness = " << fitness << endl;
		}

		void mutate(){
			int loc = rand()%sack.size();
			if(sack[loc]){
				sack[loc]=false;
			}
			else{
				sack[loc]=true;
			}
		}

		void operator=(Candidate &rhs){
			fitness=rhs.get_fit();
			sack.clear();
			for(int i=0;i<rhs.get_sackSize();i++){
				insert(rhs.get_item(i));
			}
		}

		bool operator ==(Candidate &rhs){
			for(int i=0;i<sack.size();i++){
				if(sack[i]!=rhs.get_item(i)){
					return false;
				}
			}
			return true;
		}
		bool operator !=(Candidate &rhs){
			for(int i=0;i<sack.size();i++){
				if(sack[i]!=rhs.get_item(i)){
					return true;
				}
			}
			return false;
		}
	};
		

class Population{
	private:
		int costLimit;
		vector<Candidate*> current;
		vector<knapItem*> itemList;
		int evalCount;
		int hmgCount;
		Candidate* hmgCan; 	//to know which candidate was the homogeneous candidtate from before
	public:
		Population(){hmgCount=0;hmgCan=NULL;evalCount=0;}
		Population(int inSize){
			hmgCount=0;hmgCan=NULL;evalCount=0;
			for(int i=0;i<inSize;i++){ 
				current.insert(current.end(), NULL);
			}
		}
		~Population(){}

		void insertMem(Candidate* inCan){
			for(vector<Candidate*>::iterator it=current.begin();it<current.end();it++){
				if( (*it)==NULL || inCan->get_fit()> (*it)->get_fit() || (inCan->get_fit()==(*it)->get_fit() && *inCan == *(*it) )  ){
					current.insert( it , inCan);
					if((current.back())!=NULL){
						delete current.back();
					}
					current.pop_back();
					return;
				}
			}
			delete inCan;
		}

		void set_itemList(vector<knapItem*> inList){
			itemList=inList;
		}

		void set_costLimit(int inLimit){costLimit=inLimit;}
		int get_costLimit(){return costLimit;}
		void set_hmgCount(int inCount){hmgCount=inCount;}
		int get_hmgCount(){return hmgCount;}
		void inc_hmgCount(){hmgCount++;}



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
			evalCount++;
		}

		void randInit(){
			for(int i=0;i<current.size();i++){
				Candidate* soln=NULL;
				double curFit=0;
				while(curFit==0){
					delete soln;
					soln= new Candidate(itemList.size());
					fitEval(soln);
					curFit=soln->get_fit();
				}
				insertMem(soln);
			}
		}

		void printAll(){
			for(int i=0;i<current.size();i++){ 
				cout << "Rank " << i+1 << " = ";
				current[i]->print();
				cout << " fitness = " << current[i]->get_fit() << endl;
			}
		}

		void printMember(int index){
			current[index]->print();
		}
		double getMemberFit(int index){
			return current[index]->get_fit();
		}

		void createChild(int reps){ //I want to have a higher chance of picking the higher numbers as time progresses, to eliminate competing strains
			int maxChance = (double)(current.size()-1)*(double)(current.size())/2; //equivalent to the sum of 1 to 100, but shifted to be the sum of 1 to 99 (trivially 0 to 99)
			double p[2] = {0};
			double curFit=0;
			Candidate* child= new Candidate;
			//select the parents
			while(curFit==0){
				child->reset();
			if(reps<10000){
				p[0]=((current.size()-1) - floor(sqrt(2*(rand()%maxChance))));
				p[1]=((current.size()-1) - floor(sqrt(2*(rand()%maxChance))));

				//cout << "parent Cand 1 = " << p[0] << endl;
				//cout << "parent Cand 2 = " << p[1] << endl;
				while(p[0]==p[1]){ 			//this should only fire if the same parent was chosen twice
				p[1]=((current.size()-1) - floor(sqrt(2*(rand()%maxChance))));
					//cout << "member " << p[0] << " was picked again!\n";
					//cout << "parent Cand 2 = " << p[1] << endl;
				}
				//cout << "\nParent 1 = ";
				//current[p[0]]->print();
				//cout << " fitness = " << current[p[0]]->get_fit() << endl;

				//cout << "Parent 2 = ";
				//current[p[1]]->print();
				//cout << " fitness = " << current[p[1]]->get_fit() << endl;

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
			}
			else{
				p[0]=0;
				p[1]=0;
			}
			//create child
			int tempSackSize = current[ p[0] ]->get_sackSize();
			for(int i=0; i<tempSackSize;i++){
				int parent = i%2;
				child->insert(current[p[parent]]->get_item(i));
			}
			//possibility to mutate
			if(rand()%100 < 5){
				child->mutate();
			}
			//eval fitness
			fitEval(child);
			curFit=child->get_fit();
		}
			//cout << "Child = ";
			//child->print();
			//cout << " fitness = " << child->get_fit() << endl << endl;
			//insert child into population
			insertMem(child);
		}

		bool hmgFit(){
			int currentFit=current[0]->get_fit();
			for(int i=0;i<current.size();i++){ 
				if(currentFit!=current[i]->get_fit()){
					return false;
				}
			}
			return true; 
		}

		bool isHmg(){
			Candidate tempCan;
			if (!hmgFit()){
				return false;
			}
			else{
				tempCan= *(current[0]);
				for(int i=0;i<current.size();i++){
					if(tempCan != *(current[i]) ){
						return false;
					}
				}
				return true;
			}
		}

		void cataMutation(){
			int popSize = current.size();
			int pctchange = (current[0]->get_sackSize())/2;
			for(int i=popSize-1;i>0;i--){	//leave one intact
				delete current[i];
				current[i]=NULL;
			}
			for(int i=1;i<popSize;i++){	//leave one intact
				Candidate* tempCan = new Candidate();
				*tempCan = *current[0];	
				for(int j=0;j<pctchange;j++){
					tempCan->mutate();
				}
				fitEval(tempCan);
				insertMem(tempCan);
			}
		}

		void genitor(){
			int catams=0, totalChildren=0;
			while(get_hmgCount()<3){
				int reps=0;
				while(!isHmg()){
					createChild(reps);
					reps++;
				}
				//cout << "isHmg\n";
				totalChildren +=reps;
				if( hmgCan==NULL || *hmgCan != *current[0] ){
					set_hmgCount(1);
					hmgCan=new Candidate();
					*hmgCan = *current[0];
				}
				else{
					inc_hmgCount();
				}
				cataMutation();
				catams++;
				//cout << "new generation\n";
			}
			cout << evalCount << " fitness evaluations performed\n";
			cout << catams << " Catacalysmic Mutations\n";
			cout << "Best Candidate:\n";
			current[0]->print();
		}
};

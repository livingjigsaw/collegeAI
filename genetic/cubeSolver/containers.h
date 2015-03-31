#include<set>
#include<vector>
#include<cstdlib>	//need rand and srand functions
#include<ctime>		//time function used to initialize seed in main
#include<cmath>		//for the parent selection process
using namespace std;

class coord{
	public:
		int x,y,z;

		coord(){
			x=0;y=0;z=0;
		}

		void randInit(bool isRot){	
		/*
		this gets tricky. there are 24 discrete orientations, but if i let all three rotations
		be random, there would be 64 possibilities, which would weight certain orientations
		more than others, hindering this random search. Also, order matters with rotation.
		To address these issues, rotations will always be applied in the order of x,y,then z,
		and the 24 orientations will each only have 1 appearance, weighting each equally.
		the following rand init should follow the above premesis when the bool is 1. 
		otherwise, all three are just random (for location)

		rotations are 90 degree rotations along the given axis. x = rotation about the x axis
		vector math will be in fitness evaluation
		*/
			x=rand()%4;
			y=rand()%4;
			if(isRot){	//this is initializing a rotation coord
				if(y==0){
					z=rand()%3;
					if(z==2){ //z should only ever be 0,1, or 3
						z=3;
					}
				else
					z=0;
				}
			}
			else{	//this initializes a location coord
				z=rand()%4;
			}
		
		}

		void operator=(coord rhs){
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}

		bool operator==(coord rhs){
			if(x == rhs.x && y == rhs.y && z == rhs.z){
				return true;
			}
			return false;
		}

		bool operator!=(coord rhs){
			if(x == rhs.x && y == rhs.y && z == rhs.z){
				return false;
			}
			return true;
		}

		void print(){
			cout <<x <<","<<y<<","<<z;
		}

		void rotateX(){
			y= -1*z;
			z= y;
		}		
		void rotateY(){
			x= z;
			z= -1*x;
		}
		void rotateZ(){
			x= -1*y;
			y= x;
		}
};

struct piece{
	string name;
	vector<coord> baseConfig;	//this represents the smaller cubes that make one piece
};

class Candidate{
	private:
		//members
		double fitness;
		vector<coord> baseStart; 	// the coord where the piece begins
		vector<coord> rotations;	// in rotations, the values are rotations around said axis. <1,0,0> ir one 90 degree rotation about the x axis 
		// also, the order in rots determines which piece it is referring
	public:
		//constructors/destructors
		Candidate(){}	//so the array isn't created for child generation
		Candidate(int inSize){	//num of pieces in puzzle
			for(int i=0;i<inSize;i++){
				coord tBase, tRots; // temp base location and rotations
				tBase.randInit(0);
				tRots.randInit(1);
				baseStart.push_back(tBase);
				rotations.push_back(tRots);
			}
		}

		~Candidate(){}

		//methods
		coord get_pieceBase(int index){return baseStart[index];}
		coord get_pieceRot(int index){return rotations[index];}
		int get_canSize(){return baseStart.size();}
		double get_fit(){return fitness;}
		void insert(coord inBase, coord inRot){baseStart.push_back(inBase); rotations.push_back(inRot);}

		void set_fit(double inFit){fitness=inFit;}
		void reset(){baseStart.clear();rotations.clear();fitness=0;}
		void print(){
			for(int i=0;i<baseStart.size();i++){
				cout << "B=";
				baseStart[i].print();
				cout <<"  R=";
				rotations[i].print();
				cout << endl;
			}
			cout<< "  Fitness = " << fitness << endl;
		}

		void mutate(){
			int loc = rand()%baseStart.size();
			baseStart[loc].randInit(0);
			rotations[loc].randInit(1);
		}

		void operator=(Candidate &rhs){
			fitness=rhs.get_fit();
			baseStart.clear();
			rotations.clear();
			for(int i=0;i<rhs.get_canSize();i++){
				insert(rhs.get_pieceBase(i),rhs.get_pieceRot(i));
			}
		}

		bool operator ==(Candidate &rhs){
			for(int i=0;i<rhs.get_canSize();i++){
				if(get_pieceBase(i)!=rhs.get_pieceBase(i) || get_pieceRot(i)!=rhs.get_pieceRot(i)){
					return false;
				}
			}
			return true;
		}

		bool operator !=(Candidate &rhs){
			for(int i=0;i<rhs.get_canSize();i++){
				if(get_pieceBase(i)!=rhs.get_pieceBase(i) || get_pieceRot(i)!=rhs.get_pieceRot(i)){
					return true;
				}
			}
			return false;
		}
	};
		

class Population{
	private:
		vector<Candidate*> current; //aka population
		vector<piece*> pieces;
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

		void set_pieces(vector<piece*> inPieces){
			pieces=inPieces;
		}

		void set_hmgCount(int inCount){hmgCount=inCount;}
		int get_hmgCount(){return hmgCount;}
		void inc_hmgCount(){hmgCount++;}



		void fitEval(Candidate* inCan){		
			bool theCube[4][4][4]={{{0}}};	//x, y, z
			int length=inCan->get_canSize();
			for(int i=0; i<length; i++){
				piece temp; coord tRot, tBase; //temp storage for rotations and base location
				temp.baseConfig=pieces[i]->baseConfig;
				tBase=inCan->get_pieceBase(i);
				tRot=inCan->get_pieceRot(i);
				//apply rotations to base config

				int pieceLen=temp.baseConfig.size(); //every sub-cube of the piece needs to be rotated, for each of the three dimensions, the number of Rots times
				for(int k=0;k<pieceLen;k++){
					for(int j=0;j<tRot.x;j++){
						temp.baseConfig[k].rotateX();	
					}
					for(int j=0;j<tRot.y;j++){
						temp.baseConfig[k].rotateY();
					}
					for(int j=0;j<tRot.z;j++){
						temp.baseConfig[k].rotateZ();
					}
					//apply relative coord from base location to piece
					temp.baseConfig[k].x += tBase.x;
					temp.baseConfig[k].y += tBase.y;
					temp.baseConfig[k].z += tBase.z;
					//make sure we don't leave the bounds of the cube
					if (temp.baseConfig[k].x >=0 && temp.baseConfig[k].x < 4 && temp.baseConfig[k].y >=0 && temp.baseConfig[k].y < 4 &&temp.baseConfig[k].z >=0 && temp.baseConfig[k].z < 4){
						theCube[temp.baseConfig[k].x][temp.baseConfig[k].y][temp.baseConfig[k].z]=1;
					}
				}

			}
			double fitness=0;
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					for(int k=0;k<4;k++){
						if(theCube[i][j][k]==1){
							fitness+=1;
						}
					}
				}
			}
			inCan->set_fit(fitness);
			//cout << fitness << endl;
			evalCount++;
		}

		void randInit(){
			for(int i=0;i<current.size();i++){
				Candidate* soln=NULL;
				double curFit=0;
				while(curFit==0){
					delete soln;
					soln= new Candidate(pieces.size());
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
			if(reps<current.size()*100){ //sloppy, yet efficient way to avoid saturationg the population with the second-best solution
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
			int tempCanSize = current[ p[0] ]->get_canSize();
			int tippingPoint = rand()%tempCanSize;
			for(int i=0; i<tempCanSize;i++){
				int parent = 0;
				if(i>tippingPoint){parent=1;}
				child->insert(current[p[parent]]->get_pieceBase(i), current[p[parent]]->get_pieceRot(i));
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
			int pctchange = (current[0]->get_canSize())/2;
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
			while(current[0]->get_fit() < 64){
			//while(hmgCount < 3){
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
				cout << "fit = " << current[0]->get_fit() << " new generation\n";

			}
			cout << evalCount << " fitness evaluations performed\n";
			cout << catams << " Catacalysmic Mutations\n";
			cout << "Best Candidate:\n";
			current[0]->print();
		}
};

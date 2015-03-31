#include <iostream>
#include <ctime>		//time function used to initialize seed in main
#include <cstdlib>	//need rand and srand functions
using namespace std;

class coord{
public:
	int x,y,z;

	coord(){
		x=0;y=0;z=0;
	}

	void randInit(){
		x=rand()%4;
		y=rand()%4;
		z=rand()%4;
	}
};
int main(){
	srand(time(NULL));	//initialize the seed for pseudo-randomness

coord first;
cout << first.x <<" " << first.y <<" " << first.z <<"\n";
first.randInit();
cout << first.x <<" " << first.y <<" " << first.z <<"\n";

}
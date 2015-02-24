#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
using namespace std;

int main(){
	srand(time(NULL));

	int maxChance = (double)(100-1)/2*(double)(100); //equivalent to the sum of 1 to 100, but shifted to be the sum of 1 to 99 (trivially 0 to 99)
	cout << maxChance << endl << endl;
	int check=0;
	while(!(check!=check)){
		check = floor(sqrt(2*(rand()%maxChance)));
	}
	cout << "you found a nan!\n";
	
}
#include<iostream>
#include<set>
using namespace std;

int main(){
	set<int> mySet;
	set<int>::iterator it;
	mySet.insert(1);
	mySet.insert(2);
	mySet.insert(3);
	mySet.insert(4);
	mySet.insert(5);
	for(it=mySet.begin();it!=mySet.end();it++){
		cout << *it << "\n";
	}
}
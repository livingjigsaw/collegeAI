#include <iostream>
#include <cstdlib>	
#include <ctime>
#include "ttt.h"	
using namespace std;

int main(){
	srand(time(NULL));
	cout << "TicTacToe ShouseBot\n\n";
	char inChar;
	string inString;
	GameState thisGame;
	//determine if we will load bot training from previous training interval
	cout << "Would you like to load previous training data?(y/n)\n";
	cin >> inChar;
	if(inChar=='y'){
		cout << "Please enter the filename of the data file.\n";
		cin >> inString;
		//attempt fileload here
	}
	//determine if bot vs bot or bot vs input and who goes first.
	bool keepGoing=true;
	while(keepGoing){
		cout << "Enter \"train\" to execute a training interval, or \"play\" to play against me\n";
		cout << "You can also type \"exit\" to stop the program, but then I'll miss you.\n";	//creepy lolz 
		cin >> inString;
		if(inString=="train"){ 	//set training interval, or number of games to be run between prompts
			int trainInterval=0;
			cout << "How many training games should I play?\n";
			cin >>trainInterval;
			for(int i=0;i<trainInterval;i++){	//run training game x times according to interval
				thisGame.playGame(1,1,1);
			}
			cout << "Training complete! would you like to see my saturation levels?(y/n)\n";
			cin >> inChar;
			if(inChar=='y'){
				thisGame.get_bot()->printTraining();
			}
		}
		else if(inString=="play"){
			cout << "You're on! Which player will you be?(x/o)\n";
			cin >> inChar;
			if(inChar=='x'){
				thisGame.playGame(0,1,0);
			}
			else if(inChar=='o'){
				thisGame.playGame(1,0,0);
			}
			else{
				cout << "You should try typing correctly.\n";
			}
		}
		else{
			cout <<"Please enter a valid command\n";
		}
	}
	//if n, ask to save training.
}
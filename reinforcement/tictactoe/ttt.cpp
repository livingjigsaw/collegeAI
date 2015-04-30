#include <iostream>
#include <cstdlib>
#include "ttt.h"

void InternalState::initAction(){ 
	for(int i=0;i<9;i++){	//boardstates will always be a length 9 string
		if(boardState[i]==' '){
			action[i] = 50;
		}
		else{
			action[i]=0;
		}
	}
}

void InternalState::modifyAction(int loc, bool won){
	if(won){
		action[loc]+=1;
	}
	else{
		if(action[loc]>1){
			action[loc]-=1;
		}
	}
}

int Human::takeMove(std::string inBoard){
	std::cout << "Current board state:\n";
	std::cout << inBoard[0] << "|" << inBoard[1] << "|" << inBoard[2] << std::endl;
	std::cout << "-----\n";
	std::cout << inBoard[3] << "|" << inBoard[4] << "|" << inBoard[5] << std::endl;
	std::cout << "-----\n";
	std::cout << inBoard[6] << "|" << inBoard[7] << "|" << inBoard[8] << std::endl;
	std::cout << std::endl;
	std::cout << "What is your move?\n";
	int myMove=0; 	//can check for errors here
	std::cin >> myMove;
	return myMove;
}

void Bot::insertState(std::string inBoard){
	InternalState* temp = new InternalState(inBoard);
	states.push_back(temp);
	temp=NULL;
}

int Bot::findState(std::string inBoard){
	for(int i=0;i<states.size();i++){
		if(inBoard==states[i]->get_board()){
			return i;
		}
	}
	//at this point, the state does not exist. so we create one
	insertState(inBoard);
	return states.size()-1;	//we want to return an int of the final point.
}

int Bot::takeMove(std::string inBoard){
	//check for gamestate in saved states
		//if found use it
		//if not found init a new one from inBoard
	int loc=findState(inBoard); //this represents the index of state in states
	moveHistory.push_back(loc);	//puts which state I'm in into history
	//find sum of actions
	int sum=0;
	for(int i=0;i<9;i++){
		sum+=states[loc]->get_actionChance(i);
	}
	//get rand number from 0 to sum-1 (the -1 accounts for 0 as a solution)
	int move = rand()%(sum-1);
	//find out where rand number fell and take that action
	int count=0;
	while(move >= states[loc]->get_actionChance(count)){
		move -= states[loc]->get_actionChance(count);
		count++;
	}
	actionHistory.push_back(count);	//puts action taken into history
	return count;	//to modify game state
}

void Bot::train(int winner){	//bot can only be trained if it played itself!!
	for(int i=0;i<moveHistory.size();i++){ //moveHistory and actionHistory are the same size
		if(i%2==(winner-1)){ //this allows for simple comparison to decide if current move helped win or lose. x win = 1, so win-1=0, the first, thrid, and so on moves
			states[moveHistory[i]]->modifyAction(actionHistory[i],true);	
		}
		else{
			states[moveHistory[i]]->modifyAction(actionHistory[i],false);
		}
	}
}

void Bot::clearHistory(){
	moveHistory.clear();
	actionHistory.clear();
}

void Bot::printTraining(){
	std::cout << states.size() << "\n";
	/*for(int i=0;i<states.size();i++){
		std::cout << states[i]->get_board() << "\n";
		for(int j=0;j<9;j++){
			std::cout<< states[i]->get_actionChance(j)<<" ";
		}
		std::cout << "\n";
	}
	*/
}


int GameState::isOver(){

	if(board[0]!=' ' && board[0]==board[1] && board[1]==board[2]){
		if(board[0]=='x'){
			return 1;
		}
		if(board[0]=='o'){
			return 2;
		}
	}
	else if(board[3]!=' ' && board[3]==board[4] && board[4]==board[5]){
		if(board[3]=='x'){
			return 1;
		}
		if(board[3]=='o'){
			return 2;
		}
	}	
	else if(board[6]!=' ' && board[6]==board[7] && board[7]==board[8]){
		if(board[6]=='x'){
			return 1;
		}
		if(board[6]=='o'){
			return 2;
		}
	}	
	else if(board[0]!=' ' && board[0]==board[3] && board[3]==board[6]){
		if(board[0]=='x'){
			return 1;
		}
		if(board[0]=='o'){
			return 2;
		}
	}	
	else if(board[1]!=' ' && board[1]==board[4] && board[4]==board[7]){
		if(board[1]=='x'){
			return 1;
		}
		if(board[1]=='o'){
			return 2;
		}
	}	
	else if(board[2]!=' ' && board[2]==board[5] && board[5]==board[8]){
		if(board[2]=='x'){
			return 1;
		}
		if(board[2]=='o'){
			return 2;
		}
	}	
	else if(board[0]!=' ' && board[0]==board[4] && board[4]==board[8]){
		if(board[0]=='x'){
			return 1;
		}
		if(board[0]=='o'){
			return 2;
		}
	}	
	else if(board[2]!=' ' && board[2]==board[4] && board[4]==board[6]){
		if(board[2]=='x'){
			return 1;
		}
		if(board[2]=='o'){
			return 2;
		}
	}
	//winning states have been checked now check for empty slots (game not over)
	for(int i=0;i<9;i++){
		if(board[i]==' '){
			return 0;
		}
	}

	//board is full and no winning states exist. it's a tie
	return 3;
}

void GameState::insertMove(int loc, bool player){
	if(!player){ //x's turn
		board[loc]='x';
	}
	else{
		board[loc]='o';
	}
}

void GameState::playGame(bool xIsBot, bool oIsBot, bool training){
	Human* tempH = new Human();
	if(xIsBot){
		xplayer=&myBot;
	}
	else{
		xplayer=tempH;
	}
	if(oIsBot){
		oplayer=&myBot;
	}
	else{
		oplayer=tempH;
	}
	resetGame();
	myBot.clearHistory();
	int winner = isOver();
	while(winner==0){
		insertMove(xplayer->takeMove(board), 0);
		winner = isOver();
		if(winner==0){	//don't want to add states that are past a winning state
			insertMove(oplayer->takeMove(board), 1);
			winner = isOver();
		}
	}
	if(training && winner!=3 && xIsBot && oIsBot){	//train bots only when there is a winner and only when both players are the bot
		myBot.train(winner);
	}
	if(!training){	//cout results
		std::cout << "Game Over!\n";
		std::string win;
		if(winner==1){
			win='x';
		}
		else if(winner==2){
			win='o';
		}
		else{
			win="the cat";
		}
		std::cout << "the winner is " << win << " !\n";
		printBoard();
		delete tempH; //goodbye, Chell.
	}
}

void GameState::printBoard(){
	std::cout << board[0] << "|" << board[1] << "|" << board[2] << std::endl;
	std::cout << "-----\n";
	std::cout << board[3] << "|" << board[4] << "|" << board[5] << std::endl;
	std::cout << "-----\n";
	std::cout << board[6] << "|" << board[7] << "|" << board[8] << std::endl;
}
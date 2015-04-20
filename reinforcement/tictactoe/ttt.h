#include <vector>

class InternalState{ //this represents a game state inside of a bot, with action/probability pairs
	private:
		std::string boardState; //const?
		int* action; // index in array represents the location of the move, the value there represents the probablity
		void initAction(); //checks the state, and sets values of empty locations to a set value
	
	public:

		InternalState(){action = new int[9];initAction();}
		InternalState(std::string inState){boardState = inState; action = new int[9]; initAction();}
		~InternalState(){delete action;}

		std::string get_board(){return boardState;}
		int get_actionChance(int location){return action[location];}
		
		void modifyAction(int loc, bool won); //if move helped win, won is true and we increase at location. else, we decrease at location

};

class Player{  //abstract class, bot and human inherit from this class
public:
	virtual int takeMove(std::string inBoard){} 
};

class Human: public Player{
public:
	int takeMove(std::string inBoard); //use iostream 
};

class Bot: public Player{
private:
	std::vector<InternalState*> states;
	std::vector<int> moveHistory; //represents the state in "states" entered on turn [location]
	std::vector<int> actionHistory; //represents the action taken on turn [location]
	int findState(std::string inBoard);	//returns index of where given state exists, to be called from within takemove
	void insertState(std::string inBoard);	//creates and inserts a new state
public:
	Bot(){}
	~Bot(){states.clear();}
	int takeMove(std::string inBoard); //determine which move to make, make it, store in move history
	void train(int winner); 	//this navigates the game history and makes the bots adjust appropriately
	void clearHistory();
};

class GameState{
private:
	std::string board;
	Player* xplayer;	//pointers, so that inherited classes can be pointed to dynamically
	Player* oplayer;
	Bot myBot;
	/*
	X player will always go first, so turns in history will be alternating and predictable; in other words, no need to store who's turn it was in the history;
	*/
	void resetGame(){board="         ";}
public:
	GameState(){}

	int isOver();	//returns 0 if not over, 1 if x won, 2 if O won, and 3 if tie
	void insertMove(int loc, bool player);	//0 for x, 1 for o
	void playGame(bool xIsBot, bool oIsBot, bool training); //this determines which players are bots, and if the bots should be trained
	//if training is false, cout results of game at the end
	void printBoard();

};


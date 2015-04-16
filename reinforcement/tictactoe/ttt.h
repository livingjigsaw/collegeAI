
class internalState{ //this represents a game state inside of a bot, with action/probability pairs
	private:
		std::string boardState;
		int* action; // index in array represents the location of the move, the value there represents the probablity
	public:
		internalState(){action = new int[9];}
		internalState(std::string inState){boardState = inState; action = new int[9];}

};

class Player{  //abstract class, bot and human inherit from this class
public:
	virtual int takeMove(); 
};

class human: public Player{
public:
	int takeMove(); //use iostream 
};

class Bot: public Player{
private:
	
public:
	int takeMove(); //determine which move to make, make it, store in move history
};

class GameState{
private:
	string board;
	bool turn; //0 = x's turn, 1=O's turn
	Player* xplayer;	//pointers, so that inherited classes can be pointed to dynamically
	Player* oplayer;
public:

};


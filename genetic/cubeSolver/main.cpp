#include<iostream>
#include<fstream>
#include<sstream>
#include"containers.h"
using namespace std;

bool initPieces(vector<piece*> &outpieces){
	piece* tempPiece;
	coord tConfig;
	tConfig.x=0,tConfig.y=0,tConfig.z=0;


	string filename;
	cout << "Please enter the filename of the input file\n";
	cin >> filename;
	ifstream infile(filename.c_str());
	//cout << "attempted to open file\n";
	if (infile.is_open()){
		string currentLine;
		stringstream ss;
		while(getline(infile, currentLine)){
			ss.clear();	
			ss << currentLine;
			char parse;
			string tempName="";
			int numSubdiv=0;
				tempPiece = new piece();
				ss >> tempName;
				tempPiece->name=tempName;
				ss >> numSubdiv;
				for(int i=0;i<numSubdiv;i++){
					getline(infile, currentLine);
					ss.clear();
					ss << currentLine;
					ss >> tConfig.x;
					ss >> parse;
					ss >> tConfig.y;
					ss >> parse;
					ss >> tConfig.z;
					tempPiece->baseConfig.push_back(tConfig);
				}
				outpieces.push_back(tempPiece);

		}
		//cout << "out of parsing\n";
		infile.close();

		return true;
	}
	else{
		return false; 
	}
}

int main(){
	vector<piece*> tetrisPieces;

	initPieces(tetrisPieces);
	tetrisPieces[11]->baseConfig[5].print(); cout << endl;
	srand(time(NULL));	//initialize the seed for pseudo-randomness
	cout << "what size do you want the population to be?\n";
	int popSize;
	cin >> popSize;
	Population genitor(popSize);
	genitor.set_pieces(tetrisPieces);
	genitor.randInit();
	genitor.genitor();
}
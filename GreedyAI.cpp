	#include "GreedyAI.h"
	#include <iostream>
	#include <sstream>
	#include <string>

	using namespace std;

	GreedyAI::GreedyAI(){
	}

	GreedyAI::~GreedyAI(){
		
	}

	string GreedyAI::getName(){
		return "GreedyAI";
	}

	void GreedyAI::initialize(Dictionary* dict){
		d= dict;
		set<string>temp = dict->allWords();
		for(set<string>::iterator it = temp.begin(); it!=temp.end(); ++it){
			string lol="";
			for(unsigned int i=0; i<=(*it).length(); i++){
				string lol = (*it).substr(0,i);
				if(prefix.find(lol)==prefix.end()){ //not found
					//cout<<lol<<endl;
					prefix.insert(lol);
				}
			}
		}
	}



	Move GreedyAI::getMove (const Board & board, const Player & player, std::map<char, int> initialTileCount){
		moves.clear();
		scores.clear();

		string hand;
		set<Tile*> temp = player.getHandTiles();
		for(set<Tile*>::iterator it = temp.begin(); it!=temp.end(); ++it){
			hand+=(*it)->getLetter();
		}

		/*int x = 7;
		int y = 7;
		solveHelperVertical(y,x,y,x,"","",hand,player,board);*/
		//cerr << board.getColumns() << endl;
		//cerr << board.getRows() << endl;
		for(int y=0; y<board.getColumns(); y++){ //doesn't make moves other than on the starting tile?
			for(int x=0; x<board.getRows(); x++){
				solveHelperHorizontal(y,x,y,x,"","", hand, player, board);
				solveHelperVertical(y,x,y,x,"","",hand, player, board);
			}
		}

		if (moves.empty()) {
			// Move m(true, hand, player);
			// return m;
			Move m;
			return m;
		}
		else {
			//cerr << "MOVES SIZE: " << (short) moves.size() << endl;
			//cerr << "SCORE SIZE: " << (short) scores.size() << endl;

			int max = -1;
			int j;
			for (int i = 0; i < (short) scores.size(); i++) {
				if (scores[i] > max) {
					max = scores[i];
					j = i;
				}
			}
			//cerr << "MAX SCORE: " << max << endl;

			vector<string> place = moves[j];
			string word = place[0];
			bool horizontal;
			if (place[3] == "horizontal") {
				horizontal = true;
			}
			else {
				horizontal = false;
			}
			string placement;
			if (horizontal) {
				placement = "PLACE - " + place[2] + " " + place[1] + " " + word;
			}
			else {
				placement = "PLACE | " + place[2] + " " + place[1] + " " + word;
			}
			cerr << placement << endl;

			try {
				Move m(placement, player);
				return m;

			}
			catch (MoveException &m) {
				cerr<<m.getMessage()<<endl;
				Move pass;
				return pass;
			}
		}
		
	 }

	//void GreedyAI::solveHelperHorizontal(int rows, int cols, const Board& board, const Player& player, string temp, string remaining){
void GreedyAI::solveHelperHorizontal(int rows, int cols, int start_row, int start_col, string used, string temp, string remaining, const Player& player, const Board& board){
	//cerr << "(" << rows << ", " << cols << ")" << endl;
	if(board.getSquare(rows,cols)->isOccupied() && remaining.length()==0){
		return;
	}
	if(cols == board.getColumns() || rows == board.getRows()) {

	}

	string new_temp;
	string new_remaining;
	string new_used;

	if(board.getSquare(rows,cols)->isOccupied()==true){ 
		if (temp == "") {
			//cols += 1;
		}
	   char c = board.getSquare(rows,cols)->getLetter();
	   //cerr << "(" << rows << ", " << cols << "): " << c << endl;
	   new_temp = temp + c ;
	   //cerr << new_temp << endl;
	   new_remaining = remaining;
	   new_used = used;
	   //cout<<"new_temp: "<<new_temp<<endl;
	   if(prefix.find(new_temp)!=prefix.end()){
			 if(d->isLegalWord(new_temp)){				
				try{
					Move currMove(start_row+1,start_col+1, true, new_used, player);
					int x=0;
					vector<string> words = board.getWords(currMove,x);
					//cout << temp << endl;
					//cout << "x is: " << x << endl;
					bool legalwords = true;
					for (int i = 0; i < (short) words.size(); i++) {
						if (!d->isLegalWord(words[i])) {
							legalwords = false;
							//cout << words[i] << "is not a legal word" << endl;
						}
					}
					if (legalwords) {
						//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
						vector<string> v;
						v.push_back(new_used);
						stringstream ss;
						ss << start_row+1;
						string rows_temp, cols_temp;
						ss >> rows_temp;
						stringstream ss2;
						ss2 << start_col+1;
						ss2 >> cols_temp;
						v.push_back(rows_temp);
						v.push_back(cols_temp);
						v.push_back("horizontal");
						//cerr << "pushed_back: " << new_temp << " used: " << new_used << endl;
						moves.push_back(v);
						scores.push_back(x);
					}	
				}
				catch(MoveException &m){
				//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

					//cerr<<m.getMessage()<<endl;
				}				
			}
			if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
				return;
			}
			else {
				solveHelperHorizontal(rows,cols+1, start_row, start_col, new_used, new_temp, new_remaining, player, board);
			}
		} //end of if prefix

	}

	else{
		for(unsigned int i=0; i<remaining.size(); i++){
			if (isalpha(remaining[i])) {
			    	new_used = used + remaining[i];
					new_temp = temp + remaining[i];
					new_remaining = remaining.substr(0,i) + remaining.substr(i+1);
				if(prefix.find(new_temp)!=prefix.end()){
					if(d->isLegalWord(new_temp)){				
						try{
							Move currMove(start_row+1,start_col+1, true, new_used, player);
							int x=0;
							vector<string> words = board.getWords(currMove,x);
							//cout << temp << endl;
							//cout << "x is: " << x << endl;
							bool legalwords = true;
							for (int i = 0; i < (short) words.size(); i++) {
								if (!d->isLegalWord(words[i])) {
									legalwords = false;
									//cout << words[i] << "is not a legal word" << endl;
								}
							}
							if (legalwords) {
								//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
								vector<string> v;
								v.push_back(new_used);
								stringstream ss;
								ss << start_row+1;
								string rows_temp, cols_temp;
								ss >> rows_temp;
								stringstream ss2;
								ss2 << start_col+1;
								ss2 >> cols_temp;
								v.push_back(rows_temp);
								v.push_back(cols_temp);
								v.push_back("horizontal");
								//cerr << "pushed_back: " << new_temp << " used: " << new_used <<  " row: " << start_row+1 << " col: " << start_col+1 <<endl;

								moves.push_back(v);
								scores.push_back(x);
							}	
						} //end of if prefix
						catch(MoveException &m){
						//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

							//cerr<<m.getMessage()<<endl;
						}	
					}
					if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
						return;
					}
					else {
						solveHelperHorizontal(rows,cols+1, start_row, start_col, new_used, new_temp, new_remaining, player, board);
					}
				}
			}
			else if (remaining[i] == '?') {
				for (char c = 'A'; c <= 'Z'; c++) {
					new_used += used + "?" + c;
					new_temp = temp + c;
					new_remaining = remaining.substr(0,i) + remaining.substr(i+1);
					if(prefix.find(new_temp)!=prefix.end()){
						if(d->isLegalWord(new_temp)){				
							try{
								Move currMove(start_row+1,start_col+1, true, new_used, player);
								//cerr << new_used << endl;
								int x=0;
								vector<string> words = board.getWords(currMove,x);
								//cout << temp << endl;
								//cout << "x is: " << x << endl;
								bool legalwords = true;
								for (int i = 0; i < (short) words.size(); i++) {
									if (!d->isLegalWord(words[i])) {
										legalwords = false;
										//cout << words[i] << "is not a legal word" << endl;
									}
								}
								if (legalwords) {
									//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
									vector<string> v;
									v.push_back(new_used);
									stringstream ss;
									ss << start_row+1;
									string rows_temp, cols_temp;
									ss >> rows_temp;
									stringstream ss2;
									ss2 << start_col+1;
									ss2 >> cols_temp;
									v.push_back(rows_temp);
									v.push_back(cols_temp);
									v.push_back("horizontal");
									//cerr << "pushed_back: " << new_temp << " used: " << new_used <<  " row: " << start_row+1 << " col: " << start_col+1 <<endl;

									moves.push_back(v);
									scores.push_back(x);
								}	
							} //end of if prefix
							catch(MoveException &m){
							//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

								//cerr<<m.getMessage()<<endl;
							}	
						}
						if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
							return;
						}
						else {
							solveHelperHorizontal(rows,cols+1, start_row, start_col, new_used, new_temp, new_remaining, player, board);
						}
					}
				}
			}
		}//end of for loop
		
		return;
	
	} //end of else

}

void GreedyAI::solveHelperVertical(int rows, int cols, int start_row, int start_col, string used, string temp, string remaining, const Player& player, const Board& board) {
		//cerr << "(" << rows << ", " << cols << "): " << temp << " ";
	if(board.getSquare(rows,cols)->isOccupied() && remaining.length()==0){
		return;
	}

	if(cols == board.getColumns() || rows == board.getRows()) {

	}

	string new_temp;
	string new_remaining;
	string new_used;

	if(board.getSquare(rows,cols)->isOccupied()==true){ 
		if (temp == "") {
			//rows += 1;
		}
	   char c = board.getSquare(rows,cols)->getLetter();
	   //cerr << "(" << rows << ", " << cols << "): " << c << endl;
	   new_temp = temp + c ;
	   //cerr << new_temp << endl;
	   new_remaining = remaining;
	   new_used = used;
	   //cout<<"new_temp: "<<new_temp<<endl;
	   if(prefix.find(new_temp)!=prefix.end()){
			 if(d->isLegalWord(new_temp)){				
				try{
					Move currMove(start_row+1,start_col+1, false, new_used, player);
					int x=0;
					vector<string> words = board.getWords(currMove,x);
					//cout << temp << endl;
					//cout << "x is: " << x << endl;
					bool legalwords = true;
					for (int i = 0; i < (short) words.size(); i++) {
						if (!d->isLegalWord(words[i])) {
							legalwords = false;
							//cout << words[i] << "is not a legal word" << endl;
						}
					}
					if (legalwords) {
						//cerr << new_temp << " is a legal word";
						//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
						vector<string> v;
						v.push_back(new_used);
						stringstream ss;
						ss << start_row+1;
						string rows_temp, cols_temp;
						ss >> rows_temp;
						stringstream ss2;
						ss2 << start_col+1;
						ss2 >> cols_temp;
						v.push_back(rows_temp);
						v.push_back(cols_temp);
						v.push_back("vertical");
						moves.push_back(v);
						scores.push_back(x);
					}	
				}
				catch(MoveException &m){
				//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

					//cerr<<m.getMessage()<<endl;
				}				
			}
			if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
				return;
			}
			else {
				solveHelperVertical(rows+1,cols, start_row, start_col, new_used, new_temp, new_remaining, player, board);
			}
		} //end of if prefix

	}

	else{
		for(unsigned int i=0; i<remaining.size(); i++){
			if (isalpha(remaining[i])) {
			    	new_used = used + remaining[i];
					new_temp = temp + remaining[i];
					new_remaining = remaining.substr(0,i) + remaining.substr(i+1);

				if(prefix.find(new_temp)!=prefix.end()){
					if(d->isLegalWord(new_temp)){				
						try{
							Move currMove(start_row+1,start_col+1, false, new_used, player);
							int x=0;
							vector<string> words = board.getWords(currMove,x);
							//cout << temp << endl;
							//cout << "x is: " << x << endl;
							bool legalwords = true;
							for (int i = 0; i < (short) words.size(); i++) {
								if (!d->isLegalWord(words[i])) {
									legalwords = false;
									//cout << words[i] << "is not a legal word" << endl;
								}
							}
							if (legalwords) {
								//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
								//cerr << new_temp << " is a legal word";
								vector<string> v;
								v.push_back(new_used);
								stringstream ss;
								ss << start_row+1;
								string rows_temp, cols_temp;
								ss >> rows_temp;
								stringstream ss2;
								ss2 << start_col+1;
								ss2 >> cols_temp;
								v.push_back(rows_temp);
								v.push_back(cols_temp);
								v.push_back("vertical");
								moves.push_back(v);
								scores.push_back(x);
							}	
						} //end of if prefix
						catch(MoveException &m){
						//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

							//cerr<<m.getMessage()<<endl;
						}	
					}
					if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
						return;
					}
					else {
						solveHelperVertical(rows+1,cols, start_row, start_col, new_used, new_temp, new_remaining, player, board);
					}
				}
			}
			else if (remaining[i] == '?') {
				//cerr << "true" << endl;
				for (char c = 'A'; c <= 'Z'; c++) {
					new_used += used + "?" + c;
					//cerr << new_used << ", ";
					new_temp = temp + c;
					new_remaining = remaining.substr(0,i) + remaining.substr(i+1);
					if(prefix.find(new_temp)!=prefix.end()){
						if(d->isLegalWord(new_temp)){				
							try{
								Move currMove(start_row+1,start_col+1, false, new_used, player);
								//cerr << new_used << endl;
								int x=0;
								vector<string> words = board.getWords(currMove,x);
								//cout << temp << endl;
								//cout << "x is: " << x << endl;
								bool legalwords = true;
								for (int i = 0; i < (short) words.size(); i++) {
									if (!d->isLegalWord(words[i])) {
										legalwords = false;
										//cout << words[i] << "is not a legal word" << endl;
									}
								}
								if (legalwords) {
									//cout<<"ROWS: "<<rows+1<<", COLS: "<<cols+1<<", WORD: "<<new_temp<<", USED: "<<new_used<<", SCORE: "<<x<<endl;
									vector<string> v;
									v.push_back(new_used);
									stringstream ss;
									ss << start_row+1;
									string rows_temp, cols_temp;
									ss >> rows_temp;
									stringstream ss2;
									ss2 << start_col+1;
									ss2 >> cols_temp;
									v.push_back(rows_temp);
									v.push_back(cols_temp);
									v.push_back("vertical");
									//cerr << "pushed_back: " << new_temp << " used: " << new_used <<  " row: " << start_row+1 << " col: " << start_col+1 <<endl;

									moves.push_back(v);
									scores.push_back(x);
								}	
							} //end of if prefix
							catch(MoveException &m){
							//cout<<"ROWS: "<<rows<<", COLS: "<<cols<<", WORD: "<<new_temp<<", USED: "<<new_used<<endl;

								//cerr<<m.getMessage()<<endl;
							}	
						}
						if(cols+1 == board.getColumns() || rows+1 == board.getRows()){
							return;
						}
						else {
							solveHelperVertical(rows+1,cols, start_row, start_col, new_used, new_temp, new_remaining, player, board);
						}
					}
				}
			}
		}//end of for loop
		//cerr << endl;
		return;
	
	} //end of else
}

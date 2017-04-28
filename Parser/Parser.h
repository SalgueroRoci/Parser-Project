/*
*	Program: Parser.cpp
*
*	Programmers:
* 		Naveena Chilukuri 	Email: naveenac@csu.fullerton.edu
*		Rocio Salguero 		Email: salgueroroci@csu.fullerton.edu
Cameron: Email: haipayazoo@gmail.com

* 	Date Created: 04/27/17
*	Description:
*
* 	Last Modified: 04/27/17
*/

#include "Lexer.h" 
#include <map>
#include <string>
#include <stack>

#if !defined(_PS_)
#define _PS_

enum Non_Terminals {
	Pgm = 0, Block, Stmts, Stmt, Astmt, Y, Ostmt, Wstmt, Fstmt, Else2, Elist, Elist2,
	X, E, T, Z, F, Pexpr, Fatom, Opadd, Opmul
};

//structures for grammar 
struct symbols {
	bool isTerm;
	Non_Terminals idnon;
	idType idterm;
	int symindex;
};

//Structure for rules
struct rules {
	int LHS;  //index of symArray
	int RHS[10]; //index of symArray
	int numKids; 
};

struct Node {
	int sym; //index for the grammerRules
	Node* kids[10];
	int numofKids;
};

class Parser : public Lexer
{
public:
	Parser();
	~Parser();
	void createPST(); 

private:

	map<enum Non_Terminals, map<enum idType, int>> table; //LL Matrix
														//returns the index for rules array
	stack<Node> stackParser;	//Stack for parsing
	
	Node* grandma; //node pointing to root node	
	Node* tracker; //node pointing to top of stack 

	symbols* symArray = new symbols[45];
	rules* grammerRules = new rules[40];	
	//input for parser : token * tokenStream = new token[maxtokens];

	//we have to populate the rules 
	void populate_symbols(); 
	void populate_rules();
	void populate_LLmatrix();

};

#endif
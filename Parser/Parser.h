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
* 	Last Modified: 5/11/17
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
	int node_id;	
	string value;	
};

struct occurence {
	int line_num;
	bool is_def;
};

struct table_symbol {
	string name;
	double value_dbl;
	string value_str;
	bool is_num;
	int ix;
	occurence occurences[10];

};

class Parser : public Lexer
{
public:
	Parser();
	~Parser();
	void createPST(); 
	void printTree();
	void createAST(); 
	void printSymbolTable();
private:

	map<enum Non_Terminals, map<int, int> > table; //LL Matrix
	//returns the index for rules array
	stack<Node*> stackParser;	//Stack for parsing

	Node* grandma; //node pointing to root node	
	Node* tracker; //node pointing to top of stack 
	int ID;

	symbols* symArray = new symbols[45];
	symbols* idInfo = new symbols[50]; //array to keep all the new ident created. 
	rules* grammerRules = new rules[40];
	//input for parser : token * tokenStream = new token[maxtokens];

	table_symbol* symTable = new table_symbol[50];
	int num_of_sym; 

	//we have to populate the rules 
	void populate_symbols();
	void populate_rules();
	void populate_LLmatrix();
	void printTreeHelper(Node* current);
	int getSymInx(int term); 
	string nonTerm(Non_Terminals x);

	void addtoSymTable(token symbol, int nextToken);

	void traversePost(Node* current);
	void yacccode(Node* current);
	void copyGuts(Node* node1, Node* node2);

};

#endif
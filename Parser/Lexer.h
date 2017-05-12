/*
*	Program: Lexer.cpp
*
*	Programmers:
* 		Naveena Chilukuri 	Email: naveenac@csu.fullerton.edu
*		Rocio Salguero 		Email: salgueroroci@csu.fullerton.edu
*
* 	Date Created: 02/19/17
*	Description: Lexer program which reads in a file and seperates the contents into significant tokens.
*
* 	Last Modified: 05/11/17
*/

#if !defined(_LA_)
#define _LA_


#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
using namespace std;

enum idType {
	eof = 0, newLine, ident = 2, integer, floatInt, stringType, //
	comma, semi, //unpaired delimiters 
	kwdprog, kwdmain, kwdfcn, kwdclass, kwdfloat, kwdint, kwdstring, kwdif, //keywords
	kwdelseif, kwdelse, kwdwhile, kwdinput, kwdprint, kwdnew, kwdreturn,
	angle1, angle2, brace1, brace2, bracket1, bracket2, parens1, parens2, //paired delimiters 
	aster, caret, colon, dot, equal, minus, plus, slash,
	oparrow, opeq, opne, ople, opge, opshl, opshr,  //Multi-char operators 
	letter, digit, space, pdelimiter, udelimiter, mcop, opunc, underScore, uops, quote, comment, //extra test flags
	error = 99 //Miscellaeous 
};

//Struct can be easier to label tokens. 
struct token {
	int lineNum; //keep track on which line it appeared 
	//Identify which tokens are which 
	int type;
	bool hasVal;
	string value; //Used for strings, identifiers, int, and float
};



class Lexer {
private:
	ifstream inputFile;
	int maxtokens = 1000;

	int charClass;
	char nextChar;
	string lexme;


	int lex();
	void addChar();
	void getChar();
	void rewindChar();

public:
	int lineCount = 0, numTokens = 0; //Keep track on what line youre on while reading a file 
	token* tokenStream = new token[maxtokens]; // Don't forget to delete [] a; when you're done!
	string tokenType(int a);

	Lexer();
	~Lexer();

	void read_file(string input);
	void addTokens();
	void print_tokens();
	void close_file();

};

#endif
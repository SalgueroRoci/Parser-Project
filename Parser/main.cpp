/*
*	Program: main.cpp (For Lexer)
*
*	Programmers:
* 	  Naveena Chilukuri 	Email: naveenac@csu.fullerton.edu
*	  Rocio Salguero 		Email: salgueroroci@csu.fullerton.edu
*	  Cameron:				Email: haipayazoo@gmail.com
* 	Date Created: 02/19/17
*	  Description: Main program to output the tokens.
*
* 	Last Modified: 04/27/17
*/

#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

int main() {
	Parser parme;

	string fileIn;
	cout << "Please enter file name\n";
	cin >> fileIn;

	try {
		//start tokenizing 
		parme.read_file(fileIn);
		parme.addTokens();
		parme.print_tokens();
		parme.close_file();

		//start parsing 
		parme.createPST();
		cout << "\nPrinting PST: "; 
		parme.printTree();		
		parme.createAST();
		cout << "\nPrinting AST: ";
		parme.printTree();	
		cout << "\nPringting Sym table: \n";
		parme.printSymbolTable();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "Exception opening/reading/closing file\n";
	}
	
	system("pause");
}
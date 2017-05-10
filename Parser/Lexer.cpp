/*
*	Program: Lexer.cpp
*
*	Programmers:
* 		Naveena Chilukuri 	Email: naveenac@csu.fullerton.edu
*		Rocio Salguero 		Email: salgueroroci@csu.fullerton.edu
*   Website to edit: https://kobra.io/#/e/-KdZ6nINv9kx4AselJIJ
* 	Date Created: 02/19/17
*	Description: Lexer program which reads in a file and seperates the contents into significant tokens.
*
* 	Last Modified: 02/22/17 : working and compiled file!
*/

#include "Lexer.h"

Lexer::Lexer() {}
Lexer::~Lexer() {
}

//open file 
void Lexer::read_file(string input) {
	inputFile.open(input, ios::in);

}
void Lexer::close_file() {
	inputFile.close();
}

void Lexer::addTokens() {
	int token = space;
	lineCount = 1;

	do {
		token = lex();
		//if (token == eof) break; 
		if (token == space) continue; //skip if a space 
		if (token == newLine) continue;


		tokenStream[numTokens].lineNum = lineCount;
		tokenStream[numTokens].type = token;
		tokenStream[numTokens].value = lexme;

		//if string, ident then it has a value 
		if (token == stringType || token == ident || token == integer || token == floatInt)
			tokenStream[numTokens].hasVal = true;
		numTokens++;
		cout << "lexeme: \"" << lexme << "\"   token: " << tokenType(token) << endl;
	} while (inputFile.good());
}

void Lexer::rewindChar() {
	//put back character you read when checking
	//got error token so need to reread char
	inputFile.unget();
}

void Lexer::getChar()
{
	//gets acharacters from the file 
	nextChar = inputFile.get();
	//cout << "char read: " << nextChar << endl; 
	charClass = error;

	if ((nextChar > 64 && nextChar <91) || (nextChar > 96 && nextChar <123))
		charClass = letter;

	if (nextChar > 47 && nextChar <58)
		charClass = digit;
	if (nextChar == '_')
		charClass = underScore;

	if (nextChar == '(' || nextChar == ')' || nextChar == '{' || nextChar == '}' || nextChar == '[' || nextChar == ']' || nextChar == '<' || nextChar == '>')
		charClass = pdelimiter;

	if (nextChar == ',' || nextChar == ';' || nextChar == '!')
		charClass = udelimiter;

	if (nextChar == '*' || nextChar == '^' || nextChar == ':' || nextChar == '.' || nextChar == '=' || nextChar == '-' || nextChar == '+' || nextChar == '/')
		charClass = uops;

	if (nextChar == '"')
		charClass = quote;

	if (nextChar == ' ') charClass = space;
	if (nextChar == EOF) charClass = NULL;
	if (nextChar == '\t') charClass = space;
	if (nextChar == '\n') { lineCount++; charClass = newLine; } //keeps track of what line it was on

}

void Lexer::addChar()
{
	lexme += nextChar; 
}

int Lexer::lex()
{
	lexme = "";

	//reads a character 
	getChar();  //get the first char

	//start checking what kind of character it is 
	if (charClass == space) { return space; }
	if (charClass == error) { return error; }
	if (charClass == NULL) { return eof; }
	if (charClass == newLine) { return newLine; }

	switch (charClass) {

		//literal strings 
	case quote:
		getChar();
		while (charClass != quote) {
			addChar();
			getChar();
			if (charClass == eof)
				return error;
		}
		return stringType;
		break;

		//id with underscore at the front
	case underScore:
		//string starts with underscore
		getChar();
		if (charClass != letter || charClass != digit || charClass != underScore) {
			//not a valid ident 
			rewindChar();
			return error;
		}
		while (charClass == letter || charClass == digit || charClass == underScore)
		{
			addChar();
			getChar();
			if (charClass == letter || charClass == digit || charClass == underScore)
				continue;
			else {
				rewindChar();
				break;
			}
		}
		return ident;
		break;
	case letter:
		//Get the word
		while (charClass == letter || charClass == digit || charClass == underScore)
		{
			addChar();
			getChar();
			if (charClass == letter || charClass == digit || charClass == underScore)
				continue;
			else {
				rewindChar();
				break;
			}
		}

		//check if keyword
		if (lexme == "prog")
		{
			return kwdprog;
		}
		else if (lexme == "main")
		{
			return kwdmain;
		}
		else if (lexme == "fcn")
		{
			return kwdfcn;
		}
		else if (lexme == "class")
		{
			return kwdclass;
		}
		else if (lexme == "float")
		{
			return kwdfloat;
		}
		else if (lexme == "int")
		{
			return kwdint;
		}
		else if (lexme == "string")
		{
			return kwdstring;
		}
		else if (lexme == "if")
		{
			return kwdif;
		}
		else if (lexme == "elseif")
		{
			return kwdelseif;
		}
		else if (lexme == "else")
		{
			return kwdelse;
		}
		else if (lexme == "while")
		{
			return kwdwhile;
		}
		else if (lexme == "input")
		{
			return kwdinput;
		}
		else if (lexme == "print")
		{
			return kwdprint;
		}
		else if (lexme == "new")
		{
			return kwdreturn;
		}
		else if (lexme == "return")
		{
			return kwdreturn;
		}
		//if not a keyword return ident
		return ident;
		break;
	case digit:
		while (charClass == digit) {
			addChar();
			getChar();
			if (charClass == digit)
				continue;
			else {
				rewindChar();
				break;
			}
		}

		//check if floating point 
		if (nextChar == '.') // is a floating point 
		{
			getChar();
			if (charClass != digit) {
				rewindChar(); //did not follow a number then return integer and retoken dot 
				return integer;
			}
			//get the dot again
			rewindChar(); rewindChar(); getChar();

			do {
				addChar();
				getChar();
				if (charClass == digit)
					continue;
				else {
					rewindChar();
					break;
				}
			} while (charClass == digit);
			return floatInt;
		}

		return integer;
		break;

	case uops:
		addChar();

		if (lexme == "*")
		{
			return aster;
		}
		else if (lexme == "^")
		{
			return caret;
		}
		else if (lexme == ":")
		{
			return colon;
		}
		else if (lexme == ".")
		{
			return dot;
		}
		else if (lexme == "=") //special case '=='
		{
			getChar();
			if (nextChar == '='){
				addChar();
				return opeq;
			}

			rewindChar(); //go back
			return idType::equal;
		}
		else if (lexme == "-") //special case '->'
		{
			getChar();
			if (nextChar == '>'){
				addChar();
				return oparrow;
			}

			rewindChar(); //go back
			return idType::minus;
		}
		else if (lexme == "+")
		{
			return idType::plus;
		}
		else if (lexme == "/") //check if '//' for comments
		{
			getChar();
			if (nextChar == '/') {
				do {
					getChar();
				} while (nextChar != '\n');
				return space;
			}

			rewindChar(); //go back
			return slash;
		}
		return error; //failed all the other symbols 
		break;

	case udelimiter:
		addChar();
		if (lexme == ",")
		{
			return comma;
		}
		else if (lexme == ";")
		{
			return semi;
		}
		else if (lexme == "!") { //special case '!='
			getChar();
			if (nextChar == '='){
				addChar();
				return opne;
			}

			rewindChar(); //go back
			return opne;
		}
		return udelimiter;
		break;

	case pdelimiter:
		addChar();
		if (lexme == "(")
		{
			return parens1;
		}
		else if (lexme == ")")
		{
			return parens2;
		}
		else if (lexme == "{")
		{
			return bracket1;
		}
		else if (lexme == "}")
		{
			return bracket2;
		}
		else if (lexme == "[")
		{
			return brace1;
		}
		else if (lexme == "]")
		{
			return brace2;
		}
		else if (lexme == "<") //special case '<=' and '<<'
		{
			getChar();
			if (nextChar == '='){
				addChar();
				return ople;
			}
			else if (nextChar == '<') {
				addChar();
				return opshl;
			}

			rewindChar(); //go back
			return angle1;

		}
		else if (lexme == ">") //special case '>=' and '>>'
		{
			getChar();
			if (nextChar == '='){
				addChar();
				return opge;
			}
			else if (nextChar == '<') {
				addChar();
				return opshr;
			}

			rewindChar(); //go back
			return angle2;
		}
		return pdelimiter;
		break;

	default:
		return error;
	}

}

//The outputs have to be word :Token (#line Number) [Token type] ----------------------------------
//(if strings, ints, and floats, and identifiers [:str "value"]) 
void Lexer::print_tokens(){
	cout << "(:lang A3 \n";
	for (int i = 0; i < numTokens; i++) {
		cout << "(:token " << tokenStream[i].lineNum << " " << tokenType(tokenStream[i].type) << " ";
		if (tokenStream[i].hasVal == true) //if token has a value pair  
			cout << "str: \"" << tokenStream[i].value << "\" ";
		cout << ")" << endl;
	}
	//cout << "num of tokens: " << numTokens << endl; 
}

//Can return a string of the same name to be used for output
string Lexer::tokenType(int a) {
	switch (a)
	{
	case eof: 					return "EOF";
	case ident: 				return "ident";
	case integer: 				return "integer";
	case floatInt: 				return "float";
	case stringType:			return "str";
	case comma: 				return "comma";
	case semi: 					return 	"semi";
	case kwdprog:				return	"kwdprog";
	case kwdmain: 				return	"kwdmain";
	case kwdfcn: 				return	"kwdfcn";
	case kwdclass: 				return 	"kwdclass";
	case kwdfloat: 				return 	"kwdfloat";
	case kwdint: 				return	"kwdint";
	case kwdstring: 			return 	"kwdstring";
	case kwdif: 				return	"kwdif";
	case kwdelseif: 			return 	"kwdelseif";
	case kwdelse: 				return	"kwdelse";
	case kwdwhile: 				return 	"kwdwhile";
	case kwdinput: 				return 	"kwdinput";
	case kwdprint: 				return 	"kwdprint";
	case kwdnew: 				return	"kwdnew";
	case kwdreturn: 			return	"kwdreturn";
	case angle1: 				return	"angle1";
	case angle2: 				return	"angle2";
	case brace1: 				return	"brace1";
	case brace2: 				return	"brace2";
	case bracket1: 				return	"bracket1";
	case bracket2: 				return	"bracket2";
	case parens1: 				return	"parens1";
	case parens2: 				return	"parens2";
	case aster: 				return	"aster";
	case caret: 				return	"caret";
	case colon: 				return	"colon";
	case dot: 					return	"dot";
	case idType::equal: 		return	"equal";
	case idType::minus: 		return	"minus";
	case idType::plus: 			return	"plus";
	case slash: 				return	"slash";
	case oparrow: 				return	"oparrow";
	case opeq: 					return	"opeq";
	case opne: 					return	"opne";
	case ople: 					return  "ople";
	case opge: 					return  "opge";
	case opshl: 				return 	"opshl";
	case opshr: 				return 	"opshr";
	case error: 				return 	"error";
	default: return "Bad MyEnum";
	}
}
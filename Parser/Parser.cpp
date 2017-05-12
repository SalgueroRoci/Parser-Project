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

#include "Parser.h"

void Parser::createPST() {

	Node* pgm = new Node;
	int rule;
	int numofTokens = numTokens;
	int currentToken = 0;
	int t = -1; //used to look up in LL parse matrix 

	//Add the PGM Node to the tree
	pgm->sym = Pgm;
	grandma = pgm;

	//Adds the PGM Node onto the stack
	stackParser.push(pgm);

	//Loops until the stack is empty
	while (!stackParser.empty()) {

		//Points to the top of the stack
		tracker = (stackParser.top());

		std::cout << "top of stack:" << nonTerm(symArray[tracker->sym].idnon) << " top of input: " << tokenType(tokenStream[currentToken].type) << std::endl;

		//If the top of the stack is a terminal
		if (symArray[tracker->sym].isTerm == true) {
			std::cout << "top of stack term:" << tokenType(symArray[tracker->sym].idterm) << endl; 

			//If the terminal on the stack doesn't match the input then it exits
			if (symArray[tracker->sym].idterm != tokenStream[currentToken].type) {
				std::cout << "Error top of stack =/= top of token input" << endl;
				return;
			}
			//Otherwise, if it's an identifier then it's added to the symbol table
			else {
				if (symArray[tracker->sym].idterm == ident) {
					//Add to symbol table
					addtoSymTable(tokenStream[currentToken], currentToken + 1);
				}

				//Pop the node off the stack
				stackParser.pop();
				currentToken++;
			}
		}
		//If the top of the stack is a non-terminal
		else if (symArray[tracker->sym].isTerm == false){
			//Look up in LL Matrix
			//Checks to see 
			t = getSymInx(tokenStream[currentToken].type);
			rule = table[symArray[tracker->sym].idnon][symArray[t].idterm];

			//std::cout << symArray[tracker->sym].idnon << " " << t << endl;
			std::cout << " rule: " << rule << endl; 

			//If the cell in the LL Parse Matrix is empty
			if (rule == 0) {
				std::cout << "LL parse matrix returned null" << endl;
				return; 
			}

			//Look up the children of the node
			if (grammerRules[rule].numKids == 0)
				stackParser.pop();
			else {
				stackParser.pop();

				//Creates a the kids 
				for (int i = 0; i < grammerRules[rule].numKids; i++) {
					tracker->kids[i] = new Node;
					tracker->kids[i]->sym = grammerRules[rule].RHS[i];
					tracker->kids[i]->numofKids = 0;
				}

				tracker->numofKids = grammerRules[rule].numKids;


				for (int i = grammerRules[rule].numKids - 1; i >= 0; i--)
					stackParser.push(tracker->kids[i]);

			}


		} //End of non-terminal

		//Terminates if the input is empty but the stack is not empty
		if (currentToken > numofTokens && !stackParser.empty()) {
			std::cout << "Error! Ran out of tokens. " << endl;
			return;
		}
			

	}//End of While Loop

	//Terminates if the there is input but the stack is empty
	if (currentToken < numofTokens-1) {
		std::cout << "Stack empty but still have input" << endl;
		return;
	}	

	std::cout << "No errors, grammar fine! PST created" << endl;

}//end of createPST

void Parser::addtoSymTable(token symbol, int nextToken) {
	bool found = false;


	for (int i = 0; i < num_of_sym; i++) {
		if (symbol.value == symTable[i].name) {
			symTable[i].occurences[symTable[i].ix].line_num = symbol.lineNum;
			if (tokenStream[nextToken].type == idType::equal) {
				symTable[i].occurences[symTable[i].ix].is_def = true;

			}
			else {
				symTable[i].occurences[symTable[i].ix].is_def = false;
			}
			symTable[i].ix++;
			found = true;
		}
	}

	if (!found) {
		symTable[num_of_sym].ix = 1;
		symTable[num_of_sym].name = symbol.value;
		symTable[num_of_sym].occurences[0].line_num = symbol.lineNum;

		if (tokenStream[nextToken].type == idType::equal) {
			symTable[num_of_sym].occurences[0].is_def = true;
		}
		else {
			symTable[num_of_sym].occurences[0].is_def = false;
		}
		num_of_sym++;
	}

}//end of func

void Parser::printSymbolTable() {
	cout << "Printing Symbol Table" << endl;

	for (int i = 0; i < num_of_sym; i++) {
		cout << "Name of Symbol:\t" << symTable[i].name << endl;
		cout << "Number of Times Symbol Occurs\t" << symTable[i].ix << endl;
		for (int j = 0; j < symTable[i].ix; j++) {
			cout << "Occurence " << j << ":" << endl;
			cout << "\tLine Number: " << symTable[i].occurences[j].line_num << endl;
			cout << "\tDefined? " << symTable[i].occurences[j].is_def << endl;
		}
	}
}

void Parser::printTree() {
	printTreeHelper(grandma);
	cout << endl; 
}

void Parser::printTreeHelper(Node* currentNode) {
	if (currentNode == NULL) return; 
	cout << "\n( ";
	if (symArray[currentNode->sym].isTerm == false) {
		cout << "Node:" << nonTerm(symArray[currentNode->sym].idnon);
	}
	else {
		cout << "Node:" << tokenType(symArray[currentNode->sym].idterm);
	}
	

	for (int i = 0; i < currentNode->numofKids; i++)
		printTreeHelper(currentNode->kids[i]);
	cout << ")"  ; //end of a branch
}

void Parser::createAST() {
	traversePost(grandma); 
}

void Parser::traversePost(Node* current) {
	if (current == NULL) return;

	for (int i = 0; i < current->numofKids; i++)
		traversePost(current->kids[i]);
	
	//post order 
	if (symArray[current->sym].isTerm == false) {
		//cout << "Traverse node:" << nonTerm(symArray[current->sym].idnon) << " num kids: " << current->numofKids << endl;
		yacccode(current); 
	}	
	else {
		//cout << "Traverse node:" << tokenType(symArray[current->sym].idterm) << " num kids: " << current->numofKids << endl;
	}
}

void Parser::yacccode(Node* current) {
	Non_Terminals value = symArray[current->sym].idnon;

	switch (value) {
	case Pgm:
		current->kids[0]->kids[0] = current->kids[1];
		current->kids[0]->numofKids = 1; 
		copyGuts(current, current->kids[0]);

		break;
	case Block:
		copyGuts(current, current->kids[1]);
		break;
	case Stmts:
		if (current->numofKids == 0 ) {
			
		}
		else {
			if (current->kids[2]->numofKids == 0 && symArray[current->kids[2]->sym].isTerm == false) {
				current->kids[1]->kids[0] = current->kids[0];
				current->kids[1]->numofKids = 1;
				copyGuts(current, current->kids[1]);
			}
			else {
				current->kids[1]->kids[0] = current->kids[0];
				current->kids[1]->kids[1] = current->kids[2];
				current->kids[1]->numofKids = 2;
				copyGuts(current, current->kids[1]);
			}
		}
		break;
	case Stmt:
		if (current->numofKids == 0) {
		
		}
		else
			copyGuts(current, current->kids[0]);
		break;
	case Astmt:
		current->kids[1]->kids[0] = current->kids[0];
		current->kids[1]->kids[1] = current->kids[2];
		current->kids[1]->numofKids = 2;
		copyGuts(current, current->kids[1]);
		break;
	case Y:		
		copyGuts(current, current->kids[0]);
		break;
	case Ostmt:
		if (current->kids[2]->numofKids == 0 && symArray[current->kids[2]->sym].isTerm == false) {

		}
		else {
			current->kids[0]->kids[0] = current->kids[0];
			current->kids[0]->numofKids = 1;
		}
			copyGuts(current, current->kids[0]);
		
		break;
	case Wstmt:
		current->kids[0]->kids[0] = current->kids[1];
		current->kids[0]->kids[1] = current->kids[2];
		current->kids[0]->numofKids = 2;
		copyGuts(current, current->kids[0]);
		break;
	case Fstmt:
		if (current->kids[3]->numofKids == 0 && symArray[current->kids[3]->sym].isTerm == false) {
			current->kids[0]->kids[0] = current->kids[1];
			current->kids[0]->kids[1] = current->kids[2];
			current->kids[0]->numofKids = 2;
		}
		else {
			current->kids[0]->kids[0] = current->kids[1];
			current->kids[0]->kids[1] = current->kids[2];
			current->kids[0]->kids[2] = current->kids[3];
			current->kids[0]->numofKids = 3;
		}		
		copyGuts(current, current->kids[0]);
		break;
	case Else2:
		if (current->numofKids == 0)
		{		}
		else if(current->numofKids == 2 ) {
			current->kids[0]->kids[0] = current->kids[1];
			current->kids[0]->numofKids = 1;
			copyGuts(current, current->kids[0]);
		}
		else {
			current->kids[0]->kids[0] = current->kids[1];
			current->kids[0]->kids[1] = current->kids[2];
			if (current->kids[3]->numofKids == 0 && symArray[current->kids[3]->sym].isTerm == false) {
				current->kids[0]->numofKids = 2;
			} 
			else {
				current->kids[0]->kids[2] = current->kids[3];
				current->kids[0]->numofKids = 3;
			}
			
			copyGuts(current, current->kids[0]);
		}
		break;
	case Elist:
		if (current->numofKids == 0)
		{
		}
		else {
			if (current->kids[1]->numofKids == 0 && symArray[current->kids[1]->sym].isTerm == false) {
				copyGuts(current, current->kids[0]);
			}
			else {
				current->kids[1]->kids[0] = current->kids[0];
				current->kids[1]->numofKids = 1;
				copyGuts(current, current->kids[1]);
			}
		}
		break;
	case Elist2:
		if (current->numofKids == 0) {
		}
		else {
			if (current->kids[1]->numofKids == 0 && symArray[current->kids[1]->sym].isTerm == false) {
				copyGuts(current, current->kids[0]);
			}
			else {
				current->kids[0]->kids[0] = current->kids[1];
				current->kids[0]->numofKids = 1;
				copyGuts(current, current->kids[0]);
			}
			
		}
		break;
	case X:
		if (current->numofKids == 0) {
		} 
		else {
			if (current->kids[2]->numofKids == 0 && symArray[current->kids[2]->sym].isTerm == false) {
				current->kids[0]->kids[0] = current->kids[1];
				current->kids[0]->numofKids = 1;
			} 
			else {
				current->kids[0]->kids[0] = current->kids[1];
				current->kids[0]->kids[1] = current->kids[2];
				current->kids[0]->numofKids = 2;
			}
			copyGuts(current, current->kids[0]);
		}
		break;
	case E:
		if (current->kids[1]->numofKids != 0 || symArray[current->kids[1]->sym].isTerm == true) {
			current->kids[0]->kids[0] = current->kids[1];
			current->kids[0]->numofKids = 1;
			copyGuts(current, current->kids[0]);
		}
		else {
			copyGuts(current, current->kids[0]);
		}
		break;
	case T:		
		if (current->kids[1]->numofKids == 0 && symArray[current->kids[1]->sym].isTerm == false) {
			copyGuts(current, current->kids[0]);
		}
		else {
			current->kids[1]->kids[0] = current->kids[0];
			current->kids[1]->numofKids = 1;
			copyGuts(current, current->kids[1]);			
		}
		
		break;
	case Z:
		if (current->numofKids == 0) {
		}
		else {
			current->kids[0]->kids[0] = current->kids[1]; 
			current->kids[0]->numofKids = 1;
			if (current->kids[2]->numofKids != 0) {
				current->kids[0]->kids[1] = current->kids[2];
				current->kids[0]->numofKids = 2;
			}
			copyGuts(current, current->kids[0]);
		}
		break;
	case F:
		copyGuts(current, current->kids[0]);
		break;
	case Pexpr:
		copyGuts(current, current->kids[0]);
		break;
	case Fatom:
		copyGuts(current, current->kids[0]);
		break;
	case Opadd:
		copyGuts(current, current->kids[0]);
		break;
	case Opmul:
		copyGuts(current, current->kids[0]); 
		break;
	default: return;
	}
}

void Parser::copyGuts(Node* node1, Node* node2) {
	int rest = node2->numofKids;
	cout << "gutted: " << nonTerm(symArray[node1->sym].idnon) << " num of kids: " << node1->numofKids << endl;

	//copy kids
	for (int i = 0; i < node2->numofKids; i++)
		node1->kids[i] = node2->kids[i];

	//void the rest of the kids if node1 has more kids than node2
	for (int i = rest; i < 10; i++)
		node1->kids[i] = NULL;

	node1->numofKids = node2->numofKids;
	node1->sym = node2->sym; 
	cout << "New: " << nonTerm(symArray[node1->sym].idnon) << " num of kids: " << node1->numofKids << endl; 
}

Parser::Parser() {
	populate_symbols();
	populate_rules();
	populate_LLmatrix();
	num_of_sym = 0; 
	//add eof to the end of tokenStream 
}

Parser::~Parser() {
	delete[] symArray; 
	delete[] grammerRules;
	delete[] tokenStream;
	delete[] symTable; 
}

int Parser::getSymInx(int term) {
	int i; 
	for (i = 21; i <= 43; i++) {
		//cout << " I : " << i << " term: " << term << "sym array" << symArray[i].idterm << endl;
		if (symArray[i].idterm == term)
			return i; 
	}
	return -1; 
}

void Parser::populate_symbols() {
	//Non terminals 21
	for (int i = 0; i <= static_cast<int>(Opmul); i++) {
		symArray[i].isTerm = false; 
		symArray[i].idnon = static_cast<Non_Terminals>(i);
	}

	//terminals  23 terminals 
	for (int i = static_cast<int>(Opmul)+1; i <= static_cast<int>(Opmul)+23; i++) {
		symArray[i].isTerm = true;
	}
	symArray[21].idterm = kwdprog; 
	symArray[22].idterm = bracket1;
	symArray[23].idterm = bracket2;
	symArray[24].idterm = parens1;
	symArray[25].idterm = parens2;
	symArray[26].idterm = semi;
	symArray[27].idterm = idType::equal;
	symArray[28].idterm = kwdinput;
	symArray[29].idterm = kwdprint;
	symArray[30].idterm = kwdwhile;	
	symArray[31].idterm = kwdif;
	symArray[32].idterm = kwdelseif;
	symArray[33].idterm = kwdelse;
	symArray[34].idterm = comma;
	symArray[35].idterm = ident;
	symArray[36].idterm = integer;
	symArray[37].idterm = floatInt;
	symArray[38].idterm = stringType;
	symArray[39].idterm = idType::plus;
	symArray[40].idterm = idType::minus;
	symArray[41].idterm = aster;
	symArray[42].idterm = slash;
	symArray[43].idterm = caret;
}

void Parser::populate_rules() {
	//index 0-43 for symbol array 
	grammerRules[1].LHS = 0;					
	grammerRules[1].RHS[0] = 21;		
	grammerRules[1].RHS[1] = 1;
	grammerRules[1].numKids = 2;		

	grammerRules[2].LHS = 1;			
	grammerRules[2].RHS[0] = 22;	
	grammerRules[2].RHS[1] = 2;
	grammerRules[2].RHS[2] = 23;
	grammerRules[2].numKids = 3;		

	grammerRules[3].LHS = 2;
	grammerRules[3].RHS[0] = 3;
	grammerRules[3].RHS[1] = 26;
	grammerRules[3].RHS[2] = 2;
	grammerRules[3].numKids = 3;

	grammerRules[4].LHS = 2;
	grammerRules[4].numKids = 0;

	grammerRules[5].LHS = 3;
	grammerRules[5].RHS[0] = 4;
	grammerRules[5].numKids = 1;

	grammerRules[6].LHS = 3;
	grammerRules[6].RHS[0] = 6;
	grammerRules[6].numKids = 1;

	grammerRules[7].LHS = 3;
	grammerRules[7].RHS[0] = 7;
	grammerRules[7].numKids = 1;

	grammerRules[8].LHS = 3;
	grammerRules[8].RHS[0] = 8;
	grammerRules[8].numKids = 1;

	grammerRules[9].LHS = 4;
	grammerRules[9].RHS[0] = 35;
	grammerRules[9].RHS[1] = 27;
	grammerRules[9].RHS[2] = 5;
	grammerRules[9].numKids = 3;

	grammerRules[10].LHS = 5;
	grammerRules[10].RHS[0] = 28;
	grammerRules[10].numKids = 1;

	grammerRules[11].LHS = 5;
	grammerRules[11].RHS[0] = 13;
	grammerRules[11].numKids = 1;

	grammerRules[12].LHS = 6;
	grammerRules[12].RHS[0] = 29;
	grammerRules[12].RHS[1] = 24;
	grammerRules[12].RHS[2] = 10;
	grammerRules[12].RHS[3] = 25;
	grammerRules[12].numKids = 4;

	grammerRules[13].LHS = 7;
	grammerRules[13].RHS[0] = 30;
	grammerRules[13].RHS[1] = 17;
	grammerRules[13].RHS[2] = 1;
	grammerRules[13].numKids = 3;

	grammerRules[14].LHS = 8;
	grammerRules[14].RHS[0] = 21;
	grammerRules[14].RHS[1] = 17;
	grammerRules[14].RHS[2] = 1;
	grammerRules[14].RHS[3] = 9;
	grammerRules[14].numKids = 4;

	grammerRules[15].LHS = 9;
	grammerRules[15].RHS[0] = 32;
	grammerRules[15].RHS[1] = 17;
	grammerRules[15].RHS[2] = 1;
	grammerRules[15].RHS[3] = 9;
	grammerRules[15].numKids = 4;

	grammerRules[16].LHS = 9;
	grammerRules[16].RHS[0] = 33;
	grammerRules[16].RHS[1] = 1;
	grammerRules[16].numKids = 2;

	grammerRules[17].LHS = 9;
	grammerRules[17].numKids = 0;

	grammerRules[18].LHS = 10;
	grammerRules[18].RHS[0] = 13;
	grammerRules[18].RHS[1] = 11;
	grammerRules[18].numKids = 2;

	grammerRules[19].LHS = 10;
	grammerRules[19].numKids = 0;

	grammerRules[20].LHS = 11;
	grammerRules[20].RHS[0] = 34;
	grammerRules[20].RHS[1] = 10;
	grammerRules[20].numKids = 2;

	grammerRules[21].LHS = 12;
	grammerRules[21].numKids = 0;

	grammerRules[22].LHS = 12;
	grammerRules[22].RHS[0] = 19;
	grammerRules[22].RHS[1] = 14;
	grammerRules[22].RHS[2] = 12;
	grammerRules[22].numKids = 3;

	grammerRules[23].LHS = 13;
	grammerRules[23].RHS[0] = 14;
	grammerRules[23].RHS[1] = 12;
	grammerRules[23].numKids = 2;

	grammerRules[24].LHS = 15;
	grammerRules[24].numKids = 0;

	grammerRules[25].LHS = 15;
	grammerRules[25].RHS[0] = 20;
	grammerRules[25].RHS[1] = 16;
	grammerRules[25].RHS[2] = 15;
	grammerRules[25].numKids = 3;

	grammerRules[26].LHS = 14;
	grammerRules[26].RHS[0] = 16;
	grammerRules[26].RHS[1] = 15;
	grammerRules[26].numKids = 2;

	grammerRules[27].LHS = 16;
	grammerRules[27].RHS[0] = 18;
	grammerRules[27].numKids = 1;

	grammerRules[28].LHS = 16;
	grammerRules[28].RHS[0] = 18;
	grammerRules[28].numKids = 1;

	grammerRules[29].LHS = 17;
	grammerRules[29].RHS[0] = 24;
	grammerRules[29].RHS[1] = 13;
	grammerRules[29].RHS[2] = 25;
	grammerRules[29].numKids = 3;

	grammerRules[30].LHS = 18;
	grammerRules[30].RHS[0] = 35;
	grammerRules[30].numKids = 1;

	grammerRules[31].LHS = 18;
	grammerRules[31].RHS[0] = 36;
	grammerRules[31].numKids = 1;

	grammerRules[32].LHS = 18;
	grammerRules[32].RHS[0] = 37;
	grammerRules[32].numKids = 1;

	grammerRules[33].LHS = 18;
	grammerRules[33].RHS[0] = 38;
	grammerRules[33].numKids = 1;

	grammerRules[34].LHS = 19;
	grammerRules[34].RHS[0] = 39;
	grammerRules[34].numKids = 1;

	grammerRules[35].LHS = 19;
	grammerRules[35].RHS[0] = 40;
	grammerRules[35].numKids = 1;

	grammerRules[36].LHS = 20;
	grammerRules[36].RHS[0] = 41;
	grammerRules[36].numKids = 1;

	grammerRules[37].LHS = 20;
	grammerRules[37].RHS[0] = 42;
	grammerRules[37].numKids = 1;

	grammerRules[38].LHS = 20;
	grammerRules[38].RHS[0] = 43;
	grammerRules[38].numKids = 1;

	grammerRules[39].LHS = 11;
	grammerRules[39].numKids = 0;	
}

void Parser::populate_LLmatrix() {
	//the output is the rules index for grammerRules 
	table[Pgm][kwdprog] = 1;
	table[Pgm][eof] = 1;

	table[Block][bracket1] = 2;

	table[Stmts][kwdprint] = 3;
	table[Stmts][kwdif] = 3;
	table[Stmts][kwdwhile] = 3;
	table[Stmts][ident] = 3;

	table[Stmts][bracket2] = 4;

	table[Stmt][ident] = 5;

	table[Stmt][kwdprint] = 6;

	table[Stmt][kwdwhile] = 7;

	table[Stmt][kwdif] = 8;	

	table[Astmt][ident] = 9;

	table[Y][kwdinput] = 10;

	table[Y][parens1] = 11;
	table[Y][ident] = 11;
	table[Y][integer] = 11;
	table[Y][floatInt] = 11;
	table[Y][stringType] = 11;

	table[Ostmt][kwdprint] = 12;

	table[Wstmt][kwdwhile] = 13;

	table[Fstmt][kwdif] = 14;

	table[Else2][kwdelseif] = 15;

	table[Else2][kwdelse] = 16;

	table[Else2][semi] = 17;

	table[Elist][ident] = 18;
	table[Elist][integer] = 18;
	table[Elist][floatInt] = 18;
	table[Elist][stringType] = 18;
	table[Elist][parens1] = 18;

	table[Elist][parens2] = 19;

	table[Elist2][comma] = 20;

	table[X][semi] = 21;
	table[X][comma] = 21;
	table[X][parens2] = 21;

	table[X][idType::plus] = 22;
	table[X][idType::minus] = 22;	

	table[E][parens1] = 23;
	table[E][ident] = 23;
	table[E][integer] = 23;
	table[E][floatInt] = 23;
	table[E][stringType] = 23;

	table[Z][idType::plus] = 24;
	table[Z][idType::minus] = 24;
	table[Z][parens2] = 24;
	table[Z][comma] = 24;
	table[Z][semi] = 24;

	table[Z][aster] = 25;
	table[Z][slash] = 25;
	table[Z][caret] = 25;

	table[T][ident] = 26;
	table[T][integer] = 26;
	table[T][floatInt] = 26;
	table[T][stringType] = 26;
	table[T][parens1] = 26;

	table[F][ident] = 27;
	table[F][integer] = 27;
	table[F][floatInt] = 27;
	table[F][stringType] = 27;

	table[F][parens1] = 28;

	table[Pexpr][parens1] = 29;

	table[Fatom][ident] = 30;

	table[Fatom][integer] = 31;

	table[Fatom][floatInt] = 32;

	table[Fatom][stringType] = 33;

	table[Opadd][idType::plus] = 34;

	table[Opadd][idType::minus] = 35;

	table[Opmul][aster] = 36;

	table[Opmul][slash] = 37;

	table[Opmul][caret] = 38;	

	table[Elist2][parens2] = 39;
}

string Parser::nonTerm(Non_Terminals x ) {
	switch (x)
	{
	case Pgm: 					return "Pgm";
	case Block: 				return "Block";
	case Stmts: 				return "Stmts";
	case Stmt: 					return "Stmt";
	case Astmt:					return "Astmt";
	case Y: 					return "Y";
	case Ostmt: 				return 	"Ostmt";
	case Wstmt:					return	"Wstmt";
	case Fstmt: 				return	"Fstmt";
	case Else2: 				return	"Else2";
	case Elist: 				return 	"Elist";
	case Elist2: 				return 	"Elist2";
	case X: 					return	"X";
	case E: 					return 	"E";
	case T: 					return	"T";
	case Z: 					return 	"Z";
	case F: 					return 	"F";
	case Pexpr: 				return	"Pexpr";
	case Fatom: 				return	"Fatom";
	case Opadd: 				return 	"Opadd";
	case Opmul: 				return 	"OpMul";
	default: return "terminal";
	}
}
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
	//add the pgm rule to stack 
	

	/*while(stack not empty) {
		//tracker points to top of stack 

		//check top of stack if terminal or non terminal : 
		symArray[tracker->sym].isTerm == true;
		 if (is terminal) {
			if(symArray[tracker->sym].idterm =/= tokenStream[current]) {
				return error;
			}
			else {
				if (symArray[tracker->sym].idterm == ident) {
					add to symbol table 
				}
				pop off stack;
				current++; //int for the tokenStream increases to next token 
			}
		 } //end of terminal top of stack 
		 else if (non terminal) {
			//Look up in LL matrix :
			int rule = table[symArray[tracker->sym].idnon][tokenArray[current].type];

			if (rule = 0) 
				return error; 
			
			//look up rule children 
			grammarRules[rule].numKids
			if (grammarRules[rule].numKids == 0 ) 
				pop off stack; 
			else {
				pop off stack 
				for ( int i = 0 ; i <grammarRules[rule].numKids; i++ ) {
					tracker.kids[i] = new node; 
					tracker.kids[i].sym = grammarRules[rule].RHS[i];
					tracker.kids[i].numofkids = 0; 
				} 
				//add onto stack in reserse order 
				for (int i = grammarRules[rule].numKids-1; i >= 0; i-- ) {
					add onto stack (tracker.kids[i]);
				}

				tracker = top of stack; 
			}
		 }//end non terminal 

		 if(input empty AND stack not empty) 
			return error; 		
	} //end while

	if(input not empty and stack is empty) 
		return error; 

	cout << "No errors, grammar fine! PST created" << endl; 

	*/
}

Parser::Parser() {
	populate_symbols();
	populate_rules();
	populate_LLmatrix();
	//add eof to the end of tokenStream 
}

Parser::~Parser() {
	delete[] symArray; 
	delete[] grammerRules;
	delete[] tokenStream;
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
	symArray[22].idterm = brace1;
	symArray[23].idterm = brace2;
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
	grammerRules[12].RHS[2] = 13;
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
	
}

void Parser::populate_LLmatrix() {
	//the output is the rules index for grammerRules 
	table[Pgm][kwdprog] = 1;
	table[Block][brace1] = 2;

	table[Stmts][kwdprint] = 3;
	table[Stmts][kwdif] = 3;
	table[Stmts][kwdwhile] = 3;
	table[Stmts][ident] = 3;

	table[Stmts][brace2] = 4;	
	table[Stmts][eof] = 4;

	table[Stmt][ident] = 5;

	table[Stmt][kwdprint] = 6;

	table[Stmt][kwdwhile] = 7;

	table[Stmt][kwdif] = 8;	

	table[Astmt][ident] = 9;

	table[Y][kwdinput] = 10;

	table[Y][ident] = 11;
	table[Y][integer] = 11;
	table[Y][floatInt] = 11;
	table[Y][stringType] = 11;

	table[Ostmt][kwdprint] = 12;

	table[Wstmt][kwdwhile] = 13;

	table[Fstmt][kwdif] = 14;

	table[Else2][kwdelseif] = 15;

	table[Else2][kwdelse] = 16;

	table[Else2][eof] = 17;	

	table[Elist][ident] = 18;
	table[Elist][integer] = 18;
	table[Elist][floatInt] = 18;
	table[Elist][stringType] = 18;

	table[Elist][parens2] = 19;

	table[Elist2][comma] = 20;

	table[X][parens2] = 21;
	table[X][comma] = 21;
	table[X][eof] = 21;

	table[X][idType::plus] = 22;
	table[X][idType::minus] = 22;	

	table[E][ident] = 23;
	table[E][integer] = 23;
	table[E][floatInt] = 23;
	table[E][stringType] = 23;

	table[Z][idType::plus] = 24;
	table[Z][idType::minus] = 24;
	table[Z][eof] = 24;

	table[Z][aster] = 25;
	table[Z][slash] = 25;
	table[Z][caret] = 25;

	table[T][ident] = 26;
	table[T][integer] = 26;
	table[T][floatInt] = 26;
	table[T][stringType] = 26;

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
}
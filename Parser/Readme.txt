# CPSC323_Project2
323 - Building a Parser Project 

Folder Name to turn in: 323-p1_NRC

Team Members: 
Naveena Chilukuri   Email: naveenac@csu.fullerton.edu
Rocio Salguero      Email: salgueroroci@csu.fullerton.edu
Cameron:            Email: haipayazoo@gmail.com


File Lists: 
	Source code: 
		Lexer.cpp - tokenizes an array of strings
		Lexer.h 
		main.cpp - Main function that reads, uses Lexer, and Parser to create PST and AST. 
		Parser.cpp - inherits the Lexer class and takes the Lexer array tokens as input. 
		Parser.h 
		
	Folder - Tests - 
		txt files which have the inputed tests and outputted run time tests testing out the PST and AST outputs 
	
	DOCX File - Rules and FirstFollow.docx
		File that has all the simple rules (numbered) and the first sets/follow sets that belong to that rules
		
	LLmatrix.xlsx - Excel file that has out LL Parse table.
	
	Used Visual Studio IDE. Known bug with other IDE is our enum type idType::plus does not work with other IDEs outside of visual studio. 
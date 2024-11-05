#ifndef LEXER_H
#define LEXER_H

#include "Built_In.h"
#include"ConsolePrintUtil.h"
#include "TreeUtil.h"

class Lexer {

public:
	/*breaks code into individual words*/
	static StrVector lex(String);

	static StringTree* lexTree(StrVector);

	/*strictly for debugging purposes!*/
	static void printLex(const StrVector&);

	/*returns true if c is " ", "\t" or "\n" */
	static bool isSep(char);

};

#endif // !LEXER_H
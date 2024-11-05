#ifndef PARSER_H
#define PARSER_H

#include"Built_In.h"
#include"TreeUtil.h"
//#include"Lexer.h"


class Parser {
public:
	static Tree* parse(StringNode*);

	static void printTree(Tree);
};

#endif // !PARSER_H
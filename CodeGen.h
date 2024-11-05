#ifndef CODEGEN_H
#define CODEGEN_H

#include "Built_In.h"
#include "TreeUtil.h"

class CodeGen {

private:
	static String _starterCode;

public:
	const String outputFilePath;
	const Tree* parseTree = nullptr;
    String _genCodeString;/*should be private!*/

public:
	CodeGen(Tree* _parsedTree, const String& _outFileName)
		: outputFilePath(_outFileName), parseTree(_parsedTree)
        , _genCodeString(_starterCode)
	{}

	static void generateCppCode(Node* _node, String& _genCodeString);
};

#endif
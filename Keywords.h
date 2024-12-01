#ifndef KEYWORDS_H
#define KEYWORDS_H

#include"Built_In.h"

static StrVector keywords = {

	"int", "float", "char", "bool", "string"/*, "ptr"*/ /*primitive data types*/ /*i decided that pointer support will be added later!*/

	/*"static"*/, "const", "as", /*"such_that",*/ /*instead of using static keyword, i will declare variables starting with $ as static*/

	"true", "false", /*keywords for boolean states*/

	"if", "else", "switch", "case", "default", "until", "iterate", "break", "continue", /*conditionals and loops*/ /* "for", "while", "do" are replaced with "until" & "iterate"*/

	"void", "null", "return", "returns",

	"define", "declare", "class", "function", "operator", "behavior", /*definitions*/

	/*"private", "protected", "public",*//*i changed my mind, now access modifier will be of python style i.e, var, _var & __var*/

	"it", /*"they",*/ "is",

	"try", "catch", "raise", "beware",

	"invite" /*for importing*/,

	"cpp", "end_cpp" /*for calling C++ code*/

};

static bool isKwrd(const String& _word) {
	for (String _k : keywords) {
		if (_k == _word) {
			return true;
		}
	}
	return false;
}


#endif // !KEYWORDS_H
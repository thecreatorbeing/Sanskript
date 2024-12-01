#ifndef CODETYPE_H
#define CODETYPE_H

enum class CodeType : char {
	/*Statement types*/
	INVITE_STMT = 1, /*have 'invite' keyword*/
	VAR_DEC_STMT = 2, /*have 'declare' keyword*/
	CLASS_DEC_STMT = 3,
	FUN_DEC_STMT = 4,
	BEH_DEC_STMT = 5,
	OPR_DEC_STMT = 6,
	ASSIGNMENT_STMT = 7, /*have '=' and its variants*/
	EXPRESSION_STMT = 8, /*have operators and operand/s*/
	RETURN_STMT = 9, /*have 'return' keyword*/
	FUN_CALL_STMT = 10, /*have function literal*/
	EMPTY_STMT = 11, /*Ignore this during translation*/
	RAISE_ERR_STMT = 12, /*starts with 'raise' keyword*/

	/*Block (multiple statements) types*/
	CLASS_DEF_BLK = 13, /*starts with 'define' & 'class' keywords*/
	FUN_DEF_BLK = 14, /*starts with 'define' & 'function' keywords*/
	BEH_DEF_BLK = 15, /*starts with 'define' & 'behavior' keywords*/
	OPR_DEF_BLK = 16, /*starts with 'define' & 'operator' keywords*/
	IF_BLK = 17, /*starts with 'if' keyword*/
	ELSE_IF_BLK = 18, /*starts with 'else' followed by 'if' keywords*/
	ELSE_BLK = 19, /*starts with 'else' keyword*/
	SWITCH_BLK = 20, /*starts with 'switch' keyword*/
	LOOP_BLK = 21, /*starts with 'until' & 'iterate' keywords*/
	REV_LOOP_BLK = 22, /*starts with 'iterate' & 'until' keywords*/
	TRY_BLK = 23, /*starts with 'try' keyword*/
	CATCH_BLK = 24, /*starts with 'catch' keyword*/

	STRING_LITERAL = 25, /*surrounded by double quotes!*/
	COMMENT_BLK = 26, //surrounded by /* and */

	CPP_BLK = 27
};

#endif // !CODETYPE_H
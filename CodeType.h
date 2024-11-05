#ifndef CODETYPE_H
#define CODETYPE_H

enum class CodeType: char{
	/*Statement types*/
	INVITE_STMT = 1, /*have 'invite' keyword*/
	VAR_DEC_STMT = 2, /*have 'define' keyword*/
	ASSIGNMENT_STMT = 3, /*have '=' and its variants*/
	EXPRESSION_STMT = 4, /*have operators and operand/s*/
	RETURN_STMT = 5, /*have 'return' keyword*/
	FUN_CALL_STMT = 6, /*have function literal*/
	EMPTY_STMT = 7, /*Ignore this during translation*/
	RAISE_ERR_STMT = 8, /*starts with 'raise' keyword*/

	/*Block (multiple statements) types*/
	CLASS_DEF_BLK = 9, /*starts with 'define' & 'class' keywords*/
	FUN_DEF_BLK = 10, /*starts with 'define' & 'function' keywords*/
	BEH_DEF_BLK = 11, /*starts with 'define' & 'behavior' keywords*/
	OPR_DEF_BLK = 12, /*starts with 'define' & 'operator' keywords*/
	IF_BLK = 13, /*starts with 'if' keyword*/
	ELSE_IF_BLK = 14, /*starts with 'else' followed by 'if' keywords*/
	ELSE_BLK = 15, /*starts with 'else' keyword*/
	SWITCH_BLK = 16, /*starts with 'switch' keyword*/
	LOOP_BLK = 17, /*starts with 'until' & 'iterate' keywords*/
	REV_LOOP_BLK = 18, /*starts with 'iterate' & 'until' keywords*/
	TRY_BLK = 19, /*starts with 'try' keyword*/
	CATCH_BLK = 20, /*starts with 'catch' keyword*/

	STRING_LITERAL = 21, /*surrounded by double quotes!*/
	COMMENT_BLK = 22 //surrounded by /* and */
};

#endif // !CODETYPE_H
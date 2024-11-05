#ifndef EXPRESSION_H
#define EXPRESSION_H

#include"Built_In.h"
#include"Global.h"
#include"Keywords.h"
#include"SymbolTable.h"

enum class Expr : char {
	LITERAL = 50, /*Literals like 1, 'A', "abcd", 3.142*/
	VARIABLE = 51, /*must be an entry in variable's symbol table*/
	FUNCTION = 52, /*must be an entry in function's symbol table*/
	OPERATOR = 53, /*either +,-,/,* or must be an entry in operator's symbol table*/
	OPEN_PAREN = 54,
	CLOSE_PAREN = 55,
	OPERAND = 56
};

enum Literal { NOTHING = 79, INT = 80, FLOAT = 81, BOOL = 82, CHAR = 83, STRING = 84 };

static bool isValidIdentifier(const String&);
static StrVector isFunction(const String&);
static bool isOperator(const String&);
static bool isBalanced(const String&);
static Literal isValidLiteral(const String&);
static bool isVariable(const String&);
static int precedence(const String&);
static StrVector _breakExpr(String);
static bool _verifyExpr(const StrVector&);
static StrVector infixToPostfix(const String&);
static String findReturnType(const StrVector&);
static bool isValidExpr(const String&);

/*check string against the identifier rules ($ allowed)*/
bool isValidIdentifier(const String& identifier) {

	//Print::warning(identifier);
	// Check if the string is empty
	if (identifier.empty()) {
		return false;
	}

	// The first character must be a letter or underscore or $
	if (!isalpha(identifier[0]) && identifier[0] != '_' && identifier[0] != '$') {
		return false;
	}

	// Loop through remaining characters and check if they are alphanumeric or underscore
	for (size_t i = 1; i < identifier.size(); ++i) {
		if (!isalnum(identifier[i]) && identifier[i] != '_') {
			return false;
		}
	}

	if (identifier.find("_$") != -1) {
		//Print::error("identifier \"" + identifier + "\" consists '_' after '$' which is not allowed!");
		Global::errorFlag = true;
		return false;
	}

	// Identifier cannot be a keyword
	for (String k : keywords) {
		if (k == identifier) {
			return false;
		}
	}

	return true;
}

/*checks if string exists in function symbol table having same param types
returns: string vector of param datatypes stored in function symbol table

usage: if you want to check whether function is valid then check:
1) returned vector is not empty(it can be empty for no params)
2) global::errorFlag is not true.

	update: i suspect this function cannot handle the private functions being called outside the class and other similar problem
	also, we cannot call functions like obj.fun() and Class.createObject() i.e, using period('.')*/
StrVector isFunction(const String& _funCall) {

	bool _funNameFlag = false, _funParamsFlag = false;
	const int _firstParenIndex = _funCall.find_first_of('(');
	String _funName = _funCall.substr(0, _firstParenIndex);

	if (!isBalanced(_funCall)) {
		Global::errorFlag = true;
		Print::error("parenthesis mismatch in function: " + _funCall);
		return {/*empty vector*/ };
	}

	if (_firstParenIndex == -1 || _funCall[_funCall.size() - 1] != ')') {
		Global::errorFlag = true;
		Print::error(_funCall + " is not a function!");
		return {/*empty vector*/ };
	}

	if (!isValidIdentifier(_funName)) {
		Print::error(_funName + " is not a valid identifier!");
		Global::errorFlag = true;
		return {/*empty vector*/ };
	}

	for (const Function& _f : Function::symbolTable) {/*checks funName's existance in the symbol table*/
		if (_f.identifier == _funName) {
			_funNameFlag = true; break;
		}
	}
	if (!_funNameFlag) { return {/*empty vector*/ }; }/*if there's no function of said name in the function symbol table then simple return false!*/

	std::stack<char> _stk;
	String _tempParam = "";
	StrVector _funParams;

	const String _paramData = _funCall.substr(_firstParenIndex + 1, _funCall.size() - _firstParenIndex - 2) + " ";
	/*space at end added due to experienced adjustment! remove it & observe funParams vector (at function end) if you wanted to explore more!*/

	char _c;
	for (int _i = 0; _i < _paramData.size(); _i++) {
		_c = _paramData[_i];

		//std::cout << _c << "\n";
		if (_c == '(') {
			_stk.push('(');
			//std::cout << "Pushed!\n";
			_tempParam += _c;
		}
		else if (_c == ')') {
			_stk.pop();
			//std::cout << "Popped!\n";
			_tempParam += _c;
		}
		else if ((_c == ',' || _i == _paramData.size() - 1) && _stk.empty()) { /*function params separator comma approached*/
			if (_tempParam.empty() || _tempParam == "\t" || _tempParam == "\n" || _tempParam == " ") {
				//std::cout << "error: param is empty!" << std::endl;
				Print::error("function param is empty");
				break;
			}
			else {
				_funParams.push_back(_tempParam);
				//std::cout << "pushed: |" << _tempParam << "|\n";
				_tempParam.clear();
			}
		}
		else {
			_tempParam += _c;
		}
	}

	String _param;
	for (int i = 0; i < _funParams.size(); i++) {
		if (!isValidExpr(_param)) {
			_funParamsFlag = false;
			break;
		}
		else { _funParamsFlag = true; }
	}
	_param.clear();

	/*TODO: create function to analyze the return type of expressions of funparams vector and compare it with function params vector in function symbol table!*/

	/*std::cout << "fun params:\n";
	for (string _ : _funParams) {
		std::cout << "|" << _ << "|\n";
	}
	std::cout << std::endl;*/

	return {}; //_funNameFlag&& _funParamsFlag;
}

/*returns true if _oper is an operator*/
bool isOperator(const String& _oper) {

	/*basic operators are now being added at the start*/
	//if (_oper == "+" || _oper == "*" || _oper == "-" || _oper == "/" || _oper == "%") { return true; }

	return Operator::contains(_oper);
}

/*static bool isBalancedParenthesis(const String& _expression) {
	std::stack<char> parentheses;

	for (char c : _expression) {
		if (c == '(') {
			parentheses.push(c);
		}
		else if (c == ')') {
			if (parentheses.empty() || parentheses.top() != '(') {
				return false; // Mismatched closing parenthesis
			}
			parentheses.pop();
		}
	}

	// Check if all opening parentheses have a matching closing parenthesis
	return parentheses.empty();
}*/
/*parenthesis matching for ( and [ brackets Update: add support for { when array literals will be allowed within expressions*/
bool isBalanced(const String& _expression) {
	std::stack<char> parenStack;

	for (const char ch : _expression) {
		// Outside literal, handle parentheses
		if (ch == '(' || ch == '[') {
			parenStack.push(ch);
		}
		else if (ch == ')' || ch == ']') {
			if (parenStack.empty()) {
				return false; // Unmatched closing parenthesis
			}
			char openingBracket = (ch == ')') ? '(' : '[';
			if (parenStack.top() != openingBracket) {
				return false; // Mismatched parentheses
			}
			parenStack.pop();
		}
	}
	// Check if all parentheses are matched
	return parenStack.empty();
}

/*checks for int, float, bool, char & string*/
Literal isValidLiteral(const String& _literal) {
	if (_literal.empty()) {
		return Literal::NOTHING; // Empty string is not a valid literal
	}

	// Regular expressions for different literal types
	std::regex integer_regex("^-?[0-9]+$");
	std::regex float_regex("^-?[0-9]+\\.[0-9]+([eE][+-]?[0-9]+)?$");

	// Match against regular expressions
	if (std::regex_match(_literal, integer_regex)) {
		//std::cout << "int" << std::endl;
		return Literal::INT;
	}
	else if (std::regex_match(_literal, float_regex)) {
		//std::cout << "float" << std::endl;
		return Literal::FLOAT;
	}
	else if (_literal.starts_with("'")) {
		//std::cout << "char" << std::endl;
		return Literal::CHAR;
	}
	else if (_literal.starts_with("\"")) {
		//std::cout << "string" << std::endl;
		return Literal::STRING;
	}
	else if (_literal == "true" || _literal == "false") {
		return Literal::BOOL;
	}

	//std::cout << "mismatch" << std::endl;
	return Literal::NOTHING;
}

/*checks if string exists in variable symbol table and within the scope*/
bool isVariable(const String& _var) {
	for (const Variable& _v : Variable::symbolTable) {
		if (_v.identifier == _var) {
			if (_v.isScopeEnded) {
				Print::error("Variable: '" + _var + "' is out of scope");
				Global::errorFlag = true;
				return false;
			}
			return true;
		}
	}

	Print::error("Variable: '" + _var + "' doesn't exist");
	Global::errorFlag = true;
	return false;
}

// Function to get precedence of an operator
int precedence(const String& _oper) {
	if (_oper == "+" || _oper == "-") {
		return 2;
	}
	else if (_oper == "*" || _oper == "/" || _oper == "%") {
		return 3;
	}
	else {
		return 1;
	}
}

/*breaks expression into vector of strings, passed expression must be parenthesis matched!*/
StrVector _breakExpr(String _expr) {

	_expr += ' ';/*this is one of the most important thing i learned while making Sanskript,
	it helps me to reduce 1 conditional inside the below for loop which was for checking whether the current iteration is last or not*/

	StrVector __exprVec;
	String __tempStr;

	//std::cout << "input is: |" << _expr << "|\n";

	for (const char& _c : _expr) {

		//std::cout << "c is: |" << _c << "|\n";

		switch (_c) {
		case '+':
		case '-':
		case'*':
		case '/':
		case '%':
		case '(':
		case ')':
			if (!__tempStr.empty()) {
				__exprVec.push_back(__tempStr);
				__tempStr.clear();
			}
			__tempStr += _c;
			if (!__tempStr.empty()) {
				__exprVec.push_back(__tempStr);
				__tempStr.clear();
			}
			continue;
		}

		if (_c == ' ' || _c == '\t' || _c == '\n') {
			if (!__tempStr.empty()) {
				__exprVec.push_back(__tempStr);
				__tempStr.clear();
			}
			continue;
		}
		else {
			__tempStr += _c;
		}

	}
	//for (auto& _ : __exprVec) { std::cout << "|" << _ << "|\n"; }

	StrVector _outputVec;
	for (size_t i = 0; i < __exprVec.size(); i++) {
		if (!isFunction(__exprVec[i]).empty()) {
			if (i == __exprVec.size() - 1 || __exprVec[i + 1] != "(") { std::cout << "function '(' error" << std::endl; return {}; }

			String __funStr = __exprVec[i];
			std::stack<String> _parenStk;
			for (size_t j = i + 1; j < __exprVec.size(); j++) {
				//std::cout << __funStr << "\n";
				if (__exprVec[j] == "(") {
					__funStr += '(';
					_parenStk.push("(");
				}
				else if (__exprVec[j] == ")") {
					__funStr += ')';
					_parenStk.pop();

					if (_parenStk.empty()) {
						_outputVec.push_back(__funStr);
						//std::cout << "added: |" << __funStr << "|\n";
						__funStr.clear();
						i = j;
						break;
					}
				}
				else {
					__funStr += __exprVec[j];
				}
			}
		}
		else {
			//std::cout << __exprVec[i] << " is not a function" << std::endl;
			if (i < __exprVec.size() - 1 && __exprVec[i + 1] == "(" && __exprVec[i] != "(" && !isOperator(__exprVec[i])) {
				std::cout << "undefined function name: " << __exprVec[i] << std::endl; return {/*empty array(vector)*/ };
			}
			else {
				_outputVec.push_back(__exprVec[i]);
			}
		}
	}

	/*std::cout << "output: |";
	for (const String& _ : _outputVec) {
		std::cout << _ << "|";
	}
	std::cout << std::endl;*/

	return _outputVec;
}

bool _verifyExpr(const StrVector& _expr) {
	if (_expr.empty()) { return false; }

	std::vector<Expr> _expVec;
	for (const String& _e : _expr) {
		if (isOperator(_e)) {
			_expVec.push_back(Expr::OPERATOR);
		}
		else if (_e == "(") { _expVec.push_back(Expr::OPEN_PAREN); }
		else if (_e == ")") { _expVec.push_back(Expr::CLOSE_PAREN); }
		else {
			_expVec.push_back(Expr::OPERAND);/*n for not-operator, yeah i am still not in the mood to use the enum!*/
		}
	}

	Expr __curr, __nxt;
	for (size_t i = 0; i < _expVec.size() - 1; i++)/*this loops until 2nd last element*/
	{
		__curr = _expVec[i]; __nxt = _expVec[i + 1];
		if (__curr == Expr::OPERATOR) {/*operator*/
			if (__nxt == Expr::OPERATOR) {/*consecutive operators!*/
				//std::cout << "two consecutive operators in expression" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
			else if (__nxt == Expr::CLOSE_PAREN) {/*who uses ')' after an operator? not humans at least!*/
				//std::cout << ") is not allowed after an operator" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
		}
		else if (__curr == Expr::OPERAND) {/*operand*/
			if (__nxt == Expr::OPERAND) {/*consecutive operands!*/
				//std::cout << "two consecutive operands in expression" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
			else if (__nxt == Expr::OPEN_PAREN) {/* "operand (" is not allowed*/
				//std::cout << "( after an operand is not allowed in expression" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
		}
		else if (__curr == Expr::OPEN_PAREN) {
			if (__nxt == Expr::OPERATOR) {/*"( operator" is also not allowed*/
				//std::cout << "operator after ( is not allowed in expression" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
			else if (__nxt == Expr::CLOSE_PAREN) {/*"( )" is also not allowed, except functions*/
				//std::cout << "empty '(' and ')' are not allowed except functions" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
		}
		else if (__curr == Expr::CLOSE_PAREN) {
			if (__nxt == Expr::OPERAND) {/* ") operand" is meaningless!*/
				//std::cout << "\") operator\" is not allowed" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
			else if (__nxt == Expr::OPEN_PAREN) {/*")(" is not allowed!*/
				//std::cout << "\") & (\" is not allowed!" << std::endl;
				Global::errorFlag = true;
				Print::error("invalid expression!");
				return false;
			}
		}
	}
	return true;
}

//converts infix expression into postfix
StrVector infixToPostfix(const String& _infix) {

	StrVector _infixVec = _breakExpr(_infix);
	if (_infixVec.empty()) { std::cout << "empty expression has been passed, since breakExpr returned empty!" << std::endl; }
	if (!_verifyExpr(_infixVec)) { std::cout << "error" << std::endl; return {/*empty vector*/ }; }

	std::stack<String> _s;
	StrVector _postfix;

	std::cout << "input: |" << _infix << "|\n";

	for (int __i = 0; __i < _infixVec.size(); __i++) {
		const String& _token = _infixVec[__i];
		//std::cout << "token:|" << _token << "|\n";

		if (isOperator(_token)) {
			//std::cout << "isOper\n";
			while (!_s.empty() && precedence(_token) <= precedence(_s.top())) {
				_postfix.push_back(_s.top());
				_s.pop();
			}
			_s.push(_token);
		}
		else if (_token == "(") {
			//std::cout << "pushed (\n";
			_s.push(_token);
		}
		else if (_token == ")") {
			while (!_s.empty() && _s.top() != "(") {
				_postfix.push_back(_s.top());
				//std::cout << "postfix became :|" << _postfix << "|\n";
				_s.pop();
			}
			if (!_s.empty() && _s.top() != "(") {
				std::cout << "Invalid Expression" << std::endl;
				return {/*empty vector*/ }; // Mismatched parentheses
			}
			else {
				if (!_s.empty()) {
					//std::cout << "removed: " << _s.top() << "\n";
					_s.pop();
				}
				//else { std::cout << "stk empty\n"; }
			}
		}
		else {
			_postfix.push_back(_token);
			//std::cout << "postfix became2:|" << _postfix << "\n";
		}
	}

	while (!_s.empty()) {
		_postfix.push_back(_s.top());
		_s.pop();
	}
	std::cout << std::endl;
	return _postfix;
}

String findReturnType(const StrVector& _expr) {/*keep in mind, return type can be any datatype primitive or non-primitive hence a string is required to be returned*/

	std::stack<String> _stk;

	for (const String _e : _expr) {
		if (isOperator(_e)) {/*i have checked this thing already in previously called functions such as infixToPostfix(),
			so calling it again is not optimal, but expressions are generally not very long, so it will compromise the performance
			only a little bit*/
			/*TODO: do it!*/

		}
		else {
			char _temp = isValidLiteral(_e);
			String e_datatype = "";
			if (_temp != Literal::NOTHING) {
				switch (_temp) {
				case Literal::BOOL:
					e_datatype = "bool"; break;
				case Literal::CHAR:
					e_datatype = "char"; break;
				case Literal::FLOAT:
					e_datatype = "float"; break;
				case Literal::INT:
					e_datatype = "int"; break;
				case Literal::STRING:
					e_datatype = "string"; break;
				default:
					break;
				}
			}
			else if (!isFunction(_e).empty()) {
				//e_datatype = Function::getReturnType(_e, );
			}


			_stk.push(e_datatype);/*push _e's datatype instead*/
		}
	}

	return "";
}

/*update: this function currently only supports single values, not complex expressions. Modify it to do so!*/
bool isValidExpr(const String& _expr) {

	if (isValidLiteral(_expr) != Literal::NOTHING) { return true; }
	else if (isVariable(_expr)) { return true; }
	//else if (isFunction(_expr)) { return true; } uncomment

	/*for (const char _ : _expr) {
		if (_ == '"' || _ == '\'') {
			Print::error("string or char literals are not allowed in an expression");
			Global::errorFlag = true;
			return false;
		}
		else if (_ == '{' || _ == '}') {
			Print::error("{ or } are not allowed in an expression");
			Global::errorFlag = true;
			return false;
		}
	}

	if (Parse::isBalanced(_expr)) {
		return true;
	}

	StrVector _expVector;
	string _temp = "";
	for (char _c : _expr) {
		if (_c == ' ') {
			_temp += _c;
		}
		else {
			_expVector.push_back(_temp);
			_temp.clear();
		}
	}*/

	return false;
}

/*----------------------------------------------------------------------------------*/
class ExpressionValidator {
	const String expression;
	/*if expression returns nothing(invalid or function call) then the value is "" {I treat single function call as an expression}*/
	const String returnType;
	const bool isValid;

public:
	ExpressionValidator(const String& _expr) :
		expression(_expr),
		isValid(_isValidExpression(_expr)),
		returnType(_findReturnType(_expr))
	{}

	static bool _isValidExpression(const String& _expr) {

		Print::warning("expression is: |" + _expr + "|");

		if (__isValidLiteral(_expr) != Literal::NOTHING) { return true; }
		else if (__isVariable(_expr)) { return true; }
		else if (!__isFunction(_expr).empty() && !Global::errorFlag) { /*Print::log("this one!");*/ return true; }

		if (__isBalanced(_expr)) { return true; }
	}

	static String _findReturnType(const String& _expr) {
		return "LOL!";
	}

private:
	static enum class Expr : char {
		LITERAL = 50, /*Literals like 1, 'A', "abcd", 3.142*/
		VARIABLE = 51, /*must be an entry in variable's symbol table*/
		FUNCTION = 52, /*must be an entry in function's symbol table*/
		OPERATOR = 53, /*either +,-,/,* or must be an entry in operator's symbol table*/
		OPEN_PAREN = 54,
		CLOSE_PAREN = 55,
		OPERAND = 56
	};
	static enum Literal { NOTHING = 79, INT = 80, FLOAT = 81, BOOL = 82, CHAR = 83, STRING = 84 };

	/*checks for int, float, bool, char & string*/
	static Literal __isValidLiteral(const String& _literal) {
		if (_literal.empty()) {
			return Literal::NOTHING; // Empty string is not a valid literal
		}

		// Regular expressions for different literal types
		std::regex integer_regex("^-?[0-9]+$");
		std::regex float_regex("^-?[0-9]+\\.[0-9]+([eE][+-]?[0-9]+)?$");

		// Match against regular expressions
		if (std::regex_match(_literal, integer_regex)) {
			//std::cout << "int" << std::endl;
			return Literal::INT;
		}
		else if (std::regex_match(_literal, float_regex)) {
			//std::cout << "float" << std::endl;
			return Literal::FLOAT;
		}
		else if (_literal.starts_with("'")) {
			//std::cout << "char" << std::endl;
			return Literal::CHAR;
		}
		else if (_literal.starts_with("\"")) {
			//std::cout << "string" << std::endl;
			return Literal::STRING;
		}
		else if (_literal == "true" || _literal == "false") {
			return Literal::BOOL;
		}

		//std::cout << "mismatch" << std::endl;
		return Literal::NOTHING;
	}

	/*check string against the identifier rules ($ allowed)*/
	static bool isValidIdentifier(const String& identifier) {

		//Print::warning(identifier);
		// Check if the string is empty
		if (identifier.empty()) {
			return false;
		}

		// The first character must be a letter or underscore or $
		if (!isalpha(identifier[0]) && identifier[0] != '_' && identifier[0] != '$') {
			return false;
		}

		// Loop through remaining characters and check if they are alphanumeric or underscore
		for (size_t i = 1; i < identifier.size(); ++i) {
			if (!isalnum(identifier[i]) && identifier[i] != '_') {
				return false;
			}
		}

		if (identifier.find("_$") != -1) {
			//Print::error("identifier \"" + identifier + "\" consists '_' after '$' which is not allowed!");
			Global::errorFlag = true;
			return false;
		}

		// Identifier cannot be a keyword
		if (isKwrd(identifier)) { return false; }

		return true;
	}

	/*parenthesis matching for ( and [ brackets Update: add support for { when array literals will be allowed within expressions*/
	static bool __isBalanced(const String& _expression) {
		std::stack<char> parenStack;

		for (const char ch : _expression) {
			// Outside literal, handle parentheses
			if (ch == '(' || ch == '[') {
				parenStack.push(ch);
			}
			else if (ch == ')' || ch == ']') {
				if (parenStack.empty()) {
					return false; // Unmatched closing parenthesis
				}
				char openingBracket = (ch == ')') ? '(' : '[';
				if (parenStack.top() != openingBracket) {
					return false; // Mismatched parentheses
				}
				parenStack.pop();
			}
		}
		// Check if all parentheses are matched
		return parenStack.empty();
	}

	/*update: currently, functions params are not analyzed and hence when they will be analyzed, __createCallStack function needs to be called within this function*/
	static void __validateCallStack(std::stack<String> _callStack, const String& _varOrFunCall) {

		/*reversing the stack into other temporary stack*/
		std::stack<String> _tmpStk;
		while (!_callStack.empty()) {
			_tmpStk.push(_callStack.top());
			_callStack.pop();
		}

		/*checking symbol-table to validate the call-stack*/
		while (!_tmpStk.empty()) {
			Print::log("got: " + _tmpStk.top());
			_tmpStk.pop();
		}
	}

	/* make sure the call string is not empty and must be parenthesis matched
		Update:- currently doesn't support overloaded functions since function params are not analyzed and simply ignored!
	*/
	static void __createCallStack(std::stack<String>& _callStack, String _varOrFunCall) {

		_varOrFunCall += '.';/*experienced adjustment! ... Suppose _varOrFunCall is a string "Classname.variable" then below code generates stack with |"Classname"|"variabl"|
								thats why i added a dot/period hence now it becomes |"Classname"|"variable"| */

		String _temp;
		char _c;/*char at 'i'th index in call string*/
		size_t _parenLevel = 0;/*to track whether the character is inside parenthesis or not*/
		bool _isStringEnded = true;/* "hello string!" -> observe this example and analyze it along with below code */
		for (size_t i = 0; i < _varOrFunCall.size(); i++)
		{
			_c = _varOrFunCall[i];

			///*first and foremost... ignore the string literals so that the string characters in the literal doesn't gets processed*/
			//if (_c == '"') {
			//	if (_isStringEnded) {
			//		_isStringEnded = false; /* another string literal started */
			//	}
			//	else {
			//		_isStringEnded = true; /* string literal ended */
			//	}
			//	continue;/*continue to next iteration*/
			//}
			/*-------------------------------------------------------------------------------------------------------------------*/
			/*ignore everything inside parenthesis and adding them normally into temp*/
			if (_c == '(') {
				_parenLevel++;/*since, we are ignoring everything inside parenthesis, this variable can be a bool instead of int*/
				_temp += _c;
				continue;
			}
			else if (_c == ')') {
				_parenLevel--;
				_temp += _c;
				continue;
			}
			/*------------------------------------------------------------------------------------------------------------------*/
			if (_c == '.' && _parenLevel < 1) {
				//std::cout << _parenLevel << std::endl;
				if (_temp.empty()) { 
					Print::error("Invalid Expression " + _varOrFunCall);
					Global::errorFlag = true;
				}
				else {
					_callStack.push(_temp);
					_temp.clear();
				}
			}
			else {
				_temp += _c;
			}
		}

		__validateCallStack(_callStack, _varOrFunCall);
	}

	/*checks if string exists in function symbol table having same param types
	returns: string vector of param datatypes stored in function symbol table

	usage: if you want to check whether function is valid then check:
	1) returned vector is not empty(it can be empty for no params)
	2) global::errorFlag is not true.

	update: i suspect this function cannot handle the private functions being called outside the class and other similar problem
	also, we cannot call functions like obj.fun() and Class.createObject() i.e, using period('.')*/
	static StrVector __isFunction(const String& _funCall) {

		bool _funNameFlag = false, _funParamsFlag = false;
		const int _firstParenIndex = _funCall.find_first_of('(');
		String _funName = _funCall.substr(0, _firstParenIndex);

		if (!__isBalanced(_funCall)) {
			Global::errorFlag = true;
			Print::error("parenthesis mismatch in function: " + _funCall);
			return {/*empty vector*/ };
		}

		if (_firstParenIndex == -1 || _funCall[_funCall.size() - 1] != ')') {
			Global::errorFlag = true;
			Print::error(_funCall + " is not a function!");
			return {/*empty vector*/ };
		}

		if (!isValidIdentifier(_funName)) {
			Print::error(_funName + " is not a valid identifier!");
			Global::errorFlag = true;
			return {/*empty vector*/ };
		}

		for (const Function& _f : Function::symbolTable) {/*checks funName's existance in the symbol table*/
			if (_f.identifier == _funName) {
				_funNameFlag = true; break;
			}
		}
		if (!_funNameFlag) { return {/*empty vector*/ }; }/*if there's no function of said name in the function symbol table then simple return false!*/

		std::stack<char> _stk;
		String _tempParam = "";
		StrVector _funParams;

		const String _paramData = _funCall.substr(_firstParenIndex + 1, _funCall.size() - _firstParenIndex - 2) + " ";
		/*space at end added due to experienced adjustment! remove it & observe funParams vector (at function end) if you wanted to explore more!*/

		char _c;
		for (int _i = 0; _i < _paramData.size(); _i++) {
			_c = _paramData[_i];

			//std::cout << _c << "\n";
			if (_c == '(') {
				_stk.push('(');
				//std::cout << "Pushed!\n";
				_tempParam += _c;
			}
			else if (_c == ')') {
				_stk.pop();
				//std::cout << "Popped!\n";
				_tempParam += _c;
			}
			else if ((_c == ',' || _i == _paramData.size() - 1) && _stk.empty()) { /*function params separator comma approached*/
				if (_tempParam.empty() || _tempParam == "\t" || _tempParam == "\n" || _tempParam == " ") {
					//std::cout << "error: param is empty!" << std::endl;
					Print::error("function param is empty");
					break;
				}
				else {
					_funParams.push_back(_tempParam);
					//std::cout << "pushed: |" << _tempParam << "|\n";
					_tempParam.clear();
				}
			}
			else {
				_tempParam += _c;
			}
		}

		String _param;
		for (int i = 0; i < _funParams.size(); i++) {
			if (!ExpressionValidator::_isValidExpression(_param)) {
				_funParamsFlag = false;
				break;
			}
			else { _funParamsFlag = true; }
		}
		_param.clear();

		/*TODO: create function to analyze the return type of expressions of funparams vector and compare it with function params vector in function symbol table!*/

		/*std::cout << "fun params:\n";
		for (string _ : _funParams) {
			std::cout << "|" << _ << "|\n";
		}
		std::cout << std::endl;*/

		return {}; //_funNameFlag&& _funParamsFlag;
	}

	/*returns true if _oper is an operator*/
	static bool isOperator(const String& _oper) {
		return Operator::contains(_oper);
	}

	/*checks if string exists in variable symbol table and within the scope*/
	static bool __isVariable(const String& _var) {
		/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*Below commented code snippet was useful for variables without using period('.') symbol, not usable for class or object based variable accessing*/
		/*simple example:- below code can identify 'var1' as a variable in an expression, but cannot identify 'ClassName.var' or 'object.var' as a variable*/
		/*for (const Variable& _v : Variable::symbolTable) {
			if (_v.identifier == _var) {
				if (_v.isScopeEnded) {
					Print::error("Variable: '" + _var + "' is out of scope");
					Global::errorFlag = true;
					return false;
				}
				return true;
			}
		}*/
		/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

		/*---------------------------------------------------------------------------------------------------------------------------------------------------*/
		/*That's why I added below code snippet to implement the missing functionality and commented the above code*/
		std::stack<String> callStack;/*name is self-explainatory*/
		__createCallStack(callStack, _var);

		/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

		Print::error("Variable: '" + _var + "' doesn't exist");
		Global::errorFlag = true;
		return false;
	}

	// Function to get precedence of an operator
	static int precedence(const String& _oper) {
		if (_oper == "+" || _oper == "-") {
			return 2;
		}
		else if (_oper == "*" || _oper == "/" || _oper == "%") {
			return 3;
		}
		else {
			return 1;
		}
	}

	/*breaks expression into vector of strings, passed expression must be parenthesis matched!*/
	static StrVector _breakExpr(String _expr) {

		_expr += ' ';/*this is one of the most important thing i learned while making Sanskript,
		it helps me to reduce 1 conditional inside the below for loop which was for checking whether the current iteration is last or not*/

		StrVector __exprVec;
		String __tempStr;

		//std::cout << "input is: |" << _expr << "|\n";

		for (const char& _c : _expr) {

			//std::cout << "c is: |" << _c << "|\n";

			switch (_c) {
			case '+':
			case '-':
			case'*':
			case '/':
			case '%':
			case '(':
			case ')':
				if (!__tempStr.empty()) {
					__exprVec.push_back(__tempStr);
					__tempStr.clear();
				}
				__tempStr += _c;
				if (!__tempStr.empty()) {
					__exprVec.push_back(__tempStr);
					__tempStr.clear();
				}
				continue;
			}

			if (_c == ' ' || _c == '\t' || _c == '\n') {
				if (!__tempStr.empty()) {
					__exprVec.push_back(__tempStr);
					__tempStr.clear();
				}
				continue;
			}
			else {
				__tempStr += _c;
			}

		}
		//for (auto& _ : __exprVec) { std::cout << "|" << _ << "|\n"; }

		StrVector _outputVec;
		for (size_t i = 0; i < __exprVec.size(); i++) {
			if (!__isFunction(__exprVec[i]).empty()) {
				if (i == __exprVec.size() - 1 || __exprVec[i + 1] != "(") { std::cout << "function '(' error" << std::endl; return {}; }

				String __funStr = __exprVec[i];
				std::stack<String> _parenStk;
				for (size_t j = i + 1; j < __exprVec.size(); j++) {
					//std::cout << __funStr << "\n";
					if (__exprVec[j] == "(") {
						__funStr += '(';
						_parenStk.push("(");
					}
					else if (__exprVec[j] == ")") {
						__funStr += ')';
						_parenStk.pop();

						if (_parenStk.empty()) {
							_outputVec.push_back(__funStr);
							//std::cout << "added: |" << __funStr << "|\n";
							__funStr.clear();
							i = j;
							break;
						}
					}
					else {
						__funStr += __exprVec[j];
					}
				}
			}
			else {
				//std::cout << __exprVec[i] << " is not a function" << std::endl;
				if (i < __exprVec.size() - 1 && __exprVec[i + 1] == "(" && __exprVec[i] != "(" && !isOperator(__exprVec[i])) {
					std::cout << "undefined function name: " << __exprVec[i] << std::endl; return {/*empty array(vector)*/ };
				}
				else {
					_outputVec.push_back(__exprVec[i]);
				}
			}
		}

		/*std::cout << "output: |";
		for (const String& _ : _outputVec) {
			std::cout << _ << "|";
		}
		std::cout << std::endl;*/

		return _outputVec;
	}

	//converts infix expression into postfix
	static StrVector infixToPostfix(const String& _infix) {

		StrVector _infixVec = _breakExpr(_infix);
		if (_infixVec.empty()) {
			Print::error("empty expression has been passed, since breakExpr returned empty!");
			Global::errorFlag = true;
			/*std::cout << "empty expression has been passed, since breakExpr returned empty!" << std::endl; */
		}
		if (!ExpressionValidator::_isValidExpression(_infix)) {
			/*std::cout << "error" << std::endl;*/
			Print::error("Expression is not valid!");
			Global::errorFlag = true;
			return {/*empty vector*/ };
		}

		std::stack<String> _s;
		StrVector _postfix;

		/*std::cout << "input: |" << _infix << "|\n";*/

		for (int __i = 0; __i < _infixVec.size(); __i++) {
			const String& _token = _infixVec[__i];
			//std::cout << "token:|" << _token << "|\n";

			if (isOperator(_token)) {
				//std::cout << "isOper\n";
				while (!_s.empty() && precedence(_token) <= precedence(_s.top())) {
					_postfix.push_back(_s.top());
					_s.pop();
				}
				_s.push(_token);
			}
			else if (_token == "(") {
				//std::cout << "pushed (\n";
				_s.push(_token);
			}
			else if (_token == ")") {
				while (!_s.empty() && _s.top() != "(") {
					_postfix.push_back(_s.top());
					//std::cout << "postfix became :|" << _postfix << "|\n";
					_s.pop();
				}
				if (!_s.empty() && _s.top() != "(") {
					/*std::cout << "Invalid Expression" << std::endl;*/
					Print::error("Invalid Expression!");
					Global::errorFlag = true;
					return {/*empty vector*/ }; // Mismatched parentheses
				}
				else {
					if (!_s.empty()) {
						//std::cout << "removed: " << _s.top() << "\n";
						_s.pop();
					}
					//else { std::cout << "stk empty\n"; }
				}
			}
			else {
				_postfix.push_back(_token);
				//std::cout << "postfix became2:|" << _postfix << "\n";
			}
		}

		while (!_s.empty()) {
			_postfix.push_back(_s.top());
			_s.pop();
		}
		//std::cout << std::endl;
		return _postfix;
	}

	/*verifies expression against expression rules!*/
	static bool _verifyExpr(const StrVector& _expr) {
		if (_expr.empty()) { return false; }

		std::vector<Expr> _expVec;
		for (const String& _e : _expr) {
			if (isOperator(_e)) {
				_expVec.push_back(Expr::OPERATOR);
			}
			else if (_e == "(") { _expVec.push_back(Expr::OPEN_PAREN); }
			else if (_e == ")") { _expVec.push_back(Expr::CLOSE_PAREN); }
			else {
				_expVec.push_back(Expr::OPERAND);/*n for not-operator, yeah i am still not in the mood to use the enum!*/
			}
		}

		Expr __curr, __nxt;
		for (size_t i = 0; i < _expVec.size() - 1; i++)/*this loops until 2nd last element*/
		{
			__curr = _expVec[i]; __nxt = _expVec[i + 1];
			if (__curr == Expr::OPERATOR) {/*operator*/
				if (__nxt == Expr::OPERATOR) {/*consecutive operators!*/
					//std::cout << "two consecutive operators in expression" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
				else if (__nxt == Expr::CLOSE_PAREN) {/*who uses ')' after an operator? not humans at least!*/
					//std::cout << ") is not allowed after an operator" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
			}
			else if (__curr == Expr::OPERAND) {/*operand*/
				if (__nxt == Expr::OPERAND) {/*consecutive operands!*/
					//std::cout << "two consecutive operands in expression" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
				else if (__nxt == Expr::OPEN_PAREN) {/* "operand (" is not allowed*/
					//std::cout << "( after an operand is not allowed in expression" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
			}
			else if (__curr == Expr::OPEN_PAREN) {
				if (__nxt == Expr::OPERATOR) {/*"( operator" is also not allowed*/
					//std::cout << "operator after ( is not allowed in expression" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
				else if (__nxt == Expr::CLOSE_PAREN) {/*"( )" is also not allowed, except functions*/
					//std::cout << "empty '(' and ')' are not allowed except functions" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
			}
			else if (__curr == Expr::CLOSE_PAREN) {
				if (__nxt == Expr::OPERAND) {/* ") operand" is meaningless!*/
					//std::cout << "\") operator\" is not allowed" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
				else if (__nxt == Expr::OPEN_PAREN) {/*")(" is not allowed!*/
					//std::cout << "\") & (\" is not allowed!" << std::endl;
					Global::errorFlag = true;
					Print::error("invalid expression!");
					return false;
				}
			}
		}
		return true;
	}

};

#endif // !EXPRESSION_H
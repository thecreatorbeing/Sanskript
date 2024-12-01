#ifndef PARSEUTIL_H
#define PARSEUTIL_H

#include"Built_In.h"
#include"ConsolePrintUtil.h"
#include "Expression.h"
//#include"Keywords.h"
//#include"Global.h"
//#include "SymbolTable.h"

const String MODULE_NAME = "modulename";
const String SHORT_NAME = "shortName";
	  
const String IDENTIFIERS = "identifiers";
const String DATATYPE = "datatype";
	  
const String VAR_IDENTIFIER = "varidentifier";

/*RHS_EXPR can also be used for bare function call */
const String RHS_EXPR = "rhsExpr";
//const String LHS_VALUE = "lhsvalue";/*VAL_IDENTIFIER works same!*/
	  
const String LH_OPER = "lhop";
const String RH_OPER = "rhop";
const String OPER = "oper";
const String EXP_TYPE = "exptype";
	  
const String RETURN_VALUE = "returnvalue";
	  
const String CLASS_NAME = "classname";
const String CLASS_PARENT = "classparent";
const String CLASS_ATTR = "classAttr";
const String CLASS_FUN = "classfun";
const String CLASS_OPER = "classoper";
const String CLASS_BEH = "classbeh";
	  
const String FUN_IDENTIFIER = "funidentifier";
const String FUN_PARAM_IDEN = "funParamIden";
const String FUN_PARAM_DTYPE = "funParamDType";
const String FUN_RETURN_TYPE = "returnType";
const String FUN_ACCESS_MODIFIER = "accessmodifier";
const String FUN_STATIC_FUN = "staticfun";
const String FUN_BODY = "funbody";
const String FUN_BEWARE_ERR = "bewareerr";
const String FUN_CLASS_NAME = "funclassname";
	  
const String BEH_IDENTIFIER = "behidentifier";
const String BEH_TARGET = "behtarget";
const String BEH_BODY = "behbody";
	  
const String OP_IDENTIFIER = "opidentifier";
const String OP_LHS_TYPE = "oplhstype";
const String OP_RHS_TYPE = "oprhstype";
const String OP_BODY = "opbody";
	  
const String IF_CONDITION = "ifcondition";
const String ELSE_IF_CONDITION = "ifelsecondition";
//const String IF_BODIES = "ifbodies"; /*not required since if body will 
									//be other statement/block types*/
	  
const String SW_CASES = "swcases";
const String SW_HAS_DEFAULT = "swhasdefault";
const String SW_BODIES = "swbodies";
	  
const String LOOP_CONDN = "loopcondn";
//const String LOOP_BODY = "loopbody";/*not required*/
	  
const String REV_LOOP_CONDN = "revloopcondn";
//const String REV_LOOP_BODY = "revloopbody";/*not required*/
	  
//const String TRY_BODY = "trybody";/*has nothing except "try"*/
const String CATCH_BLK = "catchblock";
//const String CATCH_BODY = "catchbody";/*not required*/
const String RAISE_ERR_STMT = "raiseerrstmt";
	  
const String STR_VALUE = "strvalue";
	  
const String CMT_VALUE = "cmtvalue";

const String CPP_BLK_IDEN = "cppBlkIden";
const String CPP_BLK_BODY = "cppBlkBody";

class Parse {
private:
	/*check if classname is valid ($ not allowed)*/
	static bool isValidClassName(const String& _clsName) {

		//Print::warning(identifier);
		// Check if the String is empty
		if (_clsName.empty()) {
			return false;
		}

		// The first character must be a letter or underscore
		if (!isalpha(_clsName[0]) && _clsName[0] != '_') {
			return false;
		}

		// Loop through remaining characters and check if they are alphanumeric or underscore
		for (size_t i = 1; i < _clsName.size(); ++i) {
			if (!isalnum(_clsName[i]) && _clsName[i] != '_') {
				return false;
			}
		}

		// Identifier cannot be a keyword except primitive types
		if(isKwrd(_clsName) && (_clsName != "int" && _clsName != "float" && _clsName != "char" && _clsName != "bool" && _clsName != "string" /*&& k != "ptr"*/)) {/*i decided that pointer support will be added later!*/
			//Print::warning("|" + _clsName + "| : |" + k + "|");
			/*TODO: remove warning and add error when Classes will be supported!*/
			return false;
		}

		/*otherwise, the class is valid!*/
		return true;
	}

	/*checks for valid identifier (start with '_' or alphabet, then may contain number but '$' is not allowed)*/
	static bool isValidNoDollarIden(const std::string& _iden) {
		// Check if the string is empty
		if (_iden.empty()) {
			return false;
		}

		// The first character must be a letter or underscore
		if (!isalpha(_iden[0]) && _iden[0] != '_') {
			return false;
		}

		// Loop through remaining characters and check if they are alphanumeric or underscore
		for (size_t i = 1; i < _iden.size(); ++i) {
			if (!isalnum(_iden[i]) && _iden[i] != '_') {
				return false;
			}
		}

		// Identifier cannot be a keyword
		for (std::string k : keywords) {
			if (k == _iden) {
				return false;
			}
		}

		return true;
	}

public:
	/*pending*/
	static CodeMap invite(StrVector _words) {
		CodeMap _map;

		if (_words.size() == 1) {/*i.e. invite; (nothing invited)*/
			String _stmt = "";
			for (const String& _str : _words) {
				/*i know the statement consists only 'invite;' and nothing else
				but still i want my program to actually print what's inside of statement
				to get realtime code related error and i think this is a good practice */
				_stmt += _str;
			}
			Print::warning("statement: '" + _stmt + "' is an empty invite statement (redundant)");
		}
		//else if (_words[1] == ) {

		//}


		return _map;
	}

	/*variables have python style access modifiers, map consists of 'datatype' and 'identifiers',
	the variables gets added into symbol table*/
	static CodeMap varDeclaration(StrVector _words) {
		CodeMap _map;

		String _dType = _words[_words.size() - 1];
		//Print::warning("datatype is: '" + _dataType+"'");

		int _clsIndx = Global::findInBlockStack("class");
		String _currentClass = (_clsIndx != -1) ? Global::blockStack[_clsIndx][1] : "";
		/*if current var is inside class then classname else no name*/

		String _varstr = "";
		StrVector _variables;
		int _isConstant = false;

		int _varstart = 1, _varEnd = 0;
		if (_words[1] == "const") { _varstart = 2; _isConstant = true; }

		for (const String& _str : _words) {
			if (_str == "as") { break; }
			_varEnd++;
		}

		for (int i = _varstart/*after 'define' (optionally 'constant')*/; i < _varEnd/*before 'as'*/; i++) {
			_varstr += _words[i];
		}
		//Print::warning("_varstr is: " + _varstr);

		String _tempIdentifier = "";
		String _tempAccessMod = "";
		bool _isStaticFlag = false;
		String __newTempIden = "";
		for (int i = 0; i < _varstr.size(); i++) {
			char c = _varstr[i];

			if (c == ',' || i == _varstr.size() - 1) {
				if (i == _varstr.size() - 1) { _tempIdentifier += c; }

				if (_tempIdentifier.ends_with("]")) { Print::error("arrays are not supported yet!"); Global::errorFlag = true; }

				if (_tempIdentifier.starts_with("$")) {
					/*if variable starts with $ then it is a static variable */
					_isStaticFlag = true;
				}

				if (isValidIdentifier(_tempIdentifier)) {
					for (char _c : _tempIdentifier) { if (_c != '$') { __newTempIden += _c; } }
					_tempIdentifier = __newTempIden;
					__newTempIden.clear();
					_variables.push_back(_tempIdentifier);
					//Print::warning("added: \"" + _tempIdentifier +"\"");
				}
				else {
					Print::error("identifier \"" + _tempIdentifier + "\" is not valid");
					Global::errorFlag = true;
				}
				/*python style access modifiers*/
				if (_tempIdentifier.starts_with("__")) { _tempAccessMod = "private"; }
				else if (_tempIdentifier.starts_with("_")) { _tempAccessMod = "protected"; }
				else { _tempAccessMod = "public"; }

				for (const Variable& _entry : Variable::symbolTable) {
					if (_tempIdentifier == _entry.identifier && _currentClass == _entry.className) {
						Print::error("identifier \"" + _tempIdentifier + "\" is already defined" + ((_currentClass != "") ? (" in \"" + _currentClass + "\"") : ""));
						/*last line of above code only checks whether class name exists and print its name*/
						Global::errorFlag = true;
						break;
					}
				}
				if (!Global::errorFlag) {
					//_tempIdentifier = (_tempIdentifier.starts_with("$") ? _tempIdentifier.substr(1))
					Variable::insertVariable(_tempIdentifier, _dType, _currentClass, _tempAccessMod, _isStaticFlag, _isConstant, 0/*arrays are not supported for now!*/);
				}
				//Print::varSymbolTable();

				/*resetting values*/
				_tempIdentifier.clear();
				_tempAccessMod.clear();
				_isStaticFlag = false;
			}
			else if (c == ' ') { continue;/*ignore spaces in variable names*/ }
			else {
				_tempIdentifier += c;
			}
		}/*for loop ends!*/

		_map[IDENTIFIERS] = _variables;
		_map[DATATYPE] = { _dType };

		return _map;
	}

	/*(Updatable!)*/
	static CodeMap assignment(StrVector _words) {
		CodeMap _map;

		/*Below section of this function needs to be updated!*/
		String _lhs = _words[0];
		String _rhs = "";

		size_t eqlIndx = 1;/*equal sign index in _words vector*/
		for (size_t i = eqlIndx+1 ; i < _words.size() ; i++) {
			_rhs += " " + _words[i];
		}

		//Print::green("|"+_rhs+"|");

		_map[VAR_IDENTIFIER] = { _lhs };/*read this twice!*/
		_map[RHS_EXPR] = { _rhs };/*this is an array/vector!*/

		return _map;
	}

	/*updatable!*/
	static CodeMap returnStmt(StrVector _words) {
		CodeMap _map;

		String _returnValue = "";
		for (size_t i = 1; i < _words.size(); i++) {
			_returnValue += ' ' + _words[i];
		}
		//Print::green("|"+_returnValue+"|");

		_map[RETURN_VALUE] = { _returnValue };

		return _map;
	}

	/*deprecated: since, function call is also an expression*/
	/*static CodeMap funCall(StrVector _words) {
		CodeMap _map;


		return _map;
	}*/

	/*implemented in next version*/
	static CodeMap raiseErrStmt(StrVector _words) {
		CodeMap _map;


		return _map;
	}

	static CodeMap classDeclaration(StrVector _words) {
		CodeMap _map;

		Print::error("Classes are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	static CodeMap funDeclaration(StrVector _words) {
		CodeMap _map;

		//ex, declare function div(a as int, b as int) returns int beware ZeroDivError

		String _funName = _words[2], _returnType = "void";/*default return type*/

		int _parenStart = _funName.find("("), _parenEnd = -1;
		if (_parenStart != -1) {
			_funName = _funName.substr(0, _parenStart);
		}

		for (int i = 0; i < _words.size(); i++) {
			if (_words[i] == "returns") {
				_parenEnd = i - 1;
				_returnType = _words[i + 1];
				break;
			}
		}

		String _paramString = "";
		for (int i = 2; i < _parenEnd + 1; i++) { _paramString += _words[i] + ((i == _parenEnd) ? "" : " "); }

		_paramString = _paramString.substr(_parenStart);

		StrVector _ParamsStrVector;
		String _currentStr;
		for (int i = 1; i < _paramString.size() - 1; i++) {
			char __c = _paramString[i];

			if (__c == ',') {
				_ParamsStrVector.push_back(_currentStr);
				_currentStr.clear();
			}
			else if (i == _paramString.size() - 2) {
				_currentStr += __c;
				_ParamsStrVector.push_back(_currentStr);
				_currentStr.clear();
			}
			else {
				//if ( __c == ' ' && _paramString[i - 1] != ',') {
				_currentStr += __c;
				//}
			}
		}

		std::vector<std::vector<String>> _params;
		StrVector __tempVec;
		String __temp;
		char _chr;
		for (const String& _s : _ParamsStrVector) {
			//Print::warning("'"+_s+"'");
			for (int __i = 0; __i < _s.size(); __i++) {
				_chr = _s[__i];

				if (_chr == ' ') {
					if (!__temp.empty()) {
						//Print::warning("added: '" + __temp +"'");
						__tempVec.push_back(__temp);
						__temp.clear();
					}
				}
				else if (_chr == ',' || __i == _s.size() - 1) {
					if (!__tempVec.empty()) {
						__temp += _chr;
						//Print::warning("added: '" + __temp + "'");
						__tempVec.push_back(__temp);
						__temp.clear();
						_params.push_back(__tempVec);
						__tempVec.clear();
					}
				}
				else {
					__temp += _chr;
				}
			}
		}
		String _tmpVarDef, ___, _funString = "", _accessMod, _varIden, _varDType;
		bool _isConst = false;
		for (int _i = 0; _i < _words.size(); _i++) { _funString += _words[_i] + ((_i != _words.size() - 1) ? " " : ""); }

		if ((!Parse::isValidClassName(_returnType)) && (_returnType != "void")) {/*because, void is valid return type but not a class which can create objects*/
			Print::error("return type: '" + _returnType + "' is not valid, in line: '" + _funString + "'");
			Global::errorFlag = true;
		}
		if (!Class::symbolTableContains(_returnType)) {
			Print::error("definition for return type class named '" + _returnType + "' is not found, in line: '" + _funString + "'");
			Global::errorFlag = true;
		}

		_map[FUN_CLASS_NAME].push_back("");
		if (!Global::blockStack.empty()) {
			int __indx = Global::findInBlockStack("class");
			if (__indx != -1) {
				_map[FUN_CLASS_NAME] = { Global::blockStack[__indx][1] };
			}
		}

		StrVector _paramDTypes, _paramIden;
		for (const StrVector& _sVec : _params) {
			for (int __i = 0; __i < _sVec.size(); __i++) { ___ = _sVec[__i]; _tmpVarDef += ___ + ((__i != _sVec.size() - 1) ? " " : ""); }

			_varDType = _sVec[_sVec.size() - 1];
			if (!Parse::isValidClassName(_varDType)) {
				Print::error("class name: '" + _varDType + "' is not valid, in line: '" + _funString + "'");
				Global::errorFlag = true;
			}
			if (!Class::symbolTableContains(_varDType)) {
				Print::error("definition for class named '" + _varDType + " is not found, in line: '" + _funString + "'");
				Global::errorFlag = true;
			}

			if (_sVec[0] == "const") {
				_isConst = true;
				_varIden = _sVec[1];
			}
			else {
				_varIden = _sVec[0];
			}

			if (_sVec[_sVec.size() - 2] != "as") {
				Print::error("syntax error: '" + _tmpVarDef + "' in line '" + _funString + "'");
				Global::errorFlag = true;
			}
			if (_varIden.starts_with('$')) {
				Print::error("syntax error: static variable '" + _tmpVarDef + "' is not allowed inside function definition '" + _funString + "' ");
				Global::errorFlag = true;
			}
			if (!isValidIdentifier(_varIden)) {
				Print::error("invalid identifier: '" + _tmpVarDef + "' is not a valid variable identifier in line: '" + _funString + "' ");
				Global::errorFlag = true;
			}
			else {/*add them into variable's symbol table*/
				/*python style access modifiers*/
				if (_varIden.starts_with("__")) { _accessMod = "private"; }
				else if (_varIden.starts_with("_")) { _accessMod = "protected"; }
				else { _accessMod = "public"; }

				_paramDTypes.push_back(_varDType);
				_paramIden.push_back(_varIden);

				Variable::insertVariable(_varIden, _varDType, _map[FUN_CLASS_NAME][0], _accessMod, false, _isConst, 0);/*currently arrays are not supported!*/
			}

			_accessMod.clear();
			_tmpVarDef.clear();
			_varIden.clear();
			_varDType.clear();
			_isConst = false;
		}

		/*Print::warning("------------------------------------------------------------------------");
		for (const StrVector& __ : _params) {
			for (const String& _ : __) {
				std::cout << "|" << _ << "|";
			}
			std::cout << std::endl;
		}
		Print::warning("------------------------------------------------------------------------");*/
		//Print::warning("fun: '"+_funName + "' params: '"+ _paramString + "'");
		String __funAccMod = "";
		if (_funName.starts_with("__")) { __funAccMod = "private"; }
		else if (_funName.starts_with('_')) { __funAccMod = "protected"; }
		else { __funAccMod = "public"; }

		_map[FUN_IDENTIFIER].push_back(_funName);
		_map[FUN_PARAM_DTYPE] = _paramDTypes;
		_map[FUN_PARAM_IDEN] = _paramIden;
		_map[FUN_RETURN_TYPE].push_back(_returnType);
		_map[FUN_ACCESS_MODIFIER].push_back(__funAccMod);
		_map[FUN_BEWARE_ERR].push_back("");
		_map[FUN_STATIC_FUN].push_back((_funName.starts_with('$')) ? "true" : "false");

		return _map;
	}

	static CodeMap behDeclaration(StrVector _words) {
		CodeMap _map;

		Print::error("behaviors are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	static CodeMap operDeclaration(StrVector _words) {
		CodeMap _map;

		Print::error("Custom Operators are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*implemented in next version*/
	static CodeMap classDef(StrVector _words) {
		CodeMap _map;

		Print::error("Classes are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*(ready to use): parse the function declaration.
	For ex, define function div(a as int, b as int) returns int beware ZeroDivError*/
	static CodeMap funDef(StrVector _words) {
		CodeMap _map;

		//ex, define function div(a as int, b as int) returns int beware ZeroDivError

		String _funName = _words[2], _returnType = "void";/*default return type*/

		int _parenStart = _funName.find("("), _parenEnd = -1;
		if (_parenStart != -1) {
			_funName = _funName.substr(0, _parenStart);
		}

		for (int i = 0; i < _words.size(); i++) {
			if (_words[i] == "returns") {
				_parenEnd = i - 1;
				_returnType = _words[i + 1];
				break;
			}
		}

		String _paramString = "";
		for (int i = 2; i < _parenEnd + 1; i++) { _paramString += _words[i] + ((i == _parenEnd) ? "" : " ");}

		_paramString = _paramString.substr(_parenStart);

		StrVector _ParamsStrVector;
		String _currentStr;
		for (int i = 1; i < _paramString.size() - 1; i++) {
			char __c = _paramString[i];

			if (__c == ',') {
				_ParamsStrVector.push_back(_currentStr);
				_currentStr.clear();
			}
			else if (i == _paramString.size() - 2) {
				_currentStr += __c;
				_ParamsStrVector.push_back(_currentStr);
				_currentStr.clear();
			}
			else {
				//if ( __c == ' ' && _paramString[i - 1] != ',') {
				_currentStr += __c;
				//}
			}
		}

		std::vector<std::vector<String>> _params;
		StrVector __tempVec;
		String __temp;
		char _chr;
		for (const String& _s : _ParamsStrVector) {
			//Print::warning("'"+_s+"'");
			for (int __i = 0; __i < _s.size(); __i++) {
				_chr = _s[__i];

				if (_chr == ' ') {
					if (!__temp.empty()) {
						//Print::warning("added: '" + __temp +"'");
						__tempVec.push_back(__temp);
						__temp.clear();
					}
				}
				else if (_chr == ',' || __i == _s.size() - 1) {
					if (!__tempVec.empty()) {
						__temp += _chr;
						//Print::warning("added: '" + __temp + "'");
						__tempVec.push_back(__temp);
						__temp.clear();
						_params.push_back(__tempVec);
						__tempVec.clear();
					}
				}
				else {
					__temp += _chr;
				}
			}
		}
		String _tmpVarDef, ___, _funString = "", _accessMod, _varIden, _varDType;
		bool _isConst = false;
		for (int _i = 0; _i < _words.size(); _i++) { _funString += _words[_i] + ((_i != _words.size() - 1) ? " " : ""); }

		if ( (!Parse::isValidClassName(_returnType)) && (_returnType!="void") ){/*because, void is valid return type but not a class which can create objects*/
			Print::error("return type: '" + _returnType + "' is not valid, in line: '" + _funString + "'");
			Global::errorFlag = true;
		}
		if (!Class::symbolTableContains(_returnType)) {
			Print::error("definition for return type class named '" + _returnType + "' is not found, in line: '" + _funString + "'");
			Global::errorFlag = true;
		}

		_map[FUN_CLASS_NAME].push_back("");
		if (!Global::blockStack.empty()) {
			int __indx = Global::findInBlockStack("class");
			if (__indx != -1) {
				_map[FUN_CLASS_NAME] = { Global::blockStack[__indx][1] };
			}
		}

		StrVector _paramDTypes, _paramIden;
		for (const StrVector& _sVec : _params) {
			for (int __i = 0; __i < _sVec.size(); __i++) { ___ = _sVec[__i]; _tmpVarDef += ___ + ((__i != _sVec.size() - 1) ? " " : ""); }

			_varDType = _sVec[_sVec.size() - 1];
			if (!Parse::isValidClassName(_varDType)) {
				Print::error("class name: '" + _varDType + "' is not valid, in line: '" + _funString + "'");
				Global::errorFlag = true;
			}
			if (!Class::symbolTableContains(_varDType)) {
				Print::error("definition for class named '" + _varDType + " is not found, in line: '" + _funString + "'");
				Global::errorFlag = true;
			}

			if (_sVec[0] == "const") {
				_isConst = true;
				_varIden = _sVec[1];
			}
			else {
				_varIden = _sVec[0];
			}

			if (_sVec[_sVec.size() - 2] != "as") {
				Print::error("syntax error: '" + _tmpVarDef + "' in line '" + _funString + "'");
				Global::errorFlag = true;
				return {/*empty CodeMap*/ };
			}
			if (_varIden.starts_with('$')) {
				Print::error("syntax error: static variable '" + _tmpVarDef + "' is not allowed inside function definition '" + _funString + "' ");
				Global::errorFlag = true;
				return {/*empty CodeMap*/ };
			}
			if (!isValidIdentifier(_varIden)) {
				Print::error("invalid identifier: '" + _tmpVarDef + "' is not a valid variable identifier in line: '" + _funString + "' ");
				Global::errorFlag = true;
				return {/*empty CodeMap*/};
			}
			else {/*add them into variable's symbol table*/
				/*python style access modifiers*/
				if (_varIden.starts_with("__")) { _accessMod = "private"; }
				else if (_varIden.starts_with("_")) { _accessMod = "protected"; }
				else { _accessMod = "public"; }

				_paramDTypes.push_back(_varDType);
				_paramIden.push_back(_varIden);

				Variable::insertVariable(_varIden, _varDType, _map[FUN_CLASS_NAME][0], _accessMod, false, _isConst, 0);/*currently arrays are not supported!*/
			}

			_accessMod.clear();
			_tmpVarDef.clear();
			_varIden.clear();
			_varDType.clear();
			_isConst = false;
		}

		/*Print::warning("------------------------------------------------------------------------");
		for (const StrVector& __ : _params) {
			for (const String& _ : __) {
				std::cout << "|" << _ << "|";
			}
			std::cout << std::endl;
		}
		Print::warning("------------------------------------------------------------------------");*/
		//Print::warning("fun: '"+_funName + "' params: '"+ _paramString + "'");
		String __funAccMod = "";
		if (_funName.starts_with("__")) { __funAccMod = "private"; }
		else if (_funName.starts_with('_')) { __funAccMod = "protected"; }
		else { __funAccMod = "public"; }

		_map[FUN_IDENTIFIER].push_back(_funName);
		_map[FUN_PARAM_DTYPE] = _paramDTypes;
		_map[FUN_PARAM_IDEN] = _paramIden;
		_map[FUN_RETURN_TYPE].push_back(_returnType);
		_map[FUN_ACCESS_MODIFIER].push_back(__funAccMod);
		_map[FUN_BEWARE_ERR].push_back("");
		_map[FUN_STATIC_FUN].push_back((_funName.starts_with('$')) ? "true" : "false");

		return _map;
	}

	/*implemented when classes will be supported!*/
	static CodeMap behDef(StrVector _words) {
		CodeMap _map;

		Print::error("behaviors are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	static CodeMap operDef(StrVector _words) {
		CodeMap _map;

		Print::error("Custom Operators are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*(Updatable!)*/
	static CodeMap ifBlk(StrVector _words) {
		CodeMap _map;

		String _condition = "";

		/*Below section of this function needs to be updated!*/
		for (size_t i = 1; i < _words.size(); i++) {
			_condition += _words[i] + ' ';
		}
		//Print::green("|" + _condition + "|");

		_map[IF_CONDITION] = { _condition };

		return _map;
	}

	/*implemented when expression will be supported*/
	static CodeMap elseIfBlk(StrVector _words) {
		CodeMap _map;

		String _elseIfCondition = "";

		for (size_t i = 2; i < _words.size(); i++) {
			_elseIfCondition += _words[i] + ' ';
		}
		//Print::green("|"+_elseIfCondition+"|");

		_map[ELSE_IF_CONDITION] = { _elseIfCondition };

		return _map;
	}

	/*Why the hell I am parsing else block, there's nothing in _words except "else"*/
	static CodeMap elseBlk(StrVector _words) {/*maybe required in future!*/
		CodeMap _map;

		/*for (const String& _ : _words) {
			Print::green("|" + _ + "|");
		}*/

		return _map;
	}

	/*implemented in next version*/
	static CodeMap switchBlk(StrVector _words) {/*switch( int|char|bool )*/
		CodeMap _map;

		Print::error("switch blocks are not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*implemented when expressions will be supported*/
	static CodeMap loop(StrVector _words) {
		CodeMap _map;

		String _loopCondition = "";
		for (size_t i = 1; i < _words.size()-1; i++) {
			_loopCondition += _words[i] + ' ';
		}
		//Print::green(_loopCondition);

		_map[LOOP_CONDN] = { _loopCondition };

		return _map;
	}

	/*implemented when expressions will be supported but in next version*/
	static CodeMap revLoop(StrVector _words) {
		CodeMap _map;

		Print::error("Reverse-loop is not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*implemented when classes will be supported*/
	static CodeMap tryBlk(StrVector _words) {
		CodeMap _map;

		Print::error("try-catch is not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*works with try block*/
	static CodeMap catchBlk(StrVector _words) {
		CodeMap _map;

		Print::error("try-catch is not supported currently!");
		Global::errorFlag = true;

		return _map;
	}

	/*this function is not needed, maybe future*/
	static CodeMap strLiteral(StrVector _words) {
		CodeMap _map;

		String _string = "";
		for (const String _ : _words) {
			_string += _ + ' ';
		}
		//Print::yellow(_string);

		Print::error("string: "+_string+"is redundant and needs to be removed, since it is not getting stored in any variable");
		Global::errorFlag = true;

		return _map;
	}

	static CodeMap comment(StrVector _words) {
		CodeMap _map;

		String _cmt = "";
		for (size_t i = 0; i < _words.size(); i++) {
			_cmt += _words[i] + ' ';
		}
		//Print::yellow(_cmt);

		_map[CMT_VALUE] = { _cmt };

		return _map;
	}

	static CodeMap cppBlk(String _cppBlkString, int _indentLevel) {
		CodeMap _map;

		/*index at which cpp block identifier's name ends*/
		size_t cppBlkIdenEndIndex = -1;
		cppBlkIdenEndIndex = _cppBlkString.find_first_of('{');

		if (cppBlkIdenEndIndex == -1 || !std::isspace(_cppBlkString[3]) ) {/*no  space between "cpp" keyword and block name*/
			Print::error("Invalid syntax \"" + _cppBlkString + "\"");
			Global::errorFlag = true;
			return{/*empty map*/ };
		}

		if (std::isspace(_cppBlkString[cppBlkIdenEndIndex - 1])) {/*whitespace between identifier and '{' */
			cppBlkIdenEndIndex -= 1;/*exclude the whitespace*/
		}

		String _cppBlkIden = _cppBlkString.substr(4, cppBlkIdenEndIndex-4);/*identifier starts after "cpp " hence at 4th index in String*/
		//Print::yellow("|"+_cppBlkIden+"|");

		if (_cppBlkIden.empty()) {
			Print::error("unnamed \"cpp\" block \""+ _cppBlkString +"\" is not allowed");
			Global::errorFlag = true;
			return {/*empty map*/};
		}		

		/*validate cppBlockIden against variable-like identifier rules, '$' not allowed*/
		if (  !Parse::isValidNoDollarIden(_cppBlkIden)  ) {
			Print::error("\"cpp\" block identifier \"" + _cppBlkIden + "\" is not valid");
			Global::errorFlag = true;

			return {/*empty CodeMap*/ };
		}
		else {

			String _cppCode = _cppBlkString.substr(cppBlkIdenEndIndex+1, _cppBlkString.find_last_of('}') - cppBlkIdenEndIndex - 1);
			//Print::yellow("|"+_cppCode+"|");

			_map[CPP_BLK_IDEN] = { _cppBlkIden };
			_map[CPP_BLK_BODY] = { _cppCode };

			return _map;
		}
	}

};

#endif // !PARSEUTIL_H
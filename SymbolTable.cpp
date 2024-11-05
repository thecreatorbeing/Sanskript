#include "SymbolTable.h"

Variable::Variable(const String& _identifier, const String& _dataType,
	const String& _className, const String& _accessModifier,
	bool _isStatic, bool _isCons, int _diamensions)
	:
	identifier(_identifier), dataType(_dataType), className(_className), accessModifier(_accessModifier), isStatic(_isStatic),
	isConstant(_isCons), isScopeEnded(false), scope(1), diamensions(_diamensions)
{/*constructor body*/
}

void Variable::enterScope() {
	for (Variable& _var : symbolTable) {

		if (_var.isScopeEnded) {/*ignore out-of-scope variables*/
			continue;
		}

		_var.scope++;
	}
	//std::cout << "\033[0;33mscope entered!\033[0m" << std::endl;
}

void Variable::exitScope() {
	for (Variable& _var : symbolTable) {

		_var.scope--;/*all variable with negative scope are out-of-scope*/

		if (_var.scope < 1 && !_var.isScopeEnded) {/*make variable out-of-scope*/
			_var.isScopeEnded = true;
		}
	}
	//std::cout << "\033[0;33mscope exited!\033[0m" << std::endl;
}

/*----------------------------------------------------------------------------------------------------*/

Function::Function(const String& iden, const StrVector& prmTypes, const String& retType,
	const String& bwrType, const String& accesMod, const String& clsName, bool _isStatic)
	: identifier(iden), paramTypes(prmTypes), returnType(retType), bewareType(bwrType), accessModifier(accesMod), className(clsName), isStatic(_isStatic)
{/*constructor body*/
}

String Function::getReturnType(const String& _fun, const StrVector& _params) {
	return "";
}
/*----------------------------------------------------------------------------------------------------*/

Operator::Operator(const String& _opName, const String& _retType, const String& _lhs, const String& _rhs, const String& _bwrType)
	: operatorName(_opName), returnType(_retType), lhsType(_lhs), rhsType(_rhs), bewareType(_bwrType)
{/*constructor body*/
}

void Operator::initializeBasicOperators() {
	Operator::insert("+", "int", "int", "int", "");
	Operator::insert("+", "float", "float", "float", "");
	Operator::insert("+", "float", "int", "float", "");
	Operator::insert("+", "float", "float", "int", "");

	Operator::insert("+", "string", "string", "int", "");
	Operator::insert("+", "string", "string", "float", "");

	Operator::insert("-", "int", "int", "int", "");
	Operator::insert("-", "float", "float", "float", "");
	Operator::insert("-", "float", "float", "int", "");
	Operator::insert("-", "float", "int", "float", "");

	Operator::insert("/", "int", "int", "int", "");/*bewareType should be ZeroDivErr*/
	Operator::insert("/", "float", "float", "int", "");
	Operator::insert("/", "float", "int", "float", "");
	Operator::insert("/", "float", "float", "float", "");

	Operator::insert("*", "int", "int", "int", "");
	Operator::insert("*", "float", "float", "float", "");
	Operator::insert("*", "float", "float", "int", "");
	Operator::insert("*", "float", "int", "float", "");

	Operator::insert("%", "int", "int", "int", "");

	/*TODO: i might require to add other operators like &&, || too!*/
}

/*returns true if passed parameter is an operator*/
bool Operator::contains(const String& _oper) {
	for (const Operator& _o : Operator::symbolTable) {
		if (_o.operatorName == _oper) {
			return true;
		}
	}
	return false;
}

/*----------------------------------------------------------------------------------------------------*/

bool Class::symbolTableContains(const String& _className) {
	if (_className == "int" || _className == "float" || _className == "char" || _className == "bool" || _className=="string" || _className == "void"/*|| _className == "ptr"*/) {/*i decided that pointer support will be added later!*/
		return true;
	}

	for (const Class _entry : Class::symbolTable) {
		if (_entry.className == _className) {
			return true; 
		}
	}
	return false;
}

std::vector<Class> Class::symbolTable = {};
void Class::initializePrimitiveDataTypes()
{
	Class::insertClass("int", "");
	Class::insertClass("float", "");
	Class::insertClass("bool", "");
	Class::insertClass("char", "");
	Class::insertClass("ptr", "");
}

/*----------------------------------------------------------------------------------------------------*/
std::vector<Variable> Variable::symbolTable = {};
std::vector<Function> Function::symbolTable = {};
std::vector<Operator> Operator::symbolTable = {};

/*----------------------------------------------------------------------------------------------------*/
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include"Built_In.h"
//#include <stack> /*i don't think i will require this since i am using int=0 as a scope*/

#include "ConsolePrintUtil.h"

class Variable {
public:
	static std::vector<Variable> symbolTable;

	String identifier,/*variable name*/
		dataType,
		className,/*if variable is member of any class*/
		accessModifier;/*public, protected or private*/
	bool isStatic, isConstant, isScopeEnded;
	int scope, /*starts from 1 then increases and decreases while entering and exiting scope resp... when become zero then variable become out of scope*/
		diamensions; /*if variable is an array then this stores it's diamension count*/

private:
	Variable(const String&, const String&, const String&, const String&, bool, bool, int);

public:
	/*inserts variable into the variable symbol table*/
	static void insertVariable(const String& _identifier, const String& _dataType, const String& _className,
		const String& _accessModifier, bool _isStatic, bool _isCons, int _diamensions) {

		Variable::symbolTable.push_back(Variable(_identifier, _dataType, _className, _accessModifier, _isStatic, _isCons, _diamensions));
	}

	/*static bool exists(const String& _identifier) {
		for()
	}*/

	static void enterScope();
	static void exitScope();
	//static String findDataType(const String& _var);

	static void printTable() {
		Print::green("-----------------------Function Symbol-Table-----------------------");
		for (const Variable& _v : Variable::symbolTable) {
			Print::yellow(_v.identifier);
		}
		Print::green("-------------------------------------------------------------------");
	}
};

class Function {

public:
	static std::vector<Function> symbolTable;

	String identifier;/*function name*/
	StrVector paramTypes;/*parameter types*/
	String returnType;/*return type*/
	String bewareType;/*error throw type*/
	String accessModifier;/*public, protected or private*/
	String className;
	bool isStatic;
	//int scope;

private:
	Function(const String&, const StrVector&, const String&, const String&, const String&, const String&, bool);

public:
	static void insertFun(const String& _identifier, const StrVector& _paramTypes, const String& _returnType,
		const String& _bewareType, const String& _accessModifier, const String& _className, bool _isStatic) {

		Function::symbolTable.push_back(Function(_identifier, _paramTypes, _returnType, _bewareType, _accessModifier, _className, _isStatic));
	}

	static void printTable() {

		Print::green("-----------------------Function Symbol-Table-----------------------");

		String _paramStr = "|";
		for (const Function _c : Function::symbolTable) {
			for (const String _p : _c.paramTypes) {
				_paramStr += _p + "|";
			}
			Print::yellow("iden: |" + _c.identifier + "| paramTypes: " + _paramStr + " retType: |" + _c.returnType + "| beware: |" + _c.bewareType + "| accessMod: |" + _c.accessModifier + "| className: |" + _c.className + "| isStatic: |" + std::to_string(_c.isStatic) + "|");
		}
		Print::green("-------------------------------------------------------------------");
	}

	static String getReturnType(const String& _fun, const StrVector& _params);

};

class Class {

	static std::vector<Class> symbolTable;

	String className;
	String parentClass;

private:
	Class(const String& _name, const String& _parent)
		: className(_name), parentClass(_parent)
	{/*constructor body*/
	}

public:
	static void insertClass(const String& _className, const String& _parentClass) {

		Class::symbolTable.push_back(Class(_className, _parentClass));
	}
	static bool symbolTableContains(const String& _className);
	static void initializePrimitiveDataTypes();
	static void printTable() {
		Print::green("Class Symbol-Table");
		for (const Class& _c : Class::symbolTable) {
			Print::yellow(_c.className);
		}
	}
};

class Behavior {

	static std::vector<Behavior> symbolTable;

	String behaviorName;
	String className;
	String targetVariable;
	String accessModifier;
	//int scope;

private:
	Behavior(const String& _behName, const String& _className, const String& _targetVar, const String& _accesMod)
		: behaviorName(_behName), className(_className), targetVariable(_targetVar), accessModifier(_accesMod)
	{/*constructor body*/
	}

public:
	void insertBehavior(const String& _behaviorName, const String& _className, const String& _targetVariable, const String& _accessModifier) {

		Behavior::symbolTable.push_back(Behavior(_behaviorName, _className, _targetVariable, _accessModifier));
	}

	static void printTable() {
		Print::green("Behavior Symbol-Table");
		for (const Behavior& _b : Behavior::symbolTable) {
			Print::yellow(_b.behaviorName);
		}
	}
};

class Operator {

public:
	static std::vector<Operator> symbolTable;

	String operatorName;
	String returnType;
	String lhsType;
	String rhsType;
	String bewareType;

private:
	Operator(const String&, const String&, const String&, const String&, const String&);

public:
	static void insert(const String& _opName, const String& _retType, const String& _lhs, const String& _rhs, const String& _bwrType) {

		Operator::symbolTable.push_back(Operator(_opName, _retType, _lhs, _rhs, _bwrType));
	}

	static void initializeBasicOperators();

	static bool contains(const String& _oper);

	/*returns the returnType if operator having name _oper exists in symbol-table and can process lhsType and rhsType*/
	static String matchOperator(const String& _oper, const String& _lhsType, const String& _rhsType) {

		for (const Operator& _o : Operator::symbolTable) {

			if (_o.operatorName == _oper && _o.lhsType == _lhsType && _o.rhsType == _rhsType) {
				return _o.returnType;
			}
		}
		return "";
	}

	static void PrintTable() {
		Print::green("Operator Symbol Table:");
		for(const Operator& _o : Operator::symbolTable)
		{
			Print::yellow(_o.operatorName);
		}
	}
};


//static int currentScope;



/*
class SymbolTable {//Deprecated
private:
	typedef struct _Symbol {
	String name;
	Symbol type;
	int scope;

	_Symbol(const String& _name, Symbol _type, int _scope):
		name(_name), type(_type), scope(_scope){}//constructor ends

} _Symbol;

	std::unordered_map<String, _Symbol> table;
	int currentScope;

public:
	SymbolTable(): currentScope(0){}

	void insert(const String& _name, Symbol _type) {
		if (find(_name)) {
			std::cerr << "Error: Symbol '" << _name << "' is already declared in this scope." << std::endl;
			return;
		}
		table[_name] = _Symbol(_name, _type, currentScope);
	}

	_Symbol* find(const String& _name) {

		auto it = table.find(_name);
		if (it != table.end()) {
			return &it->second;
		}
		return nullptr;
	}

	void enterScope() { this->currentScope++;}

	void exitScope() { this->currentScope--;}


};
*/


#endif // !SYMBOLTABLE_H
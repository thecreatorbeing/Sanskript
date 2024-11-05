#include "CodeGen.h"
#include "Global.h"
#include "ParseUtil.h"

String CodeGen::_starterCode = "/*This Code is generated using Sanskript Transpiler*/\n"
								"#include<string>\n"
								"using string = std::string;"
								"\n\n";


static int currentIndentLevel = 1, lastIndentLevel = 1;
static String _indentTabs = "";

void genCode(Node* _node, String& _genCodeString);

void CodeGen::generateCppCode(Node* _node, String& _genCodeString){
	for (Node* _child : _node->children) {
		if (Global::errorFlag) {/*playing safe!*/
			break;
		}
		genCode(_child, _genCodeString);
	}

	if (lastIndentLevel > 1) {
		//Print::red("CodeGen: indent decreased|pre:" + std::to_string(lastIndentLevel) + "|current:" + std::to_string(currentIndentLevel) + '|');

		size_t _loopCount = lastIndentLevel - 1;
		while (_loopCount != 0) { 
			
			_indentTabs.pop_back(); 

			_genCodeString += _indentTabs + "}\n";

			_loopCount--;
		}

	}
}

String _tempGenCode = "";
CodeMap _map;
void genCode(Node* _node, String& _genCodeString) {

	currentIndentLevel = _node->indentLevel;
	_tempGenCode = "";
	_map = _node->data;

	if (currentIndentLevel > lastIndentLevel) { _indentTabs.push_back('\t'); }
	else if (lastIndentLevel > currentIndentLevel) {
		//Print::red("indent decreased|pre:" + std::to_string(lastIndentLevel) + "|current:" + std::to_string(currentIndentLevel) + '|');
		
		size_t _loopCount = lastIndentLevel - currentIndentLevel;
		while (_loopCount != 0) {
		
			_indentTabs.pop_back();
			
			_genCodeString += _indentTabs + "}\n";

			_loopCount--;
		}
	}

	//Print::yellow("| pre:" + std::to_string(lastIndentLevel) + " | current : " + std::to_string(currentIndentLevel) + '|');


	if (_node->type == CodeType::VAR_DEC_STMT) {
		
		_tempGenCode += _map[DATATYPE][0];/*datatype of variable*/

		String _iden;
		for (size_t i = 0; i < _map[IDENTIFIERS].size(); i++) {/*variable identifiers*/
			_iden = _map[IDENTIFIERS][i];

			if (i < _map[IDENTIFIERS].size() - 1) {/*if item is not last in vector*/
				_tempGenCode += ' ' + _iden + ',';
			}
			else {
				_tempGenCode += ' ' + _iden + ';';/*if item is last in vector*/
			}
		}

		_genCodeString += _indentTabs + _tempGenCode + '\n';/*newline*/

		//Print::green("generated VAR_DEC: |" + _tempGenCode + "|" + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::ASSIGNMENT_STMT) {

		_tempGenCode = _map[VAR_IDENTIFIER][0] + " =" + _map[RHS_VALUE][0] + ';';

		_genCodeString += _indentTabs + _tempGenCode + '\n';/*newline*/

		//Print::green("generated ASSIGNMENT_STMT: |" + _tempGenCode + "|" + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::RETURN_STMT) {
		
		_tempGenCode = "return " + _map[RETURN_VALUE][0] + ';';

		_genCodeString += _indentTabs + _tempGenCode + '\n';/*newline*/

		//Print::green("generated RETURN_STMT: |" + _tempGenCode + "|" + std::to_string(currentIndentLevel));
	}
	
	/*NOT WORKING!*/
	else if (_node->type == CodeType::FUN_DEF_BLK) {

		if (!_map[FUN_CLASS_NAME][0].empty()) {/*if function belongs to class (can also be done using Symbol Table!)*/
			_tempGenCode = _map[FUN_ACCESS_MODIFIER][0] + ": "; /*added access modifier*/
		}
		
		if (_map[FUN_STATIC_FUN][0] == "true") { _tempGenCode += "static "; }/*either "true" or "false"*/

		_tempGenCode += _map[FUN_RETURN_TYPE][0] + ' '; /*added return type*/

		_tempGenCode += _map[FUN_IDENTIFIER][0] + "( "; /*added function identifier/name*/

		if (_map[FUN_PARAM_DTYPE].empty()) {
			_tempGenCode += ") {";
		}
		else {
			for (size_t param = 0; param < _map[FUN_PARAM_DTYPE].size(); param++) {

				if (param < _map[FUN_PARAM_DTYPE].size() - 1) {/*check whether current param item is not the last one*/
					_tempGenCode += _map[FUN_PARAM_DTYPE][param] + ' ' + _map[FUN_PARAM_IDEN][param] + ", ";
				}
				else {/*when the current param item is the last one*/
					_tempGenCode += _map[FUN_PARAM_DTYPE][param] + ' ' + _map[FUN_PARAM_IDEN][param] + ") {";
				}
			}
		}

		_genCodeString += _indentTabs + _tempGenCode + '\n';/*newline*/

		//Print::green("generated FUN_DEF: |" + _tempGenCode + "|" + std::to_string(currentIndentLevel));
		//Print::map(_map);
	}

	
	else if (_node->type == CodeType::IF_BLK) {
		
		_tempGenCode = "if" + _map[IF_CONDITION][0] + '{'; /*conditions are pre-wrapped within parentheses hence no parentheses given!*/

		_genCodeString += _indentTabs + _tempGenCode + '\n';

		//Print::green("generated IF_BLK: |" + _tempGenCode + '|' + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::ELSE_IF_BLK) {

		_tempGenCode = "else if" + _map[ELSE_IF_CONDITION][0] + '{';

		_genCodeString += _indentTabs + _tempGenCode + '\n';

		//Print::green("generated ELSE_IF_BLK: |" + _tempGenCode + '|' + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::ELSE_BLK) {
		
		_tempGenCode = "else{";

		_genCodeString += _indentTabs + _tempGenCode + '\n';

		//Print::green("generated ELSE_BLK: |" + _tempGenCode + '|' + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::LOOP_BLK) {

		_tempGenCode = "while" + _map[LOOP_CONDN][0] + '{';

		_genCodeString += _indentTabs + _tempGenCode + '\n';

		//Print::green("generated LOOP_BLK: |" + _tempGenCode + '|' + std::to_string(currentIndentLevel));
	}
	else if (_node->type == CodeType::COMMENT_BLK) {

		_tempGenCode = _map[CMT_VALUE][0];

		_genCodeString += _indentTabs + _tempGenCode + '\n';

		//Print::green("generated COMMENT: |" + _tempGenCode + '|' + std::to_string(currentIndentLevel));
	}



	lastIndentLevel = currentIndentLevel;

	for(Node* __child : _node->children){
		if (Global::errorFlag) {/*playing safe*/
			break;
		}
		
		genCode(__child, _genCodeString);
		
	}
}
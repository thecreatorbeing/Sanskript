#include"Parser.h"

#include"ParseUtil.h"
#include"Keywords.h"
#include"Global.h"
#include"SymbolTable.h"
#include"Expression.h"

/*checks for valid identifier*/
bool isVldIden(const String&);
//bool isKwrd(const String&);/*this one is moved to Keywords.h*/
bool isSep(char);/*this one is now being imported from Lexer.h (i radefined this function due to linker errors!)*/
void parseNode(StringNode*, Tree*);
StrVector breakCode(String);

Tree* Parser::parse(StringNode* _node) {
	Tree* tree = new Tree();

	for (StringNode* _c : _node->children) {
		if (Global::errorFlag) {
			//Print::error("Global Flag!");
			break;
		}
		parseNode(_c, tree);
	}
	return tree;
}

void parseNode(StringNode* _node, Tree* _root) {
    std::vector<std::string> _words = breakCode(_node->data);
    std::map<std::string, std::vector<std::string>> _mapData;

    if (_words[0] == "invite") {
        Print::error("invite statements are not supported yet!");
        Global::errorFlag = true;
        /*_mapData = Parse::invite(_words);
        _root->insert(_mapData, _root->current, Type::INVITE_STMT, _node->level);
        std::cout << _strNode->data << " is identified as InviteStmt\n";*/
        return;
    }
    else if (_words[0] == "define") { /*define block*/

        if (_words[1] == "class") {
            //Print::error("classes are not supported yet!");
            //Global::errorFlag = true;
            _mapData = Parse::classDef(_words);
            _root->insert(_mapData, _root->current, CodeType::CLASS_DEF_BLK, _node->level);
            //std::cout << _node->data << " is identified as ClassDefBlk\n";
            Global::addBlockToStack("class", _words[2]);
        }
        else if (_words[1] == "function") {
            //std::cout << _node->data << " is identified as FunDefBlk\n";
            _mapData = Parse::funDef(_words);
            _root->insert(_mapData, _root->current, CodeType::FUN_DEF_BLK, _node->level);

            std::string __funName = _mapData[FUN_IDENTIFIER][0];

            Global::addBlockToStack("function", __funName);
            //Print::warning("added fun:|" + __funName + "|");

            bool __isStatic = (_mapData[FUN_STATIC_FUN][0] == "true") ? true : false;
            Function::insertFun(__funName, _mapData[FUN_PARAM_DTYPE], _mapData[FUN_RETURN_TYPE][0], _mapData[FUN_BEWARE_ERR][0],
                _mapData[FUN_ACCESS_MODIFIER][0], _mapData[FUN_CLASS_NAME][0], __isStatic);
            //Print::map(_mapData);
        }
        else if (_words[1] == "behavior") {
            Print::error("behavior are not supported yet!");
            Global::errorFlag = true;
            /*_mapData = Parse::funDef(_words);
            _root->insert(_mapData, _root->current, Type::BEH_DEF_BLK, _node->level);
            std::cout << _strNode->data << " is identified as BehDefBlk\n";
            Global::addBlockToStack("behavior", _words[2]);*/
        }
        else if (_words[1] == "operator") {
            Print::error("operator are not supported yet!");
            Global::errorFlag = true;
            /*_mapData = Parse::funDef(_words);
            _root->insert(_mapData, _root->current, Type::OPR_DEF_BLK, _node->level);
            std::cout << _strNode->data << " is identified as OprDefBlk\n";
            Global::addBlockToStack("operator", _words[2]);*/
        }
        else if (_words[_words.size() - 2] == "as") {/*var declaration*/
            //std::cout << _node->data << " is identified as VarDecStmt\n";
            _mapData = Parse::varDeclaration(_words);
            _root->insert(_mapData, _root->current, CodeType::VAR_DEC_STMT, _node->level);
            //Print::map(_mapData);
        }
        else {
            Print::error("code snippet: '" + _node->data + "' is not identified!");
            Global::errorFlag = true;
        }
    }
    else if (_words[0].starts_with("/*")) {
        //std::cout << _node->data << " is identified as Comment\n";
        _mapData = Parse::comment(_words);
        _root->insert(_mapData, _root->current, CodeType::COMMENT_BLK, _node->level);
    }
    else if (_words[0].starts_with("\"")) { /*these type of strings are redundant,
                                        since not being stored in any variable*/
        //std::cout << _node->data << " is identified as StringLiteral\n";
        _mapData = Parse::strLiteral(_words);
        _root->insert(_mapData, _root->current, CodeType::STRING_LITERAL, _node->level);
    }
    else if (_words[0] == "return") {
        //std::cout << _node->data << " is identified as ReturnStmt\n";
        _mapData = Parse::returnStmt(_words);
        _root->insert(_mapData, _root->current, CodeType::RETURN_STMT, _node->level);
    }
    else if (_words[0] == "if" || _words[0].starts_with("if(")) {
        //std::cout << _node->data << " is identified as IfStmt\n";
        _mapData = Parse::ifBlk(_words);
        _root->insert(_mapData, _root->current, CodeType::IF_BLK, _node->level);
        Global::addBlockToStack("if", "");
    }
    else if (_words[0] == "else") {
        if (_words.size() == 1) {
            //std::cout << _node->data << " is identified as ElseBlk\n";
            _mapData = Parse::elseBlk(_words);/*this function returns empty CodeMap instance, 
                                                maybe useful in future*/
            _root->insert(_mapData, _root->current, CodeType::ELSE_BLK, _node->level);
            Global::addBlockToStack("else", "");
        }
        else if (_words[1].starts_with("if") || _words[1].starts_with("if(")) {
            //std::cout << _node->data << " is identified as ElseIfBlk\n";
            _mapData = Parse::elseIfBlk(_words);
            _root->insert(_mapData, _root->current, CodeType::ELSE_IF_BLK, _node->level);
            Global::addBlockToStack("else if", "");
        }
        else {
            std::cout << "\ncode snippet: '" << _node->data << "' is not identified!\n" << std::endl;
        }
    }
    else if (_words[0] == "switch" || _words[0].starts_with("switch(")) {
        Print::error("switches are not supported yet!");
        Global::errorFlag = true;
        /*_mapData = Parse::switchBlk(_words);
        _root->insert(_mapData, _root->current, Type::SWITCH_BLK, _node->level);
        std::cout << _strNode->data << " is identified as SwitchStmt\n";
        Global::addBlockToStack("switch", "");*/
    }
    else if (_words[0] == "until" || _words[0].starts_with("until(")) {/*TODO: currently! there is no mechanism here to identify whether iterate keyword has been used after until() or not, i should handle this into ParseUtil*/
        //std::cout << _node->data << " is identified as LoopBlk\n";
        _mapData = Parse::loop(_words);
        _root->insert(_mapData, _root->current, CodeType::LOOP_BLK, _node->level);
        Global::addBlockToStack("until", "");
    }
    else if (_words[0] == "try") {
        //std::cout << _node->data << " is identified as TryBlk\n";
        _mapData = Parse::tryBlk(_words);
        _root->insert(_mapData, _root->current, CodeType::TRY_BLK, _node->level);
        Global::addBlockToStack("try", "");
    }
    else if (_words[0] == "catch" || _words[0].starts_with("catch(")) {
        //std::cout << _node->data << " is identified as CatchBlk\n";
        _mapData = Parse::catchBlk(_words);
        _root->insert(_mapData, _root->current, CodeType::CATCH_BLK, _node->level);
        Global::addBlockToStack("catch", "");
    }
    else if (_words[0] == "raise") {
        //std::cout << _node->data << " is identified as RaiseErrStmt\n";
        _mapData = Parse::raiseErrStmt(_words);
        _root->insert(_mapData, _root->current, CodeType::RAISE_ERR_STMT, _node->level);
    }
    else if (_words.size() > 1 && isVariable(_words[0]) && _words[1] == "=") {
        //std::cout << _node->data << " is identified as AssignmentStmt\n";
        _mapData = Parse::assignment(_words);
        _root->insert(_mapData, _root->current, CodeType::ASSIGNMENT_STMT, _node->level);
    }
    else {
        std::cout << "\ncode snippet: '" << _node->data << "' is not identified!\n" << std::endl;
    }

    if (_node->children.size() > 0) { Variable::enterScope(); }
    for (StringNode* __child : _node->children) {
        if (Global::errorFlag) {
            break;
        }
        
        parseNode(__child, _root);
    
    }
    if (_node->children.size() > 0) { Variable::exitScope(); }


    //if (_node->children.size() > 0) {/*if current node has children (then it must be a block!)*/
    if (!_node->children.empty()) {/*if current node has children (then it must be a block!)*/
        //std::vector<std::string> _arr = Global::blockStack.back();/*read last element*/
        Global::blockStack.pop_back();/*remove last element*/
        //Print::log("popped: '" + _arr[0] + "' : '"+_arr[1] + "'");
    }
}

void Parser::printTree(Tree _tree) {
    _tree.preOrderTraversal(_tree.root);
}

/*don't get confused, analyze parseNode() & then parse() function as well!*/
StrVector breakCode(String _child) {
    std::vector<std::string > _retrnCode;
    std::string _word;
    bool isString = false, isComment = false;
    char lastChar = ' ', _char = ' ', nextChar;

    for (size_t i = 0; i < _child.size(); i++) {
        _char = _child[i];
        nextChar = ((i + 1) < _child.size()) ? _child[i + 1] : '\0';

        /*checks for comment*/
        if ((_char == '/' && nextChar == '*' && !isString)) {/*finding start of comment and it is not within string (double quotes)*/
            //log("1");
            isComment = true;
            _word += _char; /*adds '/'*/

            lastChar = _char;
            continue;
        }
        /*checks for string*/
        else if (_char == '"' && !isString && !isComment) {/*if " is found && not string and not comment*/
            //log("2");
            isString = true;
            _word += _char; /*adds '"' */

            lastChar = _char;
            continue;
        }


        /*checks for comment end*/
        if (lastChar == '*' && _char == '/' && isComment && !isString) {
            //log("3");
            _word += _char;/*adds '/' since c is '/' */
            isComment = false;/*since comment is ended!*/

            _retrnCode.push_back(_word);
            _word.clear();

            lastChar = _char;
            continue;
        }
        /*checks for string end*/
        else if (_char == '"' && isString && !isComment) {
            //log("4");
            _word += _char; /*adds '"' since c is '"' */
            isString = false;

            /*_retrnCode.push_back(_word);
            _word.clear();*/ /*works for comment but here it adds empty string as element in vector*/

            lastChar = _char;
            continue;
        }


        if (isSep(_char)) { /*break the code by ' ', '\t' & '\n' characters */
            _retrnCode.push_back(_word);
            _word.clear();
        }
        else {
            _word += _char;
        }


        lastChar = _char;
    }
    /*for (auto _ : _retrnCode) {
        Print::green(_);
    }*/ /*for debug/testing purpose!*/

    return _retrnCode;
}

/*different than ParseUtil's function*/
bool isVldIden(const std::string& _iden) {
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

/*checks if char is Separator {' ', '\t', '\n'}*/
bool isSep(char c) { return (c == ' ' || c == '\t' || c == '\n'); }
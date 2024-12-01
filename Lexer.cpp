#include"Lexer.h"

bool Lexer::isSep(char c) { return (c == ' ' || c == '\t' || c == '\n'); }

StrVector Lexer::lex(String _code) {
	/*vector of strings*/
	StrVector tokns;
	/*stores current word*/
	String wrd;

	/*isStr = isString | iscmt = isComment*/
	bool isStr = false, isCmt = false, isCppBlk = false;
	/*last one is for supporting C++ code in Sanskript by simply ignoring C++ code 
	and adding it in C++ generated code enclosed within try-catch block*/
	
	/*lst = last char | chr = current char | nxt = next char*/
	char lst='\0', chr='\0', nxt='\0';
	for (int i = 0; i < _code.size(); i++) {
		chr = _code[i];
		nxt = ((i + 1) < _code.size()) ? _code[i + 1] : '\0';

		/*this part added later to support C++ code within Sanskript code*/
		if (lst == 'c' && chr == 'p' && nxt == 'p' && !isStr && !isCmt ) {/*luck by chance 'cpp' keyword is 3 char long, 
											so i was able to adjust the logic like this*/

			isCppBlk = true; /*don't worry, we cannot nest two cpp blocks, 
						since whatever inner code of the outer block is ignored*/
			wrd += chr;
			lst = chr;
			continue;
		}
		else if (isCppBlk && lst=='e' && chr=='n' && nxt=='d' 
			&& _code[i+2]=='_' && _code[i+3] == 'c' && _code[i+4] == 'p'
			&& _code[i+5] == 'p') {/*'end_cpp' is a keyword*/
			/*while cpp block is active, strings and comments are not checked,
			so no need to use isStr and isCmt this time*/

			isCppBlk = false;
			i+=5;/*because, 'd_cpp' of 'end_cpp' keyword needs to be skipped*/

			wrd.pop_back(/*removed lst*/);/*remove 'e' of 'e-n-d' because 'end' is
			just for determining the end of cpp block, nothing else*/
			tokns.push_back(wrd);	wrd.clear();

			lst = chr;
			continue;
		}
		else if (isCppBlk) {/*wrd will store complete c++ code in it this time*/
			wrd += chr;
			lst = chr;
			continue;
		}



		/*--------------------------------------------------------*/
		/*ignores consecutive separators{' '|'\t'|'\n'}*/
		if (isSep(chr) && isSep(lst)) { lst = chr; continue; }
		/*--------------------------------------------------------*/
		/*finds start of comment*/
		if(chr=='/' && nxt=='*' && !isStr) {/*finding start of comment and it is not within string (double quotes)*/
			isCmt = true;
			wrd += chr;/*chr is /*/
			lst = chr;
			continue;
		}
		/*finds start of string*/
		else if (chr == '"' && !isStr && !isCmt) {/*id chr is " such that, there is no ongoing string or comment*/
			isStr = true;
			wrd += chr;/*chr is "*/
			lst = chr;
			continue;
		}
		/*finds comment end*/
		else if (lst == '*' && chr == '/' && isCmt && !isStr) {
			wrd += chr;/*chr is /*/
			isCmt = false;
			lst = chr;
			continue;
		}
		/*finds string end*/
		else if (chr == '"' && lst != '\\' && isStr && !isCmt) {
			if (lst == '\\') {Print::log("lst is \\");}
			wrd += chr;/*chr is "*/
			isStr = false;
			lst = chr;
			continue;
		}
		/*--------------------------------------------------------*/
		/*adds chr into wrd (as a comment)*/
		if (isCmt) { wrd += chr;	lst = chr;	continue; }
		/*adds chr into wrd (as a string)*/
		else if (isStr) { wrd += chr;	lst = chr;	continue; }
		/*--------------------------------------------------------*/
		if (isSep(chr) && !wrd.empty()) {/*use separators as primary delimiters to sort most of the code*/
			tokns.push_back(wrd);	wrd.clear();
		}
		else if (chr == ';' /*&& !wrd.empty()*/) {/*avoid empty statements*/
			if (lst == ';' && wrd.empty()) { lst = chr; continue; }
			
			tokns.push_back(wrd);	wrd.clear();
			tokns.push_back(";");
		}
		else if (chr == '{' && !wrd.empty()) {/*use { as delimiter for Blocks(ex, Loops)*/
			tokns.push_back(wrd);	wrd.clear();
			tokns.push_back("{");
		}
		else if (chr == '}' && !wrd.empty()) {
			tokns.push_back("}");
		}
		else {
			if(!isSep(chr)) {
				wrd += chr;
			}
		}

		lst = chr; /*current char becomes last char in next iteration!*/
	}

	if (tokns.empty()) {
		Print::error("tokenization failed due to false or empty code!");
		exit(1);
	}
	else {
		return tokns;
	}
}

StringTree* Lexer::lexTree(StrVector _tokens) {
	StringTree* _tree = new StringTree();
	_tree->insert(nullptr, "");

	std::string _codeStr = "", _word = ""/*, _nextWord = ""*/;
	for (size_t i = 0; i < _tokens.size(); i++) {
		_word = _tokens[i];
		//_nextWord = ((i + 1) < tokens.size()) ? tokens[i+1] : "";
		if (_word == ";") {
			//std::cout << "added: |" << _codeStr << "|\n";
			_tree->insert(_tree->currentNode, _codeStr);
			_tree->goToParent();
			_codeStr.clear();
		}
		else if (_word.starts_with("/*")) {
			//std::cout << "added: |" << _word << "|\n";
			_tree->insert(_tree->currentNode, _word);
			_tree->goToParent();
			_codeStr.clear();
		}
		else if (_word == "{") {
			//std::cout << "added: |" << _codeStr << "|\n";
			_tree->insert(_tree->currentNode, _codeStr);
			/*don't go to parent this time*/
			_codeStr.clear();
		}
		else if (_word == "}") {
			_tree->goToParent();
		}
		else {
			_codeStr += _word + ' ';
		}
	}

	return _tree;
}

void Lexer::printLex(const StrVector& _tokens) {
	for (const String& _ : _tokens) {
		std::cout << "|" << _ << "|\n";
	}
}
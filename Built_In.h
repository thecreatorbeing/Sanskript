#ifndef BUILT_IN_H
#define BUILT_IN_H

/*this header imports builtin libraries,
along with proper name to the used classes*/

#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<regex>
#include<stdexcept>
#include<map>
#include <unordered_map>

/*std::string*/
using String = std::string;

/*std::vector<std::String>*/
using StrVector = std::vector<String>;

/*std::vector<std::vector<std::string>>*/
using VecStrVector = std::vector<StrVector>;

/*std::map<string, std::vector<string>>*/
using CodeMap = std::map<String, StrVector>;

#endif // !BUILT_IN_H
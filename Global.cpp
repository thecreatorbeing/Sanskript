#include"Global.h"

/*this is a vector of vectors of strings(acting as stack) which stores <blockName, Identifier> e.g, <"class", "HumanClass">
this will help to prevent declaring specific blocks which are not allowed to be used inside others
for ex, class inside functions, class inside loop etc!*/
VecStrVector Global::blockStack = {};

/*this flag will stop the code execution if 'true'*/
bool Global::errorFlag = false;
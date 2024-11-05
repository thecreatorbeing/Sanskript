#ifndef GLOBAL_H
#define GLOBAL_H

#include"Built_In.h"

extern class Global {
public:

    static bool errorFlag;
    static VecStrVector blockStack;

    /*Update: this function will determine that which Block types can be nested into each other.
    For ex, a class definition cannot be nested inside of a function*/
    static void addBlockToStack(const String& _blkType, const String& _blkName) {
        //std::vector<std::string> _temp;

        Global::blockStack.push_back({ _blkType, _blkName });
        //Print::warning("added '" + _blkType + "' : '" + _blkName + "'");
    }

    /*either returns index or -1*/
    static int findInBlockStack(String _blockName) {
        for (int i = Global::blockStack.size() - 1; i > -1; i--) {
            StrVector _entry = Global::blockStack[i];
            if (_entry[0] == _blockName) {
                return i;
            }
        }
        return -1;
    }

};



#endif // !GLOBAL_H
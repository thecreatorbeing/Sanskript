#ifndef CONSOLE_PRINT_UTIL_H
#define CONSOLE_PRINT_UTIL_H

#include"Built_In.h"

class Print {
public:

    static void red(const String& _msg) {
        std::cout << "\033[1;31m" << _msg << "\033[0m" << std::endl;
    }
    static void yellow(const String _msg) {
        std::cout << "\033[0;33m" << _msg << "\033[0m" << std::endl;
    }

    static void green(const String _msg) {
        std::cout << "\033[32m" << _msg << "\033[0m" << std::endl;
    }

    static void log(const String& _msg) {
        Print::green("[Log] " + _msg);
    }

    static void warning(const String& _msg) {
        Print::yellow("[warning] " + _msg);
    }

    static void error(const String& _msg) {
        Print::red("[error] " + _msg);
    }

    static void map(const std::map<String, StrVector>& _map) {
        String temp = "";
        std::cout << "\033[0;33m[map]\n";
        for (const auto& entry : _map) {
            const String& key = entry.first;
            const StrVector& values = entry.second;

            temp += "Key: |" + key + "| Value: ";
            for (const auto& value : values) {
                temp += "|" + value + "|";
            }
            temp += "\n";
        }
        std::cout << temp << "\033[0m\n";
    }
    
    static void strVector(const StrVector& _sVec) {
        std::cout << "\033[0;33m[str-Vector]: |";
        for (const String& _s : _sVec) {
            std::cout << _s << "|";
        }
        std::cout << "\033[0m\n" << std::endl;
    }
};

#endif // !CONSOLE_PRINT_UTIL_H
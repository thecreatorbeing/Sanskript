#include <iostream>
#include <fstream>
#include <sstream>

#include"Parser.h"
#include"Built_In.h"
//#include"Playground.h"
#include"Lexer.h"
#include "Expression.h"
#include "CodeGen.h"


int main(int argCount, char* argValues)
{
    String _inputFileName = "main.krit";/*default Sanskript file name*/
    String _outputFileName = "exported_main.cpp";/*default C++ file name*/

    //std::cout << "Sanskript Programming Language!: \n" << std::endl;

    std::string code = "";
    
    
    if (argCount == 5) {/*either 1(file name i.e. default) or 5(file name + 4 arguments)*/
        /*everything is fine!*/
        std::map<String, String> argMap;
        const char* iflag = "-i";
        const char* oflag = "-o";/*input and output flags resp.*/
        

        std::cout << argValues[1] << std::endl;
        
        if ( std::to_string(argValues[1])!=iflag && std::to_string(argValues[1])!=oflag) {
            Print::red("1st");
            std::cout << argValues[1] << " is " << std::to_string(argValues[1]) << std::endl;
            Print::error("invalid flag: '" + std::to_string(argValues[1]) +"' passed!");
            exit(1);
        }
        if (std::to_string(argValues[3]) != iflag && std::to_string(argValues[3]) != oflag ) {
            Print::red("2nd");
            std::cout << argValues[3] << " is " << std::to_string(argValues[3]) << std::endl;
            Print::error("invalid flag: '" + std::to_string(argValues[3]) + "' passed!");
            exit(1);
        }

        /*1 & 3 are flags, 2 and 4 are values*/
        argMap[std::to_string(argValues[1])] = argValues[2];
        argMap[std::to_string(argValues[3])] = argValues[4];

        if ( !(argMap[iflag]).ends_with(".krit")) {/*if input file name doesnt ends with '.krit' i.e, Sanskript file extension*/
            Print::error("invalid input file: '" + argMap[iflag] + "' passed!");
        }

        if ( !(argMap[oflag]).ends_with(".cpp")) {/*if output file name doesnt ends with '.cpp' i.e, C++ file extension*/
            Print::error("invalid output file: '" + argMap[oflag] + "' passed!");
        }


        _inputFileName = argMap[iflag];
        _outputFileName = argMap[oflag];

        Print::green("inputfile: '" + _inputFileName + "', outputfile: '" + _outputFileName + '\'');
    }
    else if(argCount == 1){
        /*everything is fine! defalut file names will be used for Sanskript and C++ files*/
    }
    else{
        /*nothing is fine, give error!*/
        Print::error("wrong number of params passed!");
        exit(1);
    }

    /*file reading and copying code into code variable*/
    //{
        std::ifstream myFile;
        myFile.open(_inputFileName);

        if (!myFile.is_open()) {
            //std::cerr << "Error: Could not open file: 'main.krit'" << std::endl;
            Print::error("Could not open file: 'main.krit'");
            return 1;
        }

        std::stringstream strStream;
        strStream << myFile.rdbuf(); // Read the file into the stream

        code = strStream.str();

        myFile.close();
    //}


    Class::initializePrimitiveDataTypes();
    Operator::initializeBasicOperators();


    /*----------------LEXICAL-ANALYSIS---------------*/
    StrVector tokens = Lexer::lex(code);
    //Lexer::printLex(tokens);

    StringTree* strTree = Lexer::lexTree(tokens);
    //StringTree::preOrderTraversal(strTree->rootNode);

    /*-----------SYNTAX-&-SEMANTIC-ANALYSIS----------*/

    Tree* tree = Parser::parse(strTree->rootNode);/*Basic code supported!*/
    //Print::green("-------------------------------------------");
    //tree->preOrderTraversal(tree->root);

    /*Variable::printTable();
    Function::printTable();*/ /*debug/testing purpose*/

    
    /*update: support this important feature!*/
    /*String expString = "num1 + num2";
    ExpressionValidator expr = ExpressionValidator(expString);*/

    /*-------------CPP-CODE-GENERATION--------------*/

    CodeGen generator = CodeGen(tree, _outputFileName);
    CodeGen::generateCppCode(generator.parseTree->root, generator._genCodeString);

    /*Print::green("-------------------------------------------");
    Print::yellow("Generated CPP Code!");
    Print::green("-------------------------------------------");*/
    //Print::yellow(generator._genCodeString);

    /*---------------Exporting_CPP_Code---------------*/
   
    std::ofstream outFile(_outputFileName); // Create and open a text file 
    if (outFile.is_open() && !Global::errorFlag) { 
        outFile << generator._genCodeString << std::endl; // Write to the file 
        outFile.close(); // Close the file 
        
        //Print::green("-------------------------------------------");
        Print::yellow("Code Saved!");
        //Print::green("-------------------------------------------");
    }
    else {
        //Print::green("-------------------------------------------");
        Print::error("Code save failed!");
        //Print::green("-------------------------------------------");
    }
    
    return 0;
}
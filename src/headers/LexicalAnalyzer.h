#ifndef COMPILER_LEXICALANALYZER_H
#define COMPILER_LEXICALANALYZER_H

#include "SymbolTable.h"
#include <iostream> //For isalpha(), isdigit(), etc.
#include <fstream>
using namespace std;

/**
 * Handles all Lexical Analyzer logic in the compiler.
 */
class LexicalAnalyzer {

public:
    //Constructor
    LexicalAnalyzer();

    //This will run the lexical
    void runLexer(ifstream& fileIn);

private:

    SymbolTable symbolTable;
    unordered_map<string, string> lexerOutput; //Holds the lexer's final output in format <Lexeme, Token>

    enum DFA_Inputs { //Skips 0 because it is the DFA's state label column
        letter = 1,
        digit,       //2
        decimalPoint,//3
        dollarSign,  //4
        whiteSpace   //5
    };

    int DFA_State_Table;//This will be out 2D array of states once we make it


};

#endif
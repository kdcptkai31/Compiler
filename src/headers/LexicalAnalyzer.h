#ifndef COMPILER_LEXICALANALYZER_H
#define COMPILER_LEXICALANALYZER_H

#include "SymbolTable.h"
#include <iostream> //For isalpha(), isdigit(), etc. in the DFA
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
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
    void printOutputToFile(ofstream& fileOut);

private:

    void generateMeaningfulUnits(ifstream& fileIn);
    //Helper Functions
    bool isMeaningfulUnitSeparator(char c);
    bool DFA(string meaningfulUnit, int& acceptedStateType);
    bool isWhiteSpace(char c);
    int getUnitType(string s);
    bool stringIsNum(string s);
    int colNum(char ch);

    //Data
    SymbolTable symbolTable;                    //Holds all the assigned lexemes
    vector<string> meaningfulUnits;            //Holds the initially separated substrings from the file
    vector<pair<string, string>> lexerOutput; //Holds the lexer's final output in format <Token, Lexeme>

    enum DFA_Inputs { //Skips 0 because it is the DFA's state label column
        letter = 1,
        digit,       //2
        decimalPoint,//3
        dollarSign,  //4
        signs,  //added for the DFA table
        unknown
    };

    int DFA_State_Table;//This will be out 2D array of states once we make it

};

#endif
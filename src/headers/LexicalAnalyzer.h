#ifndef COMPILER_LEXICALANALYZER_H
#define COMPILER_LEXICALANALYZER_H

#include "SymbolTable.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/**
 * Handles all Lexical Analyzer logic in the compiler, including generating meaningful units, and the Deterministic
 * State Automata.
 */
class LexicalAnalyzer {

public:
    LexicalAnalyzer();
    void run(ifstream& fileIn);            //This will run the lexical
    void printOutputToFile(ofstream& fileOut);  //Prints the output vector to a given file stream

    //Getters
    vector<pair<string, string>>* getLexerOutput(){return &lexerOutput;}

private:

    void generateMeaningfulUnits(ifstream& fileIn);         //Reads inputs from a given file stream
    bool isMeaningfulUnitSeparator(char c, ifstream &input, string unit);                 //Determines if a character is a meaningful unit separator
    static bool DFA(const string& meaningfulUnit, int& acceptedStateType);//Determines if a unit is valid; then determines its type
    int getUnitType(const string& s);
    static bool stringIsNum(const string& s);
    static int colNum(char ch);

    //Data
    SymbolTable symbolTable;                  //Holds all the assigned lexemes
    vector<string> meaningfulUnits;           //Holds the initially separated substrings from the file
    vector<pair<string, string>> lexerOutput; //Holds the lexer's final output in format <Token, Lexeme>

    enum DFA_Inputs { //Skips 0 because it is the DFA's state label column
        letter = 1,
        digit,
        dollarSign,
        decimalPoint,
        signs,
        unknown //Character other than the accepted language for this assignment
    };

};

#endif
#ifndef COMPILER_SYNTACTICANALYZER_H
#define COMPILER_SYNTACTICANALYZER_H

#include "LexicalAnalyzer.h"
#include <stack>
#include<queue>

using namespace std;

class SyntacticAnalyzer{

public:

    SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput);
    ~SyntacticAnalyzer();
    bool run();

    //Boolean Rules
    bool isStatement();
    bool isDeclarative();
    bool isDeclarativePrime();
    bool isAssign();
    bool isExpression();
    bool isExpressionPrime();
    bool isTerm();
    bool isTermPrime();
    bool isFactor();


private:
    SymbolTable symbolTable;
    vector<pair<string, string>>* lexerOutput;

    stack<string> productionOutputs;//Used to collect production rule output
    stack<pair<string, string>> currentStatement;//Used to collect production rule output
    pair<string, string> tmp;
    vector<vector<string>> memoryTable;
    int memoryAddress;
    string declareType;
    int tokenIndex;                 //Used to construct potential statements from the lexer output
    int statementParser;            //Keeps track of the current lexeme being analyzed in each statement
    int statementCounter;           //Keeps track of the line number for error checks
    ofstream fout;
    bool foutOpened;                //True if they file is opened, false if no output is wanted

    //Helpers
    void addToMemoryTable(const string &s);
    void isInMemoryTable(const string& str);
};

#endif

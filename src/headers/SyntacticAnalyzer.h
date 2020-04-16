#ifndef COMPILER_SYNTACTICANALYZER_H
#define COMPILER_SYNTACTICANALYZER_H

#include "LexicalAnalyzer.h"
#include <stack>

using namespace std;

class SyntacticAnalyzer{

public:

    SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput);
    ~SyntacticAnalyzer();
    void run();

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
    vector<pair<string, string>> currentStatement;

    stack<string> productionOutputs;
    int tokenIndex;
    int statementParser;
    ofstream fout;
    bool foutOpened;

    //Helpers
    void incrementParser();
    bool isNumber();

};

#endif

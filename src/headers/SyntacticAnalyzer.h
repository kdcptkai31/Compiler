#ifndef COMPILER_SYNTACTICANALYZER_H
#define COMPILER_SYNTACTICANALYZER_H

#include "LexicalAnalyzer.h"

using namespace std;


class SyntacticAnalyzer{

public:

    SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput);
    ~SyntacticAnalyzer();
    void run();

    //Boolean Rules
    bool isStatementList();
    bool isStatement();
    bool isDeclarative();
    bool isAssign();
    bool isExpression();
    bool expressionPrime();
    bool isTerm();
    bool termPrime();
    bool isFactor();
    bool isNumber();

private:
    SymbolTable symbolTable;
    vector<pair<string, string>>* lexerOutput;
    int tokenIndex;
    ofstream fout;
    bool foutOpened;
    vector<string> productionRuleStrings;

    void printProductionRuleStrings();
    void outputCurrentTokenAndLexeme();
    void outputStatementProduction();
    bool isId();
    bool isSemicolon();

};

#endif

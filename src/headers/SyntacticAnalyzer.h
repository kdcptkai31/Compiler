#ifndef COMPILER_SYNTACTICANALYZER_H
#define COMPILER_SYNTACTICANALYZER_H

#include "LexicalAnalyzer.h"

using namespace std;


class SyntacticAnalyzer{

public:

    SyntacticAnalyzer(bool printProductions);
    ~SyntacticAnalyzer();
    void run(vector<pair<string, string>>* lexerOutput);

private:
    ofstream* fout;

};

#endif

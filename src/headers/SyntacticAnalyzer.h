#ifndef COMPILER_SYNTACTICANALYZER_H
#define COMPILER_SYNTACTICANALYZER_H

#include "LexicalAnalyzer.h"

using namespace std;


class SyntacticAnalyzer{

public:

    SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput);
    void run(ofstream& fout);

private:
    vector<pair<string, string>>* lexerOutput;

};

#endif

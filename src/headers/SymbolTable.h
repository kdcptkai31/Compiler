#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;

/**
 * Holds the instructor assigned symbols for this compiler, as well as the lookup functions, to be used by the entire
 * compiler.
 */
class SymbolTable {

public:
    SymbolTable();//Initializes tables with their values
    vector<string> separators;
    vector<string> operators;
    vector<string> keywords;
    bool isSeparator(const string& s);
    bool isOperator(const string& s);
    bool isKeyword(const string& s);

};

#endif
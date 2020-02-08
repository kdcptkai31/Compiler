#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <unordered_map>
using namespace std;

/**
 * Holds the instructor assigned symbols for this compiler, as well as the lookup functions, to be used by the entire
 * compiler.
 */
class SymbolTable {

public:
    SymbolTable();
    //Unordered maps are used for O(1) lookup times
    unordered_map<string, string> separators;
    unordered_map<string, string> operators;
    unordered_map<string, string> keywords;
    bool isSeparator(string s);
    bool isOperator(string s);
    bool isKeyword(string s);

private:

};

#endif
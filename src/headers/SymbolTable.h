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
    //vector<pair<string, string>> vec;
    unordered_multimap<string, string> separators;
    unordered_multimap<string, string> operators;
    unordered_multimap<string, string> keywords;
    bool isSeparator(string s);
    bool isOperator(string s);
    bool isKeyword(string s);

private:

};

#endif
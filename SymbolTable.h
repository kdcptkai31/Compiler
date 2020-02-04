#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <vector>
#include <string>
using namespace std;

static class SymbolTable {

public:
    vector<string> tokens = {"Keyword", "Identifier", "Separator", "Operator", "Number", "Comment"};
    vector<string> keywords = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while",
                               "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not"};

};


#endif
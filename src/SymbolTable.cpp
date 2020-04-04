#include "headers/SymbolTable.h"

/**
 * Constructor - builds the instructor's list of separators, operators, and keywords to be used throughout the compiler.
 */
SymbolTable::SymbolTable(){

    separators.insert(separators.end(), {"'", "(", ")", "{", "}", "[", "]", ",", ".", ":", ";"});
    operators.insert(operators.end(), {"*", "+", "-", "=", "/", ">", "<", "%"});
    keywords.insert(keywords.end(), {"int", "float", "bool", "true", "false", "if", "else", "then", "endif",
                                                 "while", "whileend", "do", "doend", "for", "forend", "input", "output",
                                                 "and", "or", "not"});
    types.insert(types.end(), {"int", "float", "bool"});

}

/**
 *
 * @param s
 * @return True if the char is a separator, false if not
 */
bool SymbolTable::isSeparator(const string& s) {

    auto it = find(separators.begin(), separators.end(), s);
    return it != separators.end();

}

/**
 *
 * @param s
 * @return True if the char is an operator, false if not.
 */
bool SymbolTable::isOperator(const string& s) {

    auto it = find(operators.begin(), operators.end(), s);
    return it != operators.end();

}

/**
 *
 * @param s
 * @return True if the string is a keyword, false if not.
 */
bool SymbolTable::isKeyword(const string& s) {

    auto it = find(keywords.begin(), keywords.end(), s);
    return it != keywords.end();

}

bool SymbolTable::isType(const string &s) {

    auto it = find(types.begin(), types.end(), s);
    return it != types.end();

}
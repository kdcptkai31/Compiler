#include "headers/SymbolTable.h"

/**
 * Constructor - builds the instructor's list of separators, operators, and keywords to be used throughout the compiler.
 */
SymbolTable::SymbolTable(){

    separators.insert({{"'", "separator"}, {"(", "separator"}, {")", "separator"},
                       {"{", "separator"}, {"}", "separator"}, {"[", "separator"},
                       {"]", "separator"}, {",", "separator"}, {".", "separator"},
                       {":", "separator"}, {";", "separator"}, {" ", "separator"}});

    operators.insert({{"*", "operator"}, {"+", "operator"}, {"-", "operator"},
                       {"=", "operator"}, {"/", "operator"}, {">", "operator"},
                       {"<", "operator"}, {"%", "operator"}});

    keywords.insert({{"int", "keyword"}, {"float", "keyword"}, {"bool", "keyword"},
                       {"true", "keyword"}, {"false", "keyword"}, {"if", "keyword"},
                       {"else", "keyword"}, {"then", "keyword"}, {"endif", "keyword"},
                       {"while", "keyword"}, {"whileend", "keyword"}, {"do", "keyword"},
                       {"doend", "keyword"}, {"for", "keyword"}, {"forend", "keyword"},
                       {"input", "keyword"}, {"output", "keyword"}, {"and", "keyword"},
                       {"or", "keyword"}, {"not", "keyword"}});
}

/**
 *
 * @param s
 * @return True if the char is a separator, false if not
 */
bool SymbolTable::isSeparator(string s) {

    auto it = separators.find(s);
    if(it != separators.end())
        return true;

    return false;

}

/**
 *
 * @param s
 * @return True if the char is an operator, false if not.
 */
bool SymbolTable::isOperator(string s) {

    auto it = operators.find(s);
    if(it != operators.end())
        return true;

    return false;

}

/**
 *
 * @param s
 * @return True if the string is a keyword, false if not.
 */
bool SymbolTable::isKeyword(string s) {

    auto it = keywords.find(s);
    if(it != keywords.end())
        return true;

    return false;

}
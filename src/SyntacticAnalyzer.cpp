#include "headers/SyntacticAnalyzer.h"

/**
 * Constructor that opens an output file if it was initialized to output to a file.
 */
SyntacticAnalyzer::SyntacticAnalyzer(bool printProductions) {

    if(printProductions)
        fout->open("output.txt", ifstream::out);
    else
        fout = NULL;

}

/**
 * Destructor that closes the output file if it was initialized to output to a file.
 */
SyntacticAnalyzer::~SyntacticAnalyzer() { if(fout) fout->close(); }

/**
 * Runs the list of tokens and lexemes from the lexer through the syntactic analyzer
 * @param lexer
 */
void SyntacticAnalyzer::run(vector<pair<string, string>>* lexerOutput) {




}
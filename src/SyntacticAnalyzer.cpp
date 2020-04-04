#include "headers/SyntacticAnalyzer.h"

/**
 * Constructor that opens an output file if it was initialized to output to a file.
 */
SyntacticAnalyzer::SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput) {

    lexerOutput = lOutput;

}

/**
 * Runs the list of tokens and lexemes from the lexer through the syntactic analyzer
 * @param lexer
 */
void SyntacticAnalyzer::run(ofstream& fout) {


}
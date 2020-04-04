#include "headers/SyntacticAnalyzer.h"

/**
 * Constructor that opens an output file if it was initialized to output to a file.
 */
SyntacticAnalyzer::SyntacticAnalyzer(bool printProductions, vector<pair<string, string>>* lOutput) {

    if(printProductions){

        fout.open("output.txt", ifstream::out);
        foutOpened = true;

    } else
        foutOpened = false;

    lexerOutput = lOutput;
    tokenIndex = 0;

}

SyntacticAnalyzer::~SyntacticAnalyzer() {

    if(foutOpened)
        fout.close();
}

/**
 * Runs the list of tokens and lexemes from the lexer through the syntactic analyzer
 * @param lexer
 */
void SyntacticAnalyzer::run() {

    if(lexerOutput->size() == 0)
        return;

    outputCurrentTokenAndLexeme();

    if(isStatementList())
        return;

    return;


}

bool SyntacticAnalyzer::isStatementList(){

    if(isStatement())
        fout << "<Statement List> -> <Statement> | <Statement> <Statement List>" << endl;

    return true;


}

bool SyntacticAnalyzer::isStatement(){

    if(isDeclarative())
        return true;

    if(isAssign())
        return true;

    return false;

}

bool SyntacticAnalyzer::isDeclarative(){

    if(!symbolTable.isType(lexerOutput->at(tokenIndex).first))
        return false;

    outputStatementProduction();
    fout << "<Declarative> -> <Type> <id>\n";
    tokenIndex++;
    
    if(isId())



}

bool SyntacticAnalyzer::isAssign(){

    if(!isId())
        return false;

    outputStatementProduction();
    fout << "<Assign> -> <id> = <Expression>\n";
    tokenIndex++;

    if(isExpression())

}

bool SyntacticAnalyzer::isId(){

    return lexerOutput->at(tokenIndex).first == "IDENTIFIER";

}

void SyntacticAnalyzer::outputStatementProduction(){

    fout << "<Statement> -> <Declarative> | <Assign>\n";

}

void SyntacticAnalyzer::outputCurrentTokenAndLexeme(){

    fout << "Token: " << setw(10) << left << lexerOutput->at(tokenIndex).first
         << "Lexeme: " << lexerOutput->at(tokenIndex).second << endl;

}

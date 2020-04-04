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
        printProductionRuleStrings();


}

bool SyntacticAnalyzer::isStatementList(){

    if(isStatement())
        productionRuleStrings.insert(productionRuleStrings.end(),
                                     "<Statement List> -> <Statement> | <Statement> <Statement List>\n");


    return true;


}

bool SyntacticAnalyzer::isStatement(){

    if(isDeclarative()){
        outputStatementProduction();
        return true;
    }

    if(isAssign()){
        outputStatementProduction();
        return true;
    }

    return false;

}

bool SyntacticAnalyzer::isDeclarative(){

    if(!symbolTable.isType(lexerOutput->at(tokenIndex).first))
        return false;

    productionRuleStrings.insert(productionRuleStrings.end(),
                                 "<Declarative> -> <Type> <id>\n");
    tokenIndex++;

    if(isId())



}

bool SyntacticAnalyzer::isAssign(){

    if (isId())
    {
        productionRuleStrings.insert(productionRuleStrings.end(),
            "<Assign> -> <id> = <Expression>\n");
        tokenIndex++;
        if (lexerOutput->at(tokenIndex).second == "=")
        {
            tokenIndex++;
            if (isExpression())
            {
                tokenIndex++;
                if (lexerOutput->at(tokenIndex).second == ";")
                    return true;
                else
                {
                    //report error
                    return false;
                }
            }
            else
            {
                //report error
                return false;
            }
        }
        else
        {
            //report error
            return false;
        }
    }
    else
    {
        //report error
        return false;
    }
}

void SyntacticAnalyzer::printProductionRuleStrings(){

    if(!foutOpened)
        return;

    for(int i = 0; i < productionRuleStrings.size(); i++)
        fout << productionRuleStrings.at(i);

    fout << endl;

}


bool SyntacticAnalyzer::isId(){

    return lexerOutput->at(tokenIndex).first == "IDENTIFIER";

}

void SyntacticAnalyzer::outputStatementProduction(){

    productionRuleStrings.insert(productionRuleStrings.end(),
                                  "<Statement> -> <Declarative> | <Assign>\n");

}

void SyntacticAnalyzer::outputCurrentTokenAndLexeme(){

    if(!foutOpened)
        return;

    fout << "Token: " << setw(10) << left << lexerOutput->at(tokenIndex).first
         << "Lexeme: " << lexerOutput->at(tokenIndex).second << endl;

}

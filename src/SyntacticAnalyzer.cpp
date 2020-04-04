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

    while(tokenIndex != lexerOutput->size() && isStatementList()){

        printProductionRuleStrings();
        productionRuleStrings.clear();

    }

}

bool SyntacticAnalyzer::isStatementList(){

    outputCurrentTokenAndLexeme();

    if(isStatement()) {
        productionRuleStrings.insert(productionRuleStrings.end(),
                                     "<Statement List> -> <Statement> | <Statement> <Statement List>\n");

        return true;
    }

    cout << "ERROR DETECTED\n";
    return false;


}

bool SyntacticAnalyzer::isStatement(){

    if(isDeclarative()){
        outputStatementProduction();
        return true;
    }

//    if(isAssign()){
//        outputStatementProduction();
//        return true;
//    }

    return false;

}

bool SyntacticAnalyzer::isDeclarative(){

    if(!symbolTable.isType(lexerOutput->at(tokenIndex).second))
        return false;

    productionRuleStrings.insert(productionRuleStrings.end(),
                                 "<Declarative> -> <Type> <id> ;\n");
    tokenIndex++;

    outputCurrentTokenAndLexeme();
    if(isId()) {
        tokenIndex++;
        outputCurrentTokenAndLexeme();
        if (isSemicolon()) {
            tokenIndex++;
            return true;

        } else {

            if (foutOpened)
                fout << "ERROR R2 violated- expected a ';' after the identifier\n";
            else
                cout << "ERROR R2 violated- expected a ';' after the identifier\n";

        }

    }

}

//bool SyntacticAnalyzer::isAssign(){
//
//    if (!isId()){
//
//        //report error
//        return false;
//
//    }
//
//    productionRuleStrings.insert(productionRuleStrings.end(),
//            "<Assign> -> <id> = <Expression>\n");
//    tokenIndex++;
//    if (lexerOutput->at(tokenIndex).second == "="){
//
//        tokenIndex++;
//        if (isExpression()){
//
//            tokenIndex++;
//            if (lexerOutput->at(tokenIndex).second == ";")
//                    return true;
//                else{
//
//                    //report error
//                    return false;
//                }
//        }else{
//
//                //report error
//                return false;
//            }
//    }
//
//}
//
//bool SyntacticAnalyzer::isExpression() {
//
//
//
//}

void SyntacticAnalyzer::printProductionRuleStrings(){

    if(!foutOpened)
        return;

    for(vector<string>::iterator it = productionRuleStrings.begin(); it != productionRuleStrings.end(); ++it)
        fout << *it;

    fout << "--------------------------------------\n";


}


bool SyntacticAnalyzer::isId(){

    return lexerOutput->at(tokenIndex).first == "IDENTIFIER";

}

bool SyntacticAnalyzer::isSemicolon(){

    return lexerOutput->at(tokenIndex).second == ";";

}

void SyntacticAnalyzer::outputStatementProduction(){

    productionRuleStrings.insert(productionRuleStrings.end(),
                                  "<Statement> -> <Declarative> | <Assign>\n");

}

void SyntacticAnalyzer::outputCurrentTokenAndLexeme(){

    if(!foutOpened)
        return;

    productionRuleStrings.insert(productionRuleStrings.end(),
            "\nToken: " + lexerOutput->at(tokenIndex).first + " Lexeme: " + lexerOutput->at(tokenIndex).second + "\n");

}

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
        
        productionRuleStrings.insert(productionRuleStrings.begin()+1,
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

    if(isAssign()){
        outputStatementProduction();
        return true;
    }

    return false;

}

bool SyntacticAnalyzer::isDeclarative(){

    if(!symbolTable.isType(lexerOutput->at(tokenIndex).second))
        return false;

    productionRuleStrings.insert(productionRuleStrings.begin()+1,
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

bool SyntacticAnalyzer::isAssign(){
    if (!isId())
        return false;

    productionRuleStrings.insert(productionRuleStrings.end(),
            "<Assign> -> <id> = <Expression>\n");
    tokenIndex++;
    outputCurrentTokenAndLexeme();
    if (lexerOutput->at(tokenIndex).second == "="){
        tokenIndex++;
        outputCurrentTokenAndLexeme();
        if (isExpression()){
            tokenIndex++;

            if (lexerOutput->at(tokenIndex).second == ";")
                    return true;
            else
                productionRuleStrings.insert(productionRuleStrings.end(),
                    "R4 Error = Expected lexeme: ;\n");
        }
        else
            productionRuleStrings.insert(productionRuleStrings.end(),
                "R4 Error = Expected integer | real | id | (<expression>)\n");
    }
    return false;
}

bool SyntacticAnalyzer::isFactor() {
    productionRuleStrings.insert(productionRuleStrings.end(),
        "<Factor> -> (<Expression>) | <id> | <num>\n");

    if (lexerOutput->at(tokenIndex).second == "(") {
        tokenIndex++;
        productionRuleStrings.insert(productionRuleStrings.end(),
            "<factor> -> (<expression>)\n");

        if (isExpression()) {
            tokenIndex++;

            if (lexerOutput->at(tokenIndex).second == ")") {
                return true;
            }
            else
                productionRuleStrings.insert(productionRuleStrings.end(),
                    "R9 Error = Expected lexeme after expression: )\n");
        }
        else
            productionRuleStrings.insert(productionRuleStrings.end(),
                "R9 Error = Expected expression after lexeme: (\n");
    }
    
    else if (isId()) {
        productionRuleStrings.insert(productionRuleStrings.end(),
            "<factor> -> <id>\n");
        return true;
    }

    else if (isNumber()) {
        return true;
    }

    else
        return false;
}

bool SyntacticAnalyzer::isNumber() {
    return lexerOutput->at(tokenIndex).first == "INTEGER" || lexerOutput->at(tokenIndex).first == "REAL";
}


bool SyntacticAnalyzer::isExpression() {
    productionRuleStrings.insert(productionRuleStrings.end(),
        "<Expression> ->  <Term> < Expression'>\n");

    if (isTerm()) {
        tokenIndex++;

        if (lexerOutput->at(tokenIndex).second == ";") {
            productionRuleStrings.insert(productionRuleStrings.end(),
                "<Expression'> -> + <Term> <Expression'> | - <Term> <Expression'>  | epsilon\n");
            outputCurrentTokenAndLexeme();
            tokenIndex--;
            return true;
        }

        else if (expressionPrime()) {
            tokenIndex--;
            return true;
        }
    }
    else
        productionRuleStrings.insert(productionRuleStrings.end(),
            "R9 Error = Expected integer | real | id | (<expression>)\n");
    return false;
}

bool SyntacticAnalyzer::expressionPrime() {
    productionRuleStrings.insert(productionRuleStrings.end(),
        "<Expression'> -> + <Terminal> <Expression'> | - <Terminal> <Expression'>  | epilson\n");
    outputCurrentTokenAndLexeme();

    if (lexerOutput->at(tokenIndex).second == ";") {
        return true;
    }
    
    if (lexerOutput->at(tokenIndex).second == "+" || lexerOutput->at(tokenIndex).second == "-") {
        tokenIndex++;
        outputCurrentTokenAndLexeme();

        if (isTerm()) {
            tokenIndex++;
            return expressionPrime();
        }
    }

    else
        productionRuleStrings.insert(productionRuleStrings.end(),
            "R6 Error = Expected lexeme: + | - | ;\n");
    return false;
}

bool SyntacticAnalyzer::isTerm() {
    productionRuleStrings.insert(productionRuleStrings.end(),
        "<Term> -> <Factor> < Term'>\n");

    if (isFactor()) {
        tokenIndex++;
        if (lexerOutput->at(tokenIndex).second == ";") {
            productionRuleStrings.insert(productionRuleStrings.end(),
                "<Term'> -> * <Factor> <Term'> | / <Factor> < Term'>  | epsilon\n");
            tokenIndex--;
            return true;
        }

        else if (lexerOutput->at(tokenIndex).second == "+" || lexerOutput->at(tokenIndex).second == "-") {
            tokenIndex--;
            return true;
        }

        else if (termPrime()) {
            tokenIndex--;
            return true;
        }
    }
    else
        productionRuleStrings.insert(productionRuleStrings.end(),
            "R9 Error = Expected integer | real | id | (<expression>)\n");
    return false;
}

bool SyntacticAnalyzer::termPrime() {
    productionRuleStrings.insert(productionRuleStrings.end(),
        "<Term'> -> * <Factor> <Term'> | / <Factor> < Term'>  | epsilon\n");
    if(lexerOutput->at(tokenIndex).second != ";")
        outputCurrentTokenAndLexeme();

    if (lexerOutput->at(tokenIndex).second == ";") {
        return true;
    }
    if (lexerOutput->at(tokenIndex).second == "/" || lexerOutput->at(tokenIndex).second == "*") {
        tokenIndex++;
        outputCurrentTokenAndLexeme();

        if (isFactor()) {
            tokenIndex++;
            return termPrime();
        }
    }
    else
        productionRuleStrings.insert(productionRuleStrings.end(),
            "R8 Error = Expected lexeme: / | * | ;\n");
    return false;
}

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

    productionRuleStrings.insert(productionRuleStrings.begin()+1,
                                  "<Statement> -> <Declarative> | <Assign>\n");

}

void SyntacticAnalyzer::outputCurrentTokenAndLexeme(){

    if(!foutOpened)
        return;

        productionRuleStrings.insert(productionRuleStrings.end(),
            "\nToken: " + lexerOutput->at(tokenIndex).first + " Lexeme: " + lexerOutput->at(tokenIndex).second + "\n");
}

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
    statementCounter = 1;

}

SyntacticAnalyzer::~SyntacticAnalyzer() {

    if(foutOpened)
        fout.close();
}

/**
 * Runs the list of tokens and lexemes from the lexer through the syntactic analyzer
 * @param lexer
 */
bool SyntacticAnalyzer::run() {

    bool analyzerPassed = true;

    do{

        currentStatement.emplace_back(lexerOutput->at(tokenIndex));
        while(lexerOutput->at(tokenIndex).second != ";"){

            tokenIndex++;
            currentStatement.emplace_back(lexerOutput->at(tokenIndex));
            
        }

        if(isStatement()){

            printStatementRules();
            cout << "GOOD\n";
            currentStatement.clear();
            tokenIndex++;
            statementCounter++;

        } else{

            cout << "ERROR DETECTED\n";
            analyzerPassed = false;
            break;

        }

    }while(tokenIndex < lexerOutput->size());


    return analyzerPassed;

}

bool SyntacticAnalyzer::isStatement(){
    printLexemeLine();
    statementParser = 0;

    if(isDeclarative())
        return true;
    else if(isAssign())
        return true;

    fout << "Synatax Error\n";
    return false;
}

bool SyntacticAnalyzer::isDeclarative(){

    
    if (currentStatement.at(statementParser).first == "KEYWORD") {
        productionOutputs.push("<Statement> -> <Declarative> | <Assign>");
        incrementParser();
        if (currentStatement.at(statementParser).first == "IDENTIFIER") {
            incrementParser();
            productionOutputs.push("<Declarative> -> <Type> <id> <Declarative'> ;");
            if (isDeclarativePrime()) {
                incrementParser();
                if (currentStatement.at(statementParser).second == ";")
                    return true;
            }
        }
    }
    else if (currentStatement.at(statementParser).first == "IDENTIFIER")
        return false;

    fout << "Declaration Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isDeclarativePrime(){

    if(currentStatement.at(statementParser).second == ","){
        incrementParser();
        if(currentStatement.at(statementParser).first == "IDENTIFIER"){
            incrementParser();
            if(isDeclarativePrime()) 
                return true;
        }
        //epsilon case
    }else if(currentStatement.at(statementParser).second == ";"){
        productionOutputs.push("<Declarative'> -> , <id> <Declarative'> | epsilon");
        statementParser--;
        
        return true;
    }

    fout << "Declarative' Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isAssign(){
    productionOutputs.push("<Statement> -> <Declarative> | <Assign>");
    if(currentStatement.at(statementParser).first == "IDENTIFIER"){
        incrementParser();
        if(currentStatement.at(statementParser).second == "="){
            productionOutputs.push("<Assign> -> <id> = <Expression> ;");
            incrementParser();
            if(isExpression()){
                incrementParser();
                if(currentStatement.at(statementParser).second == ";")
                    return true;
            }
        }
    }

    fout << "Assignment Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isExpression(){
    productionOutputs.push("<Expression> -> <Terminal> <Expression'>");

    if(isTerm()){
        incrementParser();
        if(isExpressionPrime())
            return true;
    }

    fout << "Expression Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isExpressionPrime(){

    if(currentStatement.at(statementParser).second == "+" || currentStatement.at(statementParser).second == "-"){
        productionOutputs.push("<Expression'> -> + <Terminal> <Expression'> | -<Terminal> < Expression'> | epsilon");

        incrementParser();
        if(isTerm()){
            incrementParser();
            if(isExpressionPrime())
                return true;
        }
        //epsilon case
    }else if(currentStatement.at(statementParser).second == ")" || currentStatement.at(statementParser).second == ";"){

        statementParser--;
        productionOutputs.push("<Expression'> -> + <Terminal> <Expression'> | -<Terminal> < Expression'> | epsilon");
        return true;

    }

    fout << "Expression' Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isTerm(){
    productionOutputs.push("<Term> -> <Factor> <Term'>");
    if(isFactor()){
        incrementParser();
        if(isTermPrime())
            return true;
    }

    fout << "Term Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isTermPrime(){

    if(currentStatement.at(statementParser).second == "*" || currentStatement.at(statementParser).second == "/"){
        productionOutputs.push("<Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | epsilon");
        incrementParser();
        if(isFactor()){
            incrementParser();
            if (isTermPrime())
                return true;
        }
        //epsilon case
    }else if(currentStatement.at(statementParser).second == ")" || currentStatement.at(statementParser).second == "+" ||
             currentStatement.at(statementParser).second == "-" || currentStatement.at(statementParser).second == ";"){

        statementParser--;
        productionOutputs.push("<Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | epsilon");
        return true;
    }

    fout << "Term' Rule Error\n";
    return false;
}

bool SyntacticAnalyzer::isFactor(){

    if(currentStatement.at(statementParser).second == "("){
        incrementParser();
        if(isExpression()) {
            incrementParser();
            if (currentStatement.at(statementParser).second == ")") {

                productionOutputs.push("<Factor> -> ( <Expression> ) | <id> | <num>");
                return true;

            }
        }
    }else if(currentStatement.at(statementParser).first == "IDENTIFIER"){
        productionOutputs.push("<Factor> -> ( <Expression> ) | <id> | <num>");
        return true;

    }else if(isNumber()){

        productionOutputs.push("<Factor> -> ( <Expression> ) | <id> | <num>");
        return true;
    }

    fout << "Factor Rule Error\n";
    return false;
}

/**
 * Makes sure the statement parser cannot go out of bounds.
 */
void SyntacticAnalyzer::incrementParser(){

    statementParser++;
    if(statementParser == currentStatement.size())
        statementParser--;
}

bool SyntacticAnalyzer::isNumber() {
    return currentStatement.at(statementParser).first == "INTEGER" ||
           currentStatement.at(statementParser).first == "REAL";
}

void SyntacticAnalyzer::printLexemeLine() {
    for (int i = 0; i < currentStatement.size(); i++) {
        fout << currentStatement.at(i).second << " ";
    }
    fout << "\n";
}

void SyntacticAnalyzer::printStatementRules() {
    while (productionOutputs.size() != 0)
    {
        fout << productionOutputs.front() << endl;
        productionOutputs.pop();
    }
    fout << "\n";
}
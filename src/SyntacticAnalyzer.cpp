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

    do{

        currentStatement.emplace_back(lexerOutput->at(tokenIndex));
        while(lexerOutput->at(tokenIndex).second != ";"){

            /*cout << productionOutputs.top() << "\n";
            productionOutputs.pop();*/

            tokenIndex++;
            currentStatement.emplace_back(lexerOutput->at(tokenIndex));
            
        }

        if(isStatement()){

            cout << "GOOD\n";
            //OutputPrintProductions();
            currentStatement.clear();
            tokenIndex++;

        } else{

            cout << "ERROR DETECTED\n";
            break;

        }

    }while(tokenIndex < lexerOutput->size());
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
}

bool SyntacticAnalyzer::isStatement(){
    
    statementParser = 0;

    if(isDeclarative()){
        productionOutputs.push("QQQQQQQQQQQQQQQQQQQQ");
        return true;
    }else if(isAssign()){
        productionOutputs.push("TTTTTTTTTTTTTTTTTTT");
        return true;
    }

    return false;

}

bool SyntacticAnalyzer::isDeclarative(){

    if(symbolTable.isType(currentStatement.at(statementParser).second)){
        incrementParser();
        if(currentStatement.at(1).first == "IDENTIFIER"){
            incrementParser();
            productionOutputs.push("UUUUUUUUUUUUUUUUU");

            if(currentStatement.at(2).second == ";"){

                productionOutputs.push("LLLLLLLLLLLLLLLLL");
                return true;

            }
        }
    }

    return false;

}

bool SyntacticAnalyzer::isAssign(){

    if(currentStatement.at(statementParser).first == "IDENTIFIER"){
        incrementParser();
        if(currentStatement.at(statementParser).second == "="){

            incrementParser();
            if(isExpression()){
                productionOutputs.push("$$$$$$$$$$$$$$$$$$");
                incrementParser();
                if(currentStatement.at(statementParser).second == ";"){

                    productionOutputs.push("MMMMMMMMMMMMMMMMMMMMMMMMMMM");
                    return true;

                }
            }
        }
    }

    return false;

}

bool SyntacticAnalyzer::isExpression(){

    if(isTerm()){
        incrementParser();
        if(isExpressionPrime()){

            productionOutputs.push("HHHHHHHHHHHHHHHH");
            return true;

        }
    }

    return false;

}

bool SyntacticAnalyzer::isExpressionPrime(){

    if(currentStatement.at(statementParser).second == "+" || currentStatement.at(statementParser).second == "-"){
        incrementParser();
        if(isTerm()){
            incrementParser();
            if(isExpressionPrime()){

                productionOutputs.push("YYYYYYYYYYYYYYYYYY");
                return true;

            }
        }
    }else if(currentStatement.at(statementParser).second == ")" || currentStatement.at(statementParser).second == ";"){

        statementParser--;
        productionOutputs.push("FFFFFFFFFFFFFFFFFFFF");
        return true;

    }

    return false;

}

bool SyntacticAnalyzer::isTerm(){

    if(isFactor()){
        incrementParser();
        if(isTermPrime()){

            productionOutputs.push("CCCCCCCCCCCCCCCCC");
            return true;

        }
    }

    return false;

}

bool SyntacticAnalyzer::isTermPrime(){

    if(currentStatement.at(statementParser).second == "*" || currentStatement.at(statementParser).second == "/"){
        incrementParser();
        if(isFactor()){
            incrementParser();
            if(isTermPrime()){

                productionOutputs.push("2222222222222222222222");
                return true;

            }
        }
    }else if(currentStatement.at(statementParser).second == ")" || currentStatement.at(statementParser).second == "+" ||
             currentStatement.at(statementParser).second == "-" || currentStatement.at(statementParser).second == ";"){

        statementParser--;
        productionOutputs.push("/////////////////////////");
        return true;

    }

    return false;

}

bool SyntacticAnalyzer::isFactor(){

    if(currentStatement.at(statementParser).second == "("){
        incrementParser();
        if(isExpression()) {
            incrementParser();
            if (currentStatement.at(statementParser).second == ")") {

                productionOutputs.push("----------------------------");
                return true;

            }
        }
    }else if(currentStatement.at(statementParser).first == "IDENTIFIER"){

        productionOutputs.push("000000000000000000000");
        return true;

    }else if(isNumber()){

        productionOutputs.push("zzzzzzzzzzzzzzzzzzzzzzzzz");
        return true;

    }

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
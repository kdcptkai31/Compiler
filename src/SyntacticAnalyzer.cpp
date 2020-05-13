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
    memoryAddress = 5000;
    vector<string> tmp;
    tmp.emplace_back("Type");
    tmp.emplace_back("Memory Location");
    tmp.emplace_back("Identifier");
    memoryTable.emplace_back(tmp);
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
    vector<pair<string, string>> tmpStatement;
    do{
        currentStatement.push(make_pair("$", "$"));
        do {
            tmp = lexerOutput->at(tokenIndex);
            tmpStatement.emplace_back(tmp);
            tokenIndex++;
        } while (tmp.second != ";");
       
        tmpStatement.pop_back();
        reverse(tmpStatement.begin(), tmpStatement.end());
        for (int i = 0; i < tmpStatement.size(); i++) {
            currentStatement.push(tmpStatement[i]);
        }

        if (isStatement())
            fout << "\t\tNo error detected in the Statement\n\n";
        else
            return false;

        tmpStatement.clear();
        declareType = "";
    } while (tokenIndex<lexerOutput->size());
    for (int i = 0; i < memoryTable.size(); i++) {
        for (int j = 0; j < memoryTable.at(i).size(); j++)
            fout << memoryTable.at(i).at(j)<<" ";
        fout << "\n";

    }
    return true;
}

bool SyntacticAnalyzer::isStatement(){
    //printLexemeLine();
    statementParser = 0;

    if(isDeclarative())
        return true;
    else if(isAssign())
        return true;
    fout << "Syntax Error\n";
    return false;
}

bool SyntacticAnalyzer::isDeclarative(){
    tmp = currentStatement.top();
    if (symbolTable.isType(tmp.second)) {  
        declareType = tmp.second;
        currentStatement.pop();
        fout << tmp.second + "\t\t<Statement> -> <Declarative> | <Assign>\n";
        tmp = currentStatement.top();
        if (tmp.first == "IDENTIFIER") {
            addToMemoryTable(tmp.second);
            fout << tmp.second + "\t\t<Declarative> -> <Type> <id>\n";
            currentStatement.pop();
            tmp = currentStatement.top();
            if (isDeclarativePrime())
                return true;
        }
    }
    return false;
}

bool SyntacticAnalyzer::isDeclarativePrime(){
    if (tmp.second == ",") {
        fout << tmp.second<<"\n";
        currentStatement.pop();
        tmp = currentStatement.top();
        if (tmp.first == "IDENTIFIER") {
            addToMemoryTable(tmp.second);
            fout << tmp.second << "\n";
            currentStatement.pop();
            tmp = currentStatement.top();
            if (isDeclarativePrime())
                return true;
        }
    }
    else if (tmp.second == "$") {
        fout << "\t\t<Declarative'> -> , <id> <Declarative'> | epsilon\n";
        return true;
    }
    fout << "Delcarative Prime Rule Error\n";
    return false;
}

void SyntacticAnalyzer::addToMemoryTable(const string& s) {

    for(int i = 0; i < memoryTable.size(); i++){

        if(memoryTable.at(i).at(0) == declareType && memoryTable.at(i).at(2) == s){

            fout << "\t\tERROR- Identifier: " + s + " is already in the symbol table\n";
            return;

        }
    }

    vector<string> tmp;
    tmp.emplace_back(declareType);
    tmp.emplace_back(to_string(memoryAddress++));
    tmp.emplace_back(s);
    memoryTable.emplace_back(tmp);

}


bool SyntacticAnalyzer::isAssign(){
    tmp = currentStatement.top();
    fout << tmp.second + " \t\t<Statement> -> <Declarative> | <Assign>\n";
    if (tmp.first == "IDENTIFIER") { // First(A)
        isInMemoryTable(tmp.second);
        currentStatement.pop();
        tmp = currentStatement.top();
        if (tmp.second == "=") {
            fout << tmp.second + "\t\t<Assign> -> <id> = <Expression>\n";
            currentStatement.pop();
            tmp = currentStatement.top();
            if (isExpression())                                   // check expression grammar
                return true;
        }
    }
    fout << "\tAssignation rule error: " << tmp.second << endl;
    return false;
}

bool SyntacticAnalyzer::isExpression(){
    if (isTerm()) {                                               // First(E)
        if (isExpressionPrime()) {
            return true;
        }
    }
    fout << "\tExpression rule error: " << currentStatement.top().second << endl;
    return false;
}

bool SyntacticAnalyzer::isExpressionPrime(){
    tmp = currentStatement.top();
    if (tmp.second == "+" || tmp.second == "-") {
        fout << tmp.second + "\t\t<Expression'> -> + <Terminal> <Expression'> | -<Terminal> < Expression'> | epsilon\n";
        currentStatement.pop();
        tmp = currentStatement.top();
        if (isTerm()) {                                           // First(E')
            if (isExpressionPrime())
                return true;
        }
    }
    else {                                                                    // Follow(E')
        if (tmp.second == "$")
            return true;
        if (tmp.second == ")") {
            return true;
        }
    }
    fout << "\tExpression Prime rule error: " << tmp.second << endl;
    return false;
    
}

bool SyntacticAnalyzer::isTerm(){
    if (isFactor()) {
        if (isTermPrime()) {                                          // First(T)
            return true;
        }
    }
    fout << "\tTerm rule error: " << currentStatement.top().second << endl;
    return false;
}

bool SyntacticAnalyzer::isTermPrime(){
    tmp = currentStatement.top();
    if (tmp.second == "*" || tmp.second == "/") {                              // First(T')
        currentStatement.pop();
        fout << tmp.second + "\t\t<Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | epsilon\n";
        tmp = currentStatement.top();
        if (isFactor()) {
            if (isTermPrime())
                return true;
        }
    }
    else {                                                                    // Follow(T')
        if (tmp.second == "$")
            return true;
        if (tmp.second == ")" || tmp.second == "+" || tmp.second == "-" ) {
            return true;
        }
    }
    fout << "\tTerm Prime rule error: " << tmp.second << endl;
    return false;
    
}

bool SyntacticAnalyzer::isFactor(){
    tmp = currentStatement.top();
    if (tmp.second == "(") {
        currentStatement.pop();

        fout << tmp.second + "\t\t<Expression> -> <Terminal> <Expression'>\n";
        tmp = currentStatement.top();
        if (isExpression()) {
            tmp = currentStatement.top();
            if (tmp.second == ")") {
                currentStatement.pop();
                fout << tmp.second + "\t\t<Factor> -> ( <Expression> ) | <id> | <num>\n";
                tmp = currentStatement.top();
                return true;
            }
        }
    }
    else if (tmp.first == "IDENTIFIER") {
        isInMemoryTable(tmp.second);
        currentStatement.pop();
        fout << tmp.second + "\t\t<Factor> -> ( <Expression> ) | <id> | <num>\n";
        tmp = currentStatement.top();
        return true;
    }
    else if (tmp.first == "INTEGER" || tmp.first == "REAL") {
        currentStatement.pop();
        fout << tmp.second + "\t\t<Factor> -> ( <Expression> ) | <id> | <num>\n";
        tmp = currentStatement.top();

        return true;
    }
    fout << "\tFactor rule error: " << tmp.second << endl;
    return false;
    
}

void SyntacticAnalyzer::isInMemoryTable(const string &str) {

    for(int i = 0; i < memoryTable.size(); i++){

        if(memoryTable.at(i).at(2) == str)
            return;

    }

    fout << "\t\tERROR- Identifier: " + str + " has not been declared yet\n";

}
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
    tmp.emplace_back("Identifier");
    tmp.emplace_back("Memory Location");
    tmp.emplace_back("Type");
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
    vector<pair<string, string>> helper;
    do{
        currentStatement.push(make_pair("$", "$"));
        do {
            tmp = lexerOutput->at(tokenIndex);                                          // get the next lexeme (pair of lexeme and token)
            helper.emplace_back(tmp);                                         // push into the queue
            tokenIndex++;
        } while (tmp.second != ";");
       
        helper.pop_back();
        reverse(helper.begin(), helper.end());
        for (int i = 0; i < helper.size(); i++) {
            currentStatement.push(helper[i]);
        }
        /*while (!currentStatement.empty())
        {
            cout << currentStatement.top().second<<" ";
            currentStatement.pop();
       }*/
        if (isStatement())                                         // begin the analysis
            cout << "No error detected\n\n";
        else
            return false;

        helper.clear();
        declareType = "";
    } while (tokenIndex<lexerOutput->size());
    for (int i = 0; i < memoryTable.size(); i++) {
        for (int j = 0; j < memoryTable.at(i).size(); j++)
            cout << memoryTable.at(i).at(j)<<" ";
        cout << "\n";

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
        cout << tmp.second + "\t\tS -> D\n";
        tmp = currentStatement.top();
        if (tmp.first == "IDENTIFIER") {
            addToMemoryTable(tmp.second);
            cout << tmp.second + "\t\tD -> Type id\n";
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
        cout << tmp.second<<"\n";
        currentStatement.pop();
        tmp = currentStatement.top();
        if (tmp.first == "IDENTIFIER") {
            addToMemoryTable(tmp.second);
            cout << tmp.second << "\n";
            currentStatement.pop();
            tmp = currentStatement.top();
            if (isDeclarativePrime())
                return true;
        }
    }
    else if (tmp.second == "$") {
        cout << "<Declarative'> -> , <id> <Declarative'> | epsilon\n";
        return true;
    }
    cout << "Delcarative Prime Rule Error\n";
    return false;
}

void SyntacticAnalyzer::addToMemoryTable(const string& s) {
    vector<string> tmp;
    tmp.emplace_back(declareType);
    tmp.emplace_back(to_string(memoryAddress++));
    tmp.emplace_back(s);
    memoryTable.emplace_back(tmp);
}


bool SyntacticAnalyzer::isAssign(){
    tmp = currentStatement.top();
    cout << tmp.second + " \t\tS -> A\n";
    if (tmp.first == "IDENTIFIER") { // First(A)
        currentStatement.pop();
        tmp = currentStatement.top();
        if (tmp.second == "=") {
            cout << tmp.second + "\t\tA -> id = E\n";
            currentStatement.pop();
            tmp = currentStatement.top();
            if (isExpression())                                   // check expression grammar
                return true;
        }
    }
    cerr << "\tAssignation rule error: " << tmp.second << endl;
    return false;
}

bool SyntacticAnalyzer::isExpression(){
    if (isTerm()) {                                               // First(E)
        if (isExpressionPrime()) {
            return true;
        }
    }
    cerr << "\tExpression rule error: " << currentStatement.top().second << endl;
    return false;
}

bool SyntacticAnalyzer::isExpressionPrime(){
    tmp = currentStatement.top();
    if (tmp.second == "+" || tmp.second == "-") {
        cout << tmp.second + "\t\tE' -> +TE' | -TE'\n";
        currentStatement.pop();
        tmp = currentStatement.top();
        if (isTerm()) {                                           // First(E')
            if (isExpressionPrime()) {
                return true;
            }
        }
    }
    else {                                                                    // Follow(E')
        if (tmp.second == "$")
            return true;
        if (tmp.second == ")") {
            return true;
        }
    }
    cerr << "\tExpression Prime rule error: " << tmp.second << endl;
    return false;
    
}

bool SyntacticAnalyzer::isTerm(){
    if (isFactor()) {
        if (isTermPrime()) {                                          // First(T)
            return true;
        }
    }
    cerr << "\tTerm rule error: " << currentStatement.top().second << endl;
    return false;
}

bool SyntacticAnalyzer::isTermPrime(){
    tmp = currentStatement.top();
    if (tmp.second == "*" || tmp.second == "/") {                              // First(T')
        currentStatement.pop();
        cout << tmp.second + "\t\tT' -> *FT' | /FT'\n";
        tmp = currentStatement.top();
        if (isFactor()) {
            currentStatement.pop();
            tmp = currentStatement.top();
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
    cerr << "\tTerm Prime rule error: " << tmp.second << endl;
    return false;
    
}

bool SyntacticAnalyzer::isFactor(){
    tmp = currentStatement.top();
    if (tmp.second == "(") {
        currentStatement.pop();
        cout << tmp.second<<"\n";
        tmp = currentStatement.top();
        if (isExpression()) {
            tmp = currentStatement.top();
            if (tmp.second == ")") {
                cout << tmp.second + "\t\tF -> (E)\n";
                return true;
            }
        }
    }
    else if (tmp.first == "IDENTIFIER") {
        currentStatement.pop();
        cout << tmp.second + "\t\tF -> id\n";
        tmp = currentStatement.top();
        return true;
    }
    else if (tmp.first == "INTEGER") {
        currentStatement.pop();
        cout << tmp.second + "\t\tF -> num\n";
        tmp = currentStatement.top();

        return true;
    }
    cerr << "\tFactor rule error: " << tmp.second << endl;
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

//bool SyntacticAnalyzer::isNumber() {
//    return currentStatement.at(statementParser).first == "INTEGER" ||
//           currentStatement.at(statementParser).first == "REAL";
//}
//
//void SyntacticAnalyzer::printLexemeLine() {
//    for (int i = 0; i < currentStatement.size(); i++) {
//        fout << currentStatement.at(i).second << " ";
//    }
//    fout << "\n";
//}
//
//void SyntacticAnalyzer::printStatementRules() {
//    while (productionOutputs.size() != 0)
//    {
//        fout << productionOutputs.front() << endl;
//        productionOutputs.pop();
//    }
//    fout << "\n";
//}
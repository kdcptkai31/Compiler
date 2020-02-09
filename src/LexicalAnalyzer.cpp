#include "headers/LexicalAnalyzer.h"

/**
 * Constructor
 */
LexicalAnalyzer::LexicalAnalyzer() {


}

/**
 * Controls the majority of the Lexical Analyzer's logic, will finish with a map of Lexemes and Tokens
 * @param fileIn
 */
void LexicalAnalyzer::runLexer(ifstream &fileIn) {

    //Generates meaningful units to run through the lexer, removes comment blocks here. Units are in meaningfulUnits.
    generateMeaningfulUnits(fileIn);

    //Labels each meaningful unit with its associated Token, then inserts into the lexerOutput map for production.
    for(auto currentUnit : meaningfulUnits){

        int currentUnitType = getUnitType(currentUnit);

        //Handles the meaningful unit based on what it is or could be
        switch(currentUnitType){

            //Separator
            case 0:
                lexerOutput.insert({"SEPARATOR", currentUnit});
                break;

            //Operator
            case 1:
                lexerOutput.insert({"OPERATOR", currentUnit});
                break;

            //Something else
            case 2:
                int acceptedStateType;//0 = identifier, 1 = Real Float, 2 = Integer
                if(DFA(currentUnit, acceptedStateType)){

                    switch(acceptedStateType){

                        case 0:
                            if(symbolTable.isKeyword(currentUnit))
                                lexerOutput.insert({"KEYWORD", currentUnit});
                            else
                                lexerOutput.insert({"IDENTIFIER", currentUnit});
                            break;

                        case 1:
                            lexerOutput.insert({"REAL", currentUnit});
                            break;

                        case 2:
                            lexerOutput.insert({"INTEGER", currentUnit});
                            break;

                    }

                } else{

                    //Error lexeme (idk how we're suppossed to output this)

                }

            break;

        }

    }

}

/**
 * Parses the file and separates the characters into "meaningful units," or by whitespace, separators, operators.
 * Additionally, comment blocks and additional whitespace will be removed at this stage.
 * @param fileIn
 */
void LexicalAnalyzer::generateMeaningfulUnits(ifstream &fileIn) {

    char currentChar;
    string currentUnit;
    while(fileIn.get(currentChar)){

        if(isMeaningfulUnitSeparator(currentChar) && currentChar != '.'){//Char is non-comment, non-whitespace separator

            //Appends the current string unit to the data structure, followed by the current character unit
            if(currentUnit.size() > 0)
                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);

            meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));
            currentUnit.clear();

        }else if(currentChar == '.') {                                  //Char is a '.', so check if it's part of a Real

            if(currentUnit.size() > 0) {
                if (stringIsNum(currentUnit))
                    currentUnit.append(string(1, currentChar));
                else{

                    meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                    meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));
                    currentUnit.clear();

                }

            }else
                meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));

        }else if(isWhiteSpace(currentChar)){                //Char is whitespace, remove

                if(currentUnit.size() > 0) {

                    meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                    currentUnit.clear();

                }

        }else if(currentChar == '!'){                       //Char is comment, remove the block

            if(currentUnit.size() > 0){

                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                currentUnit.clear();

            }

            string tmp;
            getline(fileIn,tmp);
            tmp.clear();

        }else                                               //Char will be appended to currentUnit
                currentUnit.append(string(1, currentChar));

    }

    fileIn.close();

}

/**
 * Deterministic Finite State Automata for Identifiers, Real Floats, and Integers.
 * @param s
 * @return True if
 */
bool LexicalAnalyzer::DFA(string meaningfulUnit, int& acceptedStateType) {



}

/**
 * Helper function for generateMeaningfulUnits
 * @param c
 * @return True if the character is a separator or operator, but not whitespace. False if it is whitespace or an alpha.
 */
bool LexicalAnalyzer::isMeaningfulUnitSeparator(char c) {

    string s(1, c);
    return (symbolTable.isSeparator(s) || symbolTable.isOperator(s)) && !isWhiteSpace(c);

}

/**
 * Helper function for generateMeaningfulUnits
 * @param c
 * @return True if the character is considered whitespace. False if not.
 */
bool LexicalAnalyzer::isWhiteSpace(char c) {return c == ' ' || c == '\n' || c == '\t';}

/**
 * Helper function for runLexer which determines which switch state should be applied to the meaningful unit.
 * @param s
 * @return Value of 0 means it is a separator, value of 1 means it is an operator, and a value of 2 means it is either
 *         an identifier, a keyword, an integer, a real float number, or a possible invalid identifier.
 */
int LexicalAnalyzer::getUnitType(string s) {

    if(symbolTable.isSeparator(s))
        return 0;

    if(symbolTable.isOperator(s))
        return 1;

    return 2;
}

/**
 * Helper function for runLexer which determines if the given string is an integer.
 * @param s
 * @return True if the string is an integer. False if not.
 */
bool LexicalAnalyzer::stringIsNum(string s) {

    char* p;
    strtol(s.c_str(), &p, 10);
    return *p == 0;

}

/**
 * Prints the final output of the Lexical Analyzer, in a list of "Tokens    Lexemes", to a file.
 */
void LexicalAnalyzer::printOutputToFile(ofstream& fileOut) {

    fileOut << setw(14) << left << "TOKENS" << "LEXEMES\n\n";

    for(auto currentLexemeTokenPair: lexerOutput)
        fileOut << setw(10) << left << currentLexemeTokenPair.first << "=   " << currentLexemeTokenPair.second << endl;

    fileOut.close();

}
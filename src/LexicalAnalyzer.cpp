#include "headers/LexicalAnalyzer.h"

/**
 * Default Constructor
 */
LexicalAnalyzer::LexicalAnalyzer(){}

/**
 * Controls the majority of the Lexical Analyzer's logic, will finish with a map of Lexemes and Tokens. It first breaks
 * up the input file into "meaningfulUnits" which may or may not be valid substrings. These units are used throughout
 * the lexical analyzer as potential valid substrings, and once verified and labeled, they will be inserted into the
 * lexerOutput data structure for shipment to the next stage of the compiler.
 * @param fileIn
 */
void LexicalAnalyzer::run(ifstream &fileIn) {

    //Generates meaningful units to run through the lexer, removes comment blocks and whitespace here.
    generateMeaningfulUnits(fileIn);

    //Labels each meaningful unit with its associated Token, then inserts into the lexerOutput map for production.
    for(auto currentUnit : meaningfulUnits){

        int currentUnitType = getUnitType(currentUnit);

        //Handles the meaningful unit based on what it is or could be
        switch(currentUnitType){

            case 0: //Separator Case
                lexerOutput.emplace_back("SEPARATOR", currentUnit);
                break;

            case 1: //Operator Case
                lexerOutput.emplace_back("OPERATOR", currentUnit);
                break;

            case 2: //DFA will be needed to determine the additional cases
                int acceptedStateType;//0 = identifier, 1 = Integer, 2 = Real Float
                if(DFA(currentUnit, acceptedStateType)){

                    switch(acceptedStateType){

                        case 0: //Identifier or Keyword Case
                            if(symbolTable.isKeyword(currentUnit))
                                lexerOutput.emplace_back("KEYWORD", currentUnit);
                            else
                                lexerOutput.emplace_back("IDENTIFIER", currentUnit);
                            break;

                        case 1: //Integer Case
                            lexerOutput.emplace_back("INTEGER", currentUnit);
                            break;

                        case 2: //Real Float Case
                            lexerOutput.emplace_back("REAL", currentUnit);
                            break;

                    }

                } else //Error Case
                    lexerOutput.emplace_back("ERROR", currentUnit);

            break;

        }

    }

}

/**
 * Parses the file and separates the characters into "meaningful units," by whitespace, separators, and operators.
 * Additionally, comment blocks and additional whitespace will be removed at this stage.
 * @param fileIn
 */
void LexicalAnalyzer::generateMeaningfulUnits(ifstream &fileIn) {

    char currentChar;
    string currentUnit;
    while(fileIn.get(currentChar)){

        //Char is non-comment, non-whitespace separator
        if(isMeaningfulUnitSeparator(currentChar, fileIn, currentUnit) && currentChar != '.' && currentChar != '-' && currentChar != '+'){

            //Appends the current string unit to the data structure, followed by the current character unit
            if(!currentUnit.empty())
                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);

            meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));
            currentUnit.clear();

        }else if(currentChar == '.') {//Char is a '.', so check if it's part of a Real Float

            if (!currentUnit.empty()) {

                if (stringIsNum(currentUnit))
                    currentUnit.append(string(1, currentChar));
                else {

                    meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                    meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));
                    currentUnit.clear();

                }

            } else {

                if (isdigit(fileIn.peek()))
                    currentUnit.append(string(1, currentChar));
                else
                    meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));

            }

        }else if(currentChar == '-' || currentChar == '+'){

            if(currentUnit.length() > 0){

                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                currentUnit.clear();
                meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));


            }else if(isdigit(fileIn.peek()))
                currentUnit.append(string(1, currentChar));
            else
                meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));

        }else if(iswspace(currentChar)){//Char is whitespace, remove

                if(!currentUnit.empty()) {

                    meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                    currentUnit.clear();

                }

        }else if(currentChar == '!'){//Char is comment, remove the block

            if(!currentUnit.empty()){

                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);
                currentUnit.clear();

            }

            string tmp;
            getline(fileIn,tmp);
            tmp.clear();

        }else//Char will be appended to currentUnit
                currentUnit.append(string(1, currentChar));

    }

    fileIn.close();

}

/**
 * Deterministic Finite State Automata for Identifiers, Real Floats, and Integers. For each character in the unit, and
 * based on the currently selected character's type, the state table will be traversed for each new character until it
 * reaches its end. Then, depending on the end state, the meaningful unit will be assigned its type and whether or not
 * it is a valid unit.
 * @param s
 * @return True if the meaningful unit is a valid Identifier, Real Float, or Integer. False is it is an error
 */
bool LexicalAnalyzer::DFA(const string& meaningfulUnit, int& acceptedStateType) {

    bool accepted = false;
    int DFATable[8][7] =
            {
                    {0, letter, digit, dollarSign, decimalPoint, signs, unknown},
                    {1,     2,     3,       7,           7,         3,      7},
                    {2,     2,     2,       2,           7,         7,      7},
                    {3,     7,     4,       7,           5,         7,      7},
                    {4,     7,     4,       7,           5,         7,      7},
                    {5,     7,     6,       7,           7,         7,      7},
                    {6,     7,     6,       7,           7,         7,      7},
                    {7,     7,     7,       7,           7,         7,      7}
            };

    int curState = 1;

    //colNum returns the enum type of the character passed to it to receive its column number
    for (char c : meaningfulUnit)
        curState = DFATable[curState][colNum(c)];

    if(curState==2){ //Valid Identifier
        acceptedStateType = 0;
        accepted = true;
    }
    else if (curState == 3 || curState == 4){ //Valid Integer
        acceptedStateType = 1;
        accepted = true;
    }
    else if (curState == 6){ //Valid Real Float
        acceptedStateType = 2;
        accepted = true;
    }

    return accepted;
    
}

/**
 * Returns the enum type of the character parameter.
 * @param ch
 * @return character type
 */
int LexicalAnalyzer::colNum(char ch) {

    if (isdigit(ch))
        return digit;
    else if (isalpha(ch))
        return letter;
    else if (ch == '$')
        return dollarSign;
    else if (ch == '.')
        return decimalPoint;
    else if (ch == '+' || ch == '-')
        return signs;
    else
        return unknown;
    
}

/**
 * Helper function for generateMeaningfulUnits.
 * @param c
 * @return True if the character is a separator or operator, but not whitespace. False if it is whitespace or an alpha.
 */
bool LexicalAnalyzer::isMeaningfulUnitSeparator(char c, ifstream &input, string unit) {
    if ((unit == "<") && (c == '>' || c == '='))
        return false;
    else if (unit == ">" && c=='=')
        return false;
    else if (unit == "=" && c == '=')
        return false;

    string s(1, c);
    if (s == "<" && (input.peek() == '=' || input.peek() == '>'))
        return false;
    else if (s == ">" && input.peek() == '=')
        return false;
    else if (s == "=" && input.peek() == '=')
        return false;
    return (symbolTable.isSeparator(s) || symbolTable.isOperator(s)) && !iswspace(c);
}

/**
 * Helper function for run which determines which switch state should be applied to the meaningful unit.
 * @param s
 * @return Value of 0 means it is a separator, value of 1 means it is an operator, and a value of 2 means it is either
 *         an identifier, a keyword, an integer, a real float number, or a possible invalid identifier.
 */
int LexicalAnalyzer::getUnitType(const string& s) {

    if(symbolTable.isSeparator(s))
        return 0;

    if(symbolTable.isOperator(s))
        return 1;

    return 2;
}

/**
 * Helper function for run which determines if the given string is an integer.
 * @param s
 * @return True if the string is an integer. False if not.
 */
bool LexicalAnalyzer::stringIsNum(const string& s) {

    char* parser;
    strtol(s.c_str(), &parser, 10);
    return *parser == 0;

}

/**
 * Prints the final output of the Lexical Analyzer, in a list of "Tokens    Lexemes", to a file.
 */
void LexicalAnalyzer::printOutputToFile(ofstream& fileOut) {

    fileOut << setw(14) << left << "TOKENS" << "LEXEMES\n\n";

    for(const auto& currentLexemeTokenPair: lexerOutput)
        fileOut << setw(10) << left << currentLexemeTokenPair.first << "=   " << currentLexemeTokenPair.second << endl;

    fileOut.close();

}
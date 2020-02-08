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

        if(isMeaningfulUnitSeparator(currentChar)){         //Char is non-comment, non-whitespace separator

            //Appends the current string unit to the data structure, followed by the current character unit
            if(currentUnit.size() > 0)
                meaningfulUnits.insert(meaningfulUnits.end(), currentUnit);

            meaningfulUnits.insert(meaningfulUnits.end(), string(1, currentChar));
            currentUnit.clear();

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
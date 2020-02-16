#include "headers/LexicalAnalyzer.h"

/**
 * Controls the flow of the current state of this project. Currently, only the lexical analyzer is complete so an input
 * file will be read into the lexer, which will output the organized data to an output file.
 * @return
 */
int main() {

    ifstream fileIn;
    LexicalAnalyzer lexer;

    fileIn.open("fileName.txt", ifstream::in);

    if(fileIn.is_open()) {

        lexer.runLexer(fileIn);
        ofstream fileOut;
        fileOut.open("output.txt", ifstream::out);
        lexer.printOutputToFile(fileOut);

    }else {

        cout << "File does not exist or cannot be opened.\n";
        fileIn.close();

    }

    return 0;

}

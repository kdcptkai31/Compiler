#include "headers/LexicalAnalyzer.h"

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

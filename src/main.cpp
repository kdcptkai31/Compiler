#include "headers/LexicalAnalyzer.h"

int main() {

    ifstream fileIn;
    string fileName = "fileName.txt";
    LexicalAnalyzer lexer;

    fileIn.open(fileName);

    if(fileIn.is_open())
        lexer.runLexer(fileIn);
    else
        cout << "File does not exist or cannot be opened.\n";

    fileIn.close();

    return 0;
}

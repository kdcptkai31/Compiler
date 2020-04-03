#include "headers/LexicalAnalyzer.h"
#include "headers/SyntacticAnalyzer.h"

/**
 * Controls the flow of the current state of this project. Currently, only the lexical analyzer is complete so an input
 * file will be read into the lexer, which will output the organized data to an output file.
 * @return
 */
int main() {

    ifstream fileIn;
    fileIn.open("fileName.txt", ifstream::in);

    //If source file does not exist, exit the compiler.
    if(!fileIn.is_open()){

        cout << "File does not exist or cannot be opened.\n";
        fileIn.close();
        return 0;

    }

    LexicalAnalyzer lexer;
    lexer.run(fileIn);

    SyntacticAnalyzer syntacticAnalyzer;
    syntacticAnalyzer.run(lexer);

//    ofstream fileOut;
//    fileOut.open("output.txt", ifstream::out);
//    lexer.printOutputToFile(fileOut);

    return 0;

}

#include "LexicalAnalyzer.h"
#include <fstream>

int main() {

    fstream fileIn;
    fileIn.open("fileName.txt");
    char currentCharacter;

    if(fileIn.is_open()){

        while(fileIn >> currentCharacter){

            //Call the DFSM on the character here

        }


    } else
        cout << "File does not exist or cannot be opened.\n";


    return 0;
}

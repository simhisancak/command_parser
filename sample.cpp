#include "command_parser.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include "solve.hpp"

using namespace std;


int main(){
    string FileText;
    ifstream FileStream("in.txt");
    int LineIndex = 0;
    while (getline (FileStream, FileText)) {
        if(!CommandParser::ParseCommand(FileText))
            cout << "Error Line: " << LineIndex << "\n\n";
        LineIndex++;
    }
    return 0;
}
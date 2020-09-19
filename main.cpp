#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

void correctOverflow(char &c, char op) {
    /* Correct value of the character */
    if (op == '+') {
        if ((int)c < 255) c++;
        else c = 0;
    } else {
        if ((int)c != 0) c--;
        else c = (char)255;
    }
}

void analyseLoopAtStart(int &ptrCode, std::string code, char caseTab, std::vector<std::pair<int, int>> &arrayLoop) {
    int startLoop = ptrCode;
    int endLoop = 0;

    if (arrayLoop.empty() || arrayLoop[arrayLoop.size()-1].first != ptrCode) {
        int closeIgnore = 0;
        int end;
        for (int i = ptrCode+1; i < code.length(); i++) {
            if (code[i] == '[') closeIgnore++;
            else if (code[i] == ']' && closeIgnore > 0) closeIgnore--;
            else if (code[i] == ']' && closeIgnore == 0) { end = i; break;}
        }
        arrayLoop.emplace_back(ptrCode, end);
        endLoop = end;
    } else {
        endLoop = arrayLoop[arrayLoop.size()-1].second;
    }

    if ((int)caseTab != 0)  return;
    else { ptrCode = endLoop; arrayLoop.pop_back(); return; }
}

void readInput(char &tab_ptr) {
    char input;
    std::cout << "Input : ";
    std::cin.get(input); tab_ptr = input;
    if (input == '\n') {tab_ptr = '\0'; return;} 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input to prevent other inputs
}

void parseCode(const std::string &code, char * tab, int &ptr, int &ptrCode) {
    std::vector<std::pair<int, int>> arrayLoop;
    
}

void brainFuck(std::string &code, char op) {
    int ptr = 0, ptrCode = 0; 
    char tab[30000] = { };
    std::vector<std::pair<int, int>> arrayLoop;

    if (op == 'i') { // If the mode selected is interactive
        while (std::cout << "[" << ptr << "]:" << tab[ptr] << " >> ", std::getline(std::cin, code), code != "exit") {

            if (std::count(code.begin(), code.end(), '[') != std::count(code.begin(), code.end(), ']')) // Check if there are the same number of '[' than ']'
                { std::cout << "Error matching brackets !\n"; continue; }

            while (ptrCode < code.length()) {
                switch (code[ptrCode]) {
                    case '>': if (ptr < 29999) ptr++; break;
                    case '<': if (ptr > 0) ptr--; break;
                    case '+': correctOverflow(tab[ptr], '+'); break;
                    case '-': correctOverflow(tab[ptr], '-'); break;
                    case '.': std::cout << tab[ptr]; break;
                    case ',': readInput(tab[ptr]); break;
                    case '[': analyseLoopAtStart(ptrCode, code, tab[ptr], arrayLoop); break;
                    case ']': ptrCode = arrayLoop[arrayLoop.size()-1].first-1; break;
                }
                ptrCode++;
            }
            ptrCode = 0;
        }
    } else {
        if (std::count(code.begin(), code.end(), '[') != std::count(code.begin(), code.end(), ']')) // Check if there are the same number of '[' than ']'
            { std::cout << "Error matching brackets !\n"; return; }

        while (ptrCode < code.length()) {
            
            switch (code[ptrCode]) {
                case '>': if (ptr < 29999) ptr++; break;
                case '<': if (ptr > 0) ptr--; break;
                case '+': correctOverflow(tab[ptr], '+'); break;
                case '-': correctOverflow(tab[ptr], '-'); break;
                case '.': std::cout << tab[ptr] << std::endl; break;
                case ',': readInput(tab[ptr]); break;
                case '[': analyseLoopAtStart(ptrCode, code, tab[ptr], arrayLoop); break;
                case ']': ptrCode = arrayLoop[arrayLoop.size()-1].first-1; break;
            }
            ptrCode++;
        }
    }
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-f") {
            std::ifstream codeFile (argv[i+1]); // Open file
            if (codeFile.is_open()) { // The file is open without errors
                std::string line, code = "";
                while (std::getline(codeFile, line)) // Read all lines
                {
                    line = line.substr(0, line.find('#')); // Ignore characters after '#' (Comments)
                    code += line;
                }                
                brainFuck(code, 'f');
            } else {
                std::cerr << "Open file failed !" << std::endl << argv[i+1] << std::endl;
            }
            break;
        } else if (arg == "-c") {
            std::string code = argv[i+1];
            brainFuck(code, 'c');
            break;
        } else if (arg == "-i") {
            std::string emptyCode;
            brainFuck(emptyCode, 'i');
            break;
        } else if (arg == "-h") {
            std::cout << "BrainFuck Interpreter" << std::endl
                      << "Options : " << std::endl
                      << "  -f [file] : run BrainFuck code of the file given"  << std::endl
                      << "  -c [code] : run BrainFuck code directly given in parameter" << std::endl
                      << "  -i : run the live interpreter" << std::endl;

        } else {
            std::cout << "Option not found, use -h to see the valids options" << std::endl;
            return 1; // If the options is not available end the program
        }
    }
    return 0;
}

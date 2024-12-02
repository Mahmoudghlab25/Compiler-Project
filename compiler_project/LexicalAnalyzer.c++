#include <iostream>
#include "common.h"
using namespace std;

class LexicalAnalyzer {
private:
    // Attributes (e.g., variables to store source code, tokens, etc.)
    vector<string> codelines;

public:
    // Constructor(s)
    LexicalAnalyzer(vector<string> codelines) {
        this->codelines = codelines;
    }
    // Methods
    vector<string> analyze() {

    }
    void displayTokens(vector<string> tokens) {
        for (auto token : tokens) {
            std::cout << token << std::endl;
        }
    }

};
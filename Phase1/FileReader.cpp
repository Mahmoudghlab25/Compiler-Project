#include <iostream>
#include "common.h"

using namespace std;

class FileReader {
private:
    string filePath;

public:
    // Constructor
    explicit FileReader(string filePath) {
        this->filePath = std::move(filePath);
    }

    // Method to read the file line by line
    vector<string> readLines() {
        vector<string> lines;
        ifstream file(this->filePath);

        if (!file.is_open()) {
            std::cerr << "Failed to open the file with path: " << this->filePath << endl;
        }

        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }

    void printLines(vector<string>& codeLines) {
        for (const auto& line : codeLines) {
            cout << line << endl;
        }
    }
};
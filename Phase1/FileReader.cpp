#ifndef FILEREADER_CPP
#define FILEREADER_CPP

#include <iostream>
#include "common.h"

using namespace std;

class FileReader {
private:

public:
    vector<string> readLines(const string& fileName) {
        vector<string> lines;
        ifstream file(fileName);

        while (true) {
            string line;
            if (!getline(file, line)) {
                lines.push_back(line);
                break;
            }
            lines.push_back(line);
        }
        return lines;
    }
    // Method to read the file line by line
//    vector<string> readLines(string filePath) {
//        vector<string> lines;
//        ifstream file(filePath);
//
//        if (!file.is_open()) {
//            std::cerr << "Failed to open the file with path: " << filePath << endl;
//        }
//
//        string line;
//        while (getline(file, line)) {
//            lines.push_back(line);
//        }
//
//        file.close();
//        return lines;
//    }

    void printLines(vector<string> &codeLines) {
        for (const auto &line: codeLines) {
            cout << line << endl;
        }
    }
};

#endif
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

class FileReader {
private:
    string filePath;

public:
    // Constructor
    FileReader(string filePath) : filePath(filePath) {}

    // Method to read the file line by line
    vector<string> readLines(){
        vector<string> lines;
        ifstream file(this->filePath);

        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + this->filePath);
        }

        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }
    void printLines(vector<string> codeLines){
        for (auto line : codeLines) {
            std::cout << line << std::endl;
        }
    }
};

int main(){
    FileReader fileReader = FileReader("input.txt");
    vector<string> codeLines = fileReader.readLines();
    fileReader.printLines(codeLines);
    return 0;
}
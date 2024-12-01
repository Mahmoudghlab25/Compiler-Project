#include <iostream>
#include <fstream>
#include <vector>

class FileWriter {
private:
    std::string filePath;

public:
    explicit FileWriter(const std::string &fileName) {
        this->filePath = R"(C:\Users\Mahmo\OneDrive - Alexandria University\Documents\GitHub\Compiler-Project\Phase1\Output\)" + fileName + ".txt";
    }

    void writeLines(const std::vector<std::string> &tokens) {
        std::ofstream outputFile(this->filePath);

        if (!outputFile.is_open()) {
            std::cerr << "Error: Unable to open file " << this->filePath << std::endl;
            return;
        }

        for (const auto &token: tokens) {
            outputFile << token << std::endl;
        }

        outputFile.close();
    }
};

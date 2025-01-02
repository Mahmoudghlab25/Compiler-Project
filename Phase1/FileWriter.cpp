#ifndef FILEWRITER_CPP
#define FILEWRITER_CPP

#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

class FileWriter {
private:
    std::string filePath;

public:
    explicit FileWriter() {}

    void writeLines(const std::vector<std::string> &tokens, const std::string &fileName) {
        this->filePath = fileName;
        // Explicitly clear the file content by opening it in truncation mode
        std::ofstream outputFile(this->filePath, std::ios::trunc);

        if (!outputFile.is_open()) {
            std::cerr << "Error: Unable to open file " << this->filePath << std::endl;
            return;
        }

        for (int i = 0; i < tokens.size(); i += 2) {
            outputFile << tokens[i] << " : " << tokens[i + 1] << std::endl;
        }

        outputFile.close(); // Optional since the destructor will close it
        std::cout << "Tokens written to " << fileName << "\n\n";
    }

    void writeGrammarToMarkdown(
            std::unordered_map<std::string, std::vector<std::vector<std::string>>> grammar,
            const std::string filename) {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);

        if (!file) {
            std::cerr << "Failed to open or create file: " << filename << "\n";
            return;
        }

        // Write table header for grammar
        file << "\n## Grammar Table\n";
        file << "| Non-Terminal | Production Rules                                      |\n";
        file << "|--------------|-------------------------------------------------------|\n";

        // Write grammar rules
        for (const auto &entry: grammar) {
            file << "| " << entry.first << " | ";
            for (const auto &rule: entry.second) {
                file << "[ ";
                for (const auto &token: rule) {
                    file << token << " ";
                }
                file << "] ";
            }
            file << "|\n";
        }

        std::cout << "Grammar written to " << filename << "\n\n";
    }

    // Function to write First or Follow sets to a Markdown file
    void writeSetToMarkdown(const std::map<std::string, std::set<std::string>> setMap,
                            const std::string filename,
                            const std::string setName) {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);

        if (!file) {
            std::cerr << "Failed to open or create file: " << filename << "\n";
            return;
        }

        // Write table header for First/Follow sets
        file << "\n## " << setName << " Sets\n";
        file << "| Non-Terminal | " << setName << " Values                  |\n";
        file << "|--------------|-------------------------------------------|\n";

        // Write sets
        for (const auto &entry: setMap) {
            file << "| " << entry.first << " | ";
            for (const auto &value: entry.second) {
                file << value << "  ";
            }
            file << "|\n";
        }

        file.close();
        std::cout << setName << " sets written to " << filename << "\n\n";
    }

    // Function to write returnedStack (queue) to a Markdown file
    void writeActionsToMarkdown(const std::queue<std::string> &stack,
                                const std::vector<std::string> &tokens,
                                const std::vector<std::string> &productions,
                                const std::string &filename) {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);

        if (!file) {
            std::cerr << "Failed to open or create file: " << filename << "\n";
            return;
        }

        // Write Markdown table header
        file << "# Parser Table\n\n";
        file << "## Stack, Tokens, and Productions\n";
        file << "| Stack Element                | Token               | Production          |\n";
        file << "|------------------------------|---------------------|---------------------|\n";

        // Use iterators for tokens and productions
        std::queue<std::string> tempStack = stack;
        auto tokenIt = tokens.begin();
        auto productionIt = productions.begin();

        // Iterate through all parameters and write them side by side
        while (!tempStack.empty() || tokenIt != tokens.end() || productionIt != productions.end()) {
            std::string stackElement = tempStack.empty() ? "" : tempStack.front();
            std::string token = (tokenIt != tokens.end()) ? *tokenIt : "";
            std::string production = (productionIt != productions.end()) ? *productionIt : "";

            file << "| " << stackElement
                 << " | " << token
                 << " | " << production
                 << " |\n";

            if (!tempStack.empty()) tempStack.pop();
            if (tokenIt != tokens.end()) ++tokenIt;
            if (productionIt != productions.end()) ++productionIt;
        }

        file.close();
    }


    std::string join(const std::vector<std::string>& vec, const std::string& delimiter = ", ") {
        std::ostringstream ss;
        for (size_t i = 0; i < vec.size(); ++i) {
            ss << vec[i];
            if (i < vec.size() - 1) {
                ss << delimiter;
            }
        }
        return ss.str();
    }

    void writePredictiveTableToMarkdown(
            const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& predictive_table,
            const std::string& file_path)
    {
        // Collect all unique terminals to form the table columns
        std::set<std::string> terminals;
        for (const auto& row : predictive_table) {
            for (const auto& terminal : row.second) {
                terminals.insert(terminal.first);
            }
        }

        std::ofstream file(file_path, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + file_path);
        }

        // Write the header row
        file << "| Non-Terminal ";
        for (const auto& terminal : terminals) {
            file << "| " << terminal << " ";
        }
        file << "|\n";

        // Write the header separator
        file << "|--------------";
        for (size_t i = 0; i < terminals.size(); ++i) {
            file << "|----------";
        }
        file << "|\n";

        // Write each row of the predictive table
        for (const auto& row : predictive_table) {
            const std::string& non_terminal = row.first;
            file << "| " << non_terminal << " ";

            for (const auto& terminal : terminals) {
                if (row.second.find(terminal) != row.second.end()) {
                    file << "| " << join(row.second.at(terminal)) << " ";
                } else {
                    file << "|   "; // Empty cell
                }
            }

            file << "|\n";
        }

        file.close();
    }
};

#endif
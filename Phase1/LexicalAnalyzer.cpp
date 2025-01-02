#ifndef LEXICALANALYZAR_CPP
#define LEXICALANALYZAR_CPP
#include <iostream>
#include <utility>
#include "State.h"
#include "MinimizeDFA/MinimizeDFA.h"
#include "FileReader.cpp"
#include "FileWriter.cpp"
#include "LexicalRulesHandler.h"

class LexicalAnalyzer {
private:
    std::vector<std::string> codelines;
    std::set<State *> states;
    string next_token;
    int currentLineIndex;
public:
    explicit LexicalAnalyzer(std::vector<std::string> codelines, std::set<State *> states) {
        this->codelines = std::move(codelines);
        this->states = std::move(states);
        next_token = "";
        currentLineIndex = 0;
    }

    void remove_previous_words(string token){
        // Remove all words before the last token in the current line
        if (currentLineIndex < codelines.size()) {
            size_t lastTokenPos = this->codelines[currentLineIndex].find(token) + token.size();

            if (lastTokenPos != std::string::npos) {
                this->codelines[currentLineIndex] = this->codelines[currentLineIndex].substr(lastTokenPos); // Keep only the last token and beyond
            }
        }
    }

    bool analyze() {
        State *currentState = *(this->states.begin());
        std::string token;
        std::vector<std::string> tokens;
        std::string currentAccToken;
        int index;
        int errIndex;
        int lastAccInd = 0;
        if (currentState->is_dead_state()) {
            return true; // Return early if the starting state is dead
        }

        for (int i=currentLineIndex ; i<codelines.size() ; i++) {
            string &codeline = codelines[i];
            index = 0;
            errIndex = 0;
            token.clear();
            currentAccToken.clear();
            currentState = *(this->states.begin());
            while (index < codeline.size()) {
                if (currentState->is_dead_state()) {
                    if (!token.empty()) {
//                        cout << "index: " << index << ", Token: " << token << endl;
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                        next_token = token;
                        remove_previous_words(currentAccToken);
                        return true;
                    } else {
//                        cout << "index: " << index << ", " << "ERRRRRRRRR" << endl;
                        errIndex++;
                        index = errIndex;
                        tokens.emplace_back("Error at index " + std::to_string(index));
                    }
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                    continue;
                }
                if (currentState->is_accepting_state()) {
                    token = currentState->get_token_type();
                    lastAccInd = index - 1;
                    errIndex = index;
                }
                if (codeline[index] == ' ') {
                    if (!token.empty()) {
//                        cout << "index: " << index << ", Token: " << token << endl;
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                        next_token = token;
                        remove_previous_words(currentAccToken);
                        return true;
                    }
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                    index++;
                    continue;
                }
                if (currentState->get_transitions().count(codeline[index]) > 0) {
                    currentState = *(currentState->get_transitions().at(codeline[index]).begin());
                    if (currentState->is_dead_state()) {
                        index = lastAccInd;
                    } else {
                        currentAccToken += codeline[index];
                    }
                } else {
                    if (!token.empty()) {
                        tokens.push_back(currentAccToken);
                        tokens.push_back(token);
                        next_token = token;
                        remove_previous_words(currentAccToken);
                        return true;
                    }
                    cerr << "Error: Character " << codeline[index] << " isn't in language." << endl;
                    string err;
                    err += codeline[index];
                    tokens.push_back(err);
                    tokens.emplace_back("Error");
                    currentState = *(this->states.begin());
                    token.clear();
                    currentAccToken.clear();
                }
                index++;
            }
            if (currentState->is_accepting_state()) {
                tokens.push_back(currentAccToken);
                tokens.push_back(currentState->get_token_type());
                next_token = currentState->get_token_type();
                remove_previous_words(currentAccToken);
                return true;
            }
            currentLineIndex++;
        }
        return false;
    }

    string get_token(){
        while (next_token.empty()) {
            // Wait for the lexical analyzer to provide the next token
            // This could be replaced with a condition variable for thread-safe waiting
            cout << "Waiting for next token... Provide input:" << endl;
            string input;
            cin >> input;
            if(!analyze()) break;
        }
        if (!next_token.empty()) {
            string token = next_token;
            next_token.clear();  // Clear the token to indicate it's been consumed
            return token;
        }
        cout<<"Ending Analyze..........\n\n";
        return "$";
    }


};

#endif
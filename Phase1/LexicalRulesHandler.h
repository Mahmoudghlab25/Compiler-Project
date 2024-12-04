#pragma once
#pragma once
#include "LexicalRulesCommon.h"
#include "LexicalRuleParser.h"
#include "NFA.h"


class LexicalRulesHandler {
private:
    std::unordered_set<std::string> allNames;
    std::unordered_set<std::string> expNames;
    std::unordered_map<std::string, std::string> statements;
    std::unordered_set<std::string> keyWords;
    std::unordered_set<std::string> punctuation;
    std::map<std::string, NFA*> nfaMap;
    std::set<char> alphabet;

    // Adds the statement to its corresponding map
    // If lhs already exists, new rhs is joined with the existing rhs by '|'
    void addStatement(
            std::unordered_map<std::string, std::string>& statementHolder,
            const std::string& lhs,
            const std::string& rhs
    );

    std::vector<Token> parseRHS(const std::string& rhs);
    NFA* generateNFA(const std::string& curr);
    NFA* generateNFAForPunctuation(const std::string& punc);
    void addToAlphabet(char c);
    void addToAlphabet(const std::string& val);


public:
    /*
        Accepts rules read from input file (which are expected to be trimmed).
        Extracts statements (expressions/definitions)
        for each defined variable, then pushes
        the rhs to the corresponding map.
        If lhs already exists, the existing rhs is joined with the new rhs
        with a union '|' operation.
        Defined names are stored in a set.
    */
    void extractStatements(const std::vector<std::string>& rules);
    void extractKeywords(const std::vector<std::string>& rules);
    void extractPunctuation(const std::vector<std::string>& rules);
    std::vector<std::string> readRules(const char* fileName);
    NFA* generateNFAs();
    std::set<char> getAlphabet();
};
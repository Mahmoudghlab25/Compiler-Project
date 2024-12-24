#ifndef PARSERRULESLEXER_H
#define PARSERRULESLEXER_H

#include "common.h"

typedef enum PLState {DEAD, INITIAL, START_TOKEN, NT, T1, T2, T, OR, LAMBDA1,
                       LAMBDA, ASSIGN1, ASSIGN2, ASSIGN, WHITESPACE}
PLState;

class ParserRulesLexer {
private:
    PLState state;
    std::vector<std::string> tokens;
    std::string currToken;
    bool inAcceptingState();
    void reset();

    PLState getNextState(char c);
    // transition functions for each state
    PLState handleInitial(char c);
    PLState handleStartToken(char c);
    PLState handleNT(char c);
    PLState handleT1(char c);
    PLState handleT2(char c);
    PLState handleT(char c);
    PLState handleOr(char c);
    PLState handleLambda1(char c);
    PLState handleLambda(char c);
    PLState handleAssign1(char c);
    PLState handleAssign2(char c);
    PLState handleAssign(char c);
    PLState handleWhitespace(char c);

public:
    ParserRulesLexer();
    void consume(char c);
    void finalizeInput();
    std::vector<std::string> getTokens();
};



#endif //PARSERRULESLEXER_H

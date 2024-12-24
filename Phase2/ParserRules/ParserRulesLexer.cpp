#include "ParserRulesLexer.h"
#include <iostream>

using namespace std;

ParserRulesLexer::ParserRulesLexer(): state(INITIAL){}

PLState ParserRulesLexer::getNextState(char c) {
    switch (state)  {
        case INITIAL:
          return handleInitial(c);
        case START_TOKEN:
          return handleStartToken(c);
        case NT:
          return handleNT(c);
        case T1:
          return handleT1(c);
        case T2:
          return handleT2(c);
        case T:
          return handleT(c);
        case OR:
          return handleOr(c);
        case LAMBDA1:
          return handleLambda1(c);
        case LAMBDA:
          return handleLambda(c);
        case ASSIGN1:
          return handleAssign1(c);
        case ASSIGN2:
          return handleAssign2(c);
        case ASSIGN:
          return handleAssign(c);
        default:
          // handles dead state
          return DEAD;
    }
}

PLState ParserRulesLexer::handleInitial(char c) {
    if (c == '#') return START_TOKEN;
    if (isalpha(c)) return NT;
    if (c == '\'') return T1;
    if (c == '|') return OR;
    if (c == '\\') return LAMBDA1;
    if (c == ':') return ASSIGN1;
    if (isspace(c)) return WHITESPACE;
    return DEAD;
}

PLState ParserRulesLexer::handleWhitespace(char c) {
    if (isspace(c)) return WHITESPACE;
    return DEAD;
}


PLState ParserRulesLexer::handleStartToken(char c) {
    return DEAD;
}

PLState ParserRulesLexer::handleNT(char c) {
    if (isalnum(c) || c == '_') return NT;
    else return DEAD;
}

PLState ParserRulesLexer::handleT1(char c) {
    if (ispunct(c) || isalnum(c)) return T2;
    else return DEAD;
}

PLState ParserRulesLexer::handleT2(char c) {
    if (c == '\'') return T;
    else if (ispunct(c) || isalnum(c)) return T2;
    else return DEAD;
}

PLState ParserRulesLexer::handleT(char c) {
    return DEAD;
}

PLState ParserRulesLexer::handleOr(char c) {
    return DEAD;
}

PLState ParserRulesLexer::handleLambda1(char c) {
    if (c == 'L') return LAMBDA;
    else return DEAD;
}

PLState ParserRulesLexer::handleLambda(char c) {
    return DEAD;
}

PLState ParserRulesLexer::handleAssign1(char c) {
    if (c == ':') return ASSIGN2;
    else return DEAD;
}

PLState ParserRulesLexer::handleAssign2(char c) {
    if (c == '=') return ASSIGN;
    else return DEAD;
}

PLState ParserRulesLexer::handleAssign(char c) {
    return DEAD;
}

void ParserRulesLexer::consume(char c) {

    PLState nextState = getNextState(c);
    if (nextState != DEAD) {
        currToken += c;
        state = nextState;
        return;
    }

    // 'c' makes the DFA reach a dead state
    // if currState is accepting, append currToken, reset, then try
    // to consume 'c' again
    if (inAcceptingState()) {
        if (state != WHITESPACE) {
            tokens.push_back(currToken);
        }
        reset();
        consume(c);
    } else {
        cerr << "Unrecognized sequence: " << currToken << c << "\n";
        exit(1);
    }
}

void ParserRulesLexer::reset() {
    currToken = "";
    state = INITIAL;
}

vector<string> ParserRulesLexer::getTokens() {
    return tokens;
}

bool ParserRulesLexer::inAcceptingState() {
    return state == START_TOKEN ||
    state == NT ||
    state == T ||
    state == OR ||
    state == LAMBDA ||
    state == WHITESPACE ||
    state == ASSIGN;
}

void ParserRulesLexer::finalizeInput() {
    // 'c' makes the DFA reach a dead state
    // if currState is accepting, append currToken, reset, then try
    // to consume 'c' again
    if (inAcceptingState()) {
        if (state != WHITESPACE) {
            tokens.push_back(currToken);
        }
        reset();
    } else {
        cerr << "Unrecognized sequence: " << currToken << "\n";
        exit(1);
    }
}

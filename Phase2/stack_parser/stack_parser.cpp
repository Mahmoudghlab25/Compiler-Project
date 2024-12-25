#include "stack_parser.h"
// #include <FirstAndFollow.h>
stack_parser:: stack_parser(unordered_map<string,unordered_map<string, vector<string>>> &parserTable,
                            string &statrSymbol, LexicalAnalyzer &lexicalAnalyzer, string &output_path):
                            parserTable(parserTable),
                            startSymbol(statrSymbol),
                            lexicalAnalyzer(lexicalAnalyzer),
                            output_path(output_path){this->index = 0;}

queue<string> stack_parser:: get_actions(){return this->returnedStack;}

string stack_parser:: get_next_token(){
    string token = lexicalAnalyzer.get_token();
    cout<<token<<"\n";
    return token;
}

string stack_parser::string_stack(stack<string> stk){
    string out = "";
    while(!stk.empty()){
        string str = stk.top();
        reverse(str.begin(),str.end());
        out += str;
        out +=" ";
        stk.pop();
    }
    reverse(out.begin(),out.end());
    return out;
}
// there is two options of error 
// 1. the top of stack contains terminal and token not match will output the top of stack and pop it
// 2. the top of stack contains nonterminal and no production in the table will skip this token
void stack_parser::parse(
   set<std::string> terminals
){
    FileWriter fileWriter;
    // index to know the position in the string input
    int index = 0;
    // push $ to stack first
    this->parserStack.push("$");
    //push start symbol to stack
    this->parserStack.push(this->startSymbol);
    string action = string_stack(this->parserStack);
    returnedStack.push(action);
    string nextToken = this->get_next_token();
    tokens.push_back(nextToken);
    string current = this->parserStack.top();
    while(parserStack.size() > 1){
        // check if terminal or not
        bool isTerminal = is_terminal(terminals, current);
        if(isTerminal){
            if(current == nextToken){
                // match terminal
                // action = string_stack(this->parserStack);
                // returnedStack.push(action);
                this->parserStack.pop();
                action = string_stack(this->parserStack);
                returnedStack.push(action);
                current = parserStack.top();
                prodaction.push_back("Match " + nextToken);
                nextToken = this->get_next_token();
            }
            else{
                // error 1
                prodaction.push_back("Inserted error");
                this->parserStack.pop();
                action = string_stack(this->parserStack);
                returnedStack.push(action);
                current = parserStack.top();
            }
        }else{
            // get from the table
            vector <string> prod = this->parserTable[current][nextToken];
            if(prod.size() == 0){
                // error 2 will skip this token and get the next
                nextToken = this->get_next_token();
                prodaction.push_back("Discard error");
                action = string_stack(this->parserStack);
                returnedStack.push(action);
            }
            else if(prod[0] == EPSILON){
                this->parserStack.pop();
                action = string_stack(this->parserStack);
                returnedStack.push(action);
                string prd = current;
                prd += " >>> epsilon";
                prodaction.push_back(prd);
                current = this->parserStack.top();
            }
            else if(prod[0]=="SYNC"){
                this->parserStack.pop();
                action = string_stack(this->parserStack);
//                returnedStack.push("SYNC error");
                returnedStack.push(action);
                string prd = current;
                prd += " >>> SYNC";
                prodaction.push_back(prd);
                current = this->parserStack.top();
            }
            else{
                this->parserStack.pop();
                string prd = "";
                for (int i= prod.size() - 1; i >= 0; i--){
                    this->parserStack.push(prod[i]);
                    prd = prod[i] + prd ;
                    prd += " ";
                }
                prd = current + " >>> " + prd ;
                action = string_stack(this->parserStack);
                returnedStack.push(action);
                current = this->parserStack.top();
                prodaction.push_back(prd);
            }
        }
        tokens.push_back(nextToken);
        auto actions_output_path = output_path + ("ParserActions.md");
        fileWriter.writeActionsToMarkdown(returnedStack,tokens,prodaction,actions_output_path);
    }

}
void stack_parser:: display_action(){
    while(!returnedStack.empty()){
        cout << returnedStack.front() << endl;
        returnedStack.pop();
    }
}
bool stack_parser::is_terminal(set<std::string> terminals,string terminal){
        return terminals.find(terminal) != terminals.end();
    }
vector<string> stack_parser::get_prodaction(){
    return this->prodaction;
}

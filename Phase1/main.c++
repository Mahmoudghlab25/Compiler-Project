#include <iostream>
#include <bits/stdc++.h>
#include "FileReader.c++"
#include "NFA.c++"
using namespace std;
int main(){
    // try{
    //     FileReader fileReader("input.txt");
    //     vector<string> codeLines = fileReader.readLines();
    //     fileReader.printLines(codeLines);
    // }catch (exception& ex){
    //     cerr << "Exception: " << ex.what() << endl;
    // }
    NFA nfa1 = NFA();
    NFA nfa2 = NFA();
    NFA nfa3 = NFA();
    NFA nfa4 = NFA();
    NFA nfa5 = NFA();
    NFA nfa6 = NFA();
    NFA nfa7 = NFA();
    NFA nfa8 = NFA();

    //-------combination---------
    // 1- union(basic , basic)
    // nfa3 = nfa3.Union(nfa1.basic('a'),nfa2.basic('b'));
    // nfa3.display();
    // 2- union(basic , seq)
    // nfa3 = nfa3.Union(nfa1.basic('a'),nfa2.nfa_sequence("def","alpha"));
    // nfa3.display();
    // 3- union(basic , option)
    // nfa3 = nfa3.Union(nfa1.basic('a'),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    // 4- union(seq,seq)
    // nfa3 = nfa3.Union(nfa1.nfa_sequence("abc","alpha"),nfa2.nfa_sequence("def","alpha"));
    // nfa3.display();
    // 5- union(seq,option)
    // nfa3 = nfa3.Union(nfa1.nfa_sequence("abc","alpha"),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    // 6- union(option,option)
    // nfa3 = nfa3.Union(nfa1.nfa_options("abc","alpha"),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    ///-----------Concatenate---------------
    // 1- Concatenate(basic , basic)
    // nfa3 = nfa3.Concatenate(nfa1.basic('a'),nfa2.basic('b'));
    // nfa3.display();
    // 2- Concatenate(basic , seq)
    // nfa3 = nfa3.Concatenate(nfa1.basic('a'),nfa2.nfa_sequence("def","alpha"));
    // nfa3.display();
    // 3- Concatenate(basic , option)
    // nfa3 = nfa3.Concatenate(nfa1.basic('a'),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    // 4- Concatenate(seq,seq)
    // nfa3 = nfa3.Concatenate(nfa1.nfa_sequence("abc","alpha"),nfa2.nfa_sequence("def","alpha"));
    // nfa3.display();
    // 5- Concatenate(seq,option)
    // nfa3 = nfa3.Concatenate(nfa1.nfa_sequence("abc","alpha"),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    // 6- Concatenate(option,option)
    // nfa3 = nfa3.Concatenate(nfa1.nfa_options("abc","alpha"),nfa2.nfa_options("def","alpha"));
    // nfa3.display();
    //-------closure (star)----------
    // 1- closure(basic)
    // nfa2 = nfa2.Closure(nfa1.basic('a'));
    // nfa2.display();
    // 2- closure(seq)
    // nfa2 = nfa2.Closure(nfa1.nfa_sequence("abc","alpha"));
    // nfa2.display();
    // 3- closure(option)
    // nfa2 = nfa2.Closure(nfa1.nfa_options("abc","alpha"));
    // nfa2.display();
    //-------positive closure (plus)----------
    // 1- positive_closure(basic)
    // nfa2 = nfa2.positive_closure(nfa1.basic('a'));
    // nfa2.display();
    // 2- positive_closure(seq)
    // nfa2 = nfa2.positive_closure(nfa1.nfa_sequence("abc","alpha"));
    // nfa2.display();
    // 3- positive_closure(option)
    // nfa2 = nfa2.positive_closure(nfa1.nfa_options("abc","alpha"));
    // nfa2.display();
    // more complex
    // nfa6 = nfa6.Union(nfa3.Concatenate(nfa1.basic('g'),nfa2.nfa_sequence("abc","alpha")),nfa4.Closure(nfa5.nfa_options("def","alpha")));
    // nfa6.display();
    return 0;
}
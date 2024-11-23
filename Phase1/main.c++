#include <iostream>
#include <bits/stdc++.h>
#include "FileReader.c++"

using namespace std;
int main(){
    try{
        FileReader fileReader("input.txt");
        vector<string> codeLines = fileReader.readLines();
        fileReader.printLines(codeLines);
    }catch (exception& ex){
        cerr << "Error: " << ex.what() << endl;
    }
    return 0;
}
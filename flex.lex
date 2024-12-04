%{
#include <stdio.h>
%}

letter [a-zA-Z]
digit [0-9]

id {letter}({letter}|{digit})*
integer {digit}+
num {digit}+"."{digit}+([eE][+-]?{digit}+)?

%%
"if"                 { printf("Keyword: if\n"); }
"else"               { printf("Keyword: else\n"); }
"while"              { printf("Keyword: while\n"); }
{id}                 { printf("ID: %s\n", yytext); }
{num}              { printf("float: %s\n", yytext); }
{integer}            { printf("Integer: %s\n", yytext); }
"=="                 { printf("Relop: ==\n"); }
"!="                 { printf("Relop: !=\n"); }
">="                 { printf("Relop: >=\n"); }
">"                  { printf("Relop: >\n"); }
"<="                 { printf("Relop: <=\n"); }
"<"                  { printf("Relop: <\n"); }
"="                  { printf("Assign: =\n"); }
";"                  { printf("Punctuation: ;\n"); }
","                  { printf("Punctuation: ,\n"); }
"("                  { printf("Punctuation: (\n"); }
")"                  { printf("Punctuation: )\n"); }
"{"                  { printf("Punctuation: {\n"); }
"}"                  { printf("Punctuation: }\n"); }
"+"                  { printf("Addop: +\n"); }
"-"                  { printf("Addop: -\n"); }
"*"                  { printf("Mulop: *\n"); }
"/"                  { printf("Mulop: /\n"); }
[ \t\n\r]+           { /* skip whitespace */ }
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}


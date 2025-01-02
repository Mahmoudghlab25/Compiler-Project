
## First Set Sets
| Non-Terminal | First Set Values                  |
|--------------|-------------------------------------------|
| ASSIGNMENT | id  |
| DECLARATION | float  int  |
| EXPRESSION | (  addop  id  num  |
| EXPRESSION`1 | epsilon  relop  |
| FACTOR | (  id  num  |
| IF | if  |
| METHOD_BODY | float  id  if  int  while  |
| PRIMITIVE_TYPE | float  int  |
| SIGN | addop  |
| SIMPLE_EXPRESSION | (  addop  id  num  |
| SIMPLE_EXPRESSION` | addop  epsilon  |
| STATEMENT | float  id  if  int  while  |
| STATEMENT_LIST | float  id  if  int  while  |
| STATEMENT_LIST` | epsilon  float  id  if  int  while  |
| TERM | (  id  num  |
| TERM` | epsilon  mulop  |
| WHILE | while  |

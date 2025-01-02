
## Follow Set Sets
| Non-Terminal | Follow Set Values                  |
|--------------|-------------------------------------------|
| ASSIGNMENT | $  float  id  if  int  while  }  |
| DECLARATION | $  float  id  if  int  while  }  |
| EXPRESSION | )  ;  |
| EXPRESSION`1 | )  ;  |
| FACTOR | )  ;  addop  mulop  relop  |
| IF | $  float  id  if  int  while  }  |
| METHOD_BODY | $  |
| PRIMITIVE_TYPE | id  |
| SIGN | (  id  num  |
| SIMPLE_EXPRESSION | )  ;  relop  |
| SIMPLE_EXPRESSION` | )  ;  relop  |
| STATEMENT | $  float  id  if  int  while  }  |
| STATEMENT_LIST | $  |
| STATEMENT_LIST` | $  |
| TERM | )  ;  addop  relop  |
| TERM` | )  ;  addop  relop  |
| WHILE | $  float  id  if  int  while  }  |

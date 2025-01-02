
## Grammar Table
| Non-Terminal | Production Rules                                      |
|--------------|-------------------------------------------------------|
| FACTOR | [ id ] [ num ] [ ( EXPRESSION ) ] |
| TERM | [ FACTOR TERM` ] |
| SIMPLE_EXPRESSION | [ TERM SIMPLE_EXPRESSION` ] [ SIGN TERM SIMPLE_EXPRESSION` ] |
| STATEMENT_LIST` | [ STATEMENT STATEMENT_LIST` ] [ epsilon ] |
| IF | [ if ( EXPRESSION ) { STATEMENT } else { STATEMENT } ] |
| EXPRESSION | [ SIMPLE_EXPRESSION EXPRESSION`1 ] |
| METHOD_BODY | [ STATEMENT_LIST ] |
| SIMPLE_EXPRESSION` | [ addop TERM SIMPLE_EXPRESSION` ] [ epsilon ] |
| STATEMENT_LIST | [ STATEMENT STATEMENT_LIST` ] |
| STATEMENT | [ DECLARATION ] [ IF ] [ WHILE ] [ ASSIGNMENT ] |
| EXPRESSION`1 | [ epsilon ] [ relop SIMPLE_EXPRESSION ] |
| TERM` | [ mulop FACTOR TERM` ] [ epsilon ] |
| DECLARATION | [ PRIMITIVE_TYPE id ; ] |
| SIGN | [ addop ] |
| ASSIGNMENT | [ id assign EXPRESSION ; ] |
| WHILE | [ while ( EXPRESSION ) { STATEMENT } ] |
| PRIMITIVE_TYPE | [ int ] [ float ] |

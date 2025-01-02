| Non-Terminal | $ | ( | ) | ; | addop | float | id | if | int | mulop | num | relop | while | } |
|--------------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|
| TERM |   | FACTOR, TERM` | SYNC | SYNC | SYNC |   | FACTOR, TERM` |   |   |   | FACTOR, TERM` | SYNC |   |   |
| STATEMENT_LIST` | epsilon |   |   |   |   | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` |   |   |   | STATEMENT, STATEMENT_LIST` |   |
| ASSIGNMENT | SYNC |   |   |   |   | SYNC | id, assign, EXPRESSION, ; | SYNC | SYNC |   |   |   | SYNC | SYNC |
| WHILE | SYNC |   |   |   |   | SYNC | SYNC | SYNC | SYNC |   |   |   | while, (, EXPRESSION, ), {, STATEMENT, } | SYNC |
| PRIMITIVE_TYPE |   |   |   |   |   | float | SYNC |   | int |   |   |   |   |   |
| TERM` |   |   | epsilon | epsilon | epsilon |   |   |   |   | mulop, FACTOR, TERM` |   | epsilon |   |   |
| DECLARATION | SYNC |   |   |   |   | PRIMITIVE_TYPE, id, ; | SYNC | SYNC | PRIMITIVE_TYPE, id, ; |   |   |   | SYNC | SYNC |
| SIGN |   | SYNC |   |   | addop |   | SYNC |   |   |   | SYNC |   |   |   |
| EXPRESSION |   | SIMPLE_EXPRESSION, EXPRESSION`1 | SYNC | SYNC | SIMPLE_EXPRESSION, EXPRESSION`1 |   | SIMPLE_EXPRESSION, EXPRESSION`1 |   |   |   | SIMPLE_EXPRESSION, EXPRESSION`1 |   |   |   |
| IF | SYNC |   |   |   |   | SYNC | SYNC | if, (, EXPRESSION, ), {, STATEMENT, }, else, {, STATEMENT, } | SYNC |   |   |   | SYNC | SYNC |
| EXPRESSION`1 |   |   | epsilon | epsilon |   |   |   |   |   |   |   | relop, SIMPLE_EXPRESSION |   |   |
| SIMPLE_EXPRESSION |   | TERM, SIMPLE_EXPRESSION` | SYNC | SYNC | SIGN, TERM, SIMPLE_EXPRESSION` |   | TERM, SIMPLE_EXPRESSION` |   |   |   | TERM, SIMPLE_EXPRESSION` | SYNC |   |   |
| FACTOR |   | (, EXPRESSION, ) | SYNC | SYNC | SYNC |   | id |   |   | SYNC | num | SYNC |   |   |
| METHOD_BODY | SYNC |   |   |   |   | STATEMENT_LIST | STATEMENT_LIST | STATEMENT_LIST | STATEMENT_LIST |   |   |   | STATEMENT_LIST |   |
| STATEMENT | SYNC |   |   |   |   | DECLARATION | ASSIGNMENT | IF | DECLARATION |   |   |   | WHILE | SYNC |
| SIMPLE_EXPRESSION` |   |   | epsilon | epsilon | addop, TERM, SIMPLE_EXPRESSION` |   |   |   |   |   |   | epsilon |   |   |
| STATEMENT_LIST | SYNC |   |   |   |   | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` | STATEMENT, STATEMENT_LIST` |   |   |   | STATEMENT, STATEMENT_LIST` |   |

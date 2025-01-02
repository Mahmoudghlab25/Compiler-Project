# Parser Table

## Stack, Tokens, and Productions
| Stack Element                | Token               | Production          |
|------------------------------|---------------------|---------------------|
|  $ METHOD_BODY | int | METHOD_BODY >>> STATEMENT_LIST   |
|  $ STATEMENT_LIST | int | STATEMENT_LIST >>> STATEMENT  STATEMENT_LIST`   |
|  $ STATEMENT_LIST` STATEMENT | int | STATEMENT >>> DECLARATION   |
|  $ STATEMENT_LIST` DECLARATION | int | DECLARATION >>> PRIMITIVE_TYPE  id  ;   |
|  $ STATEMENT_LIST` ; id PRIMITIVE_TYPE | int | PRIMITIVE_TYPE >>> int   |
|  $ STATEMENT_LIST` ; id int | int | Match int |
|  $ STATEMENT_LIST` ; id | id | Match id |
|  $ STATEMENT_LIST` ; | ; |  |

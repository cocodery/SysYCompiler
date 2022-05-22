lexer grammar SysYToken;

// keyword
Int : 'int';
Void: 'void';
Const: 'const';
Return : 'return';
If : 'if';
Else : 'else';
While : 'while';
For : 'for';
Do : 'do';
Break : 'break';
Continue : 'continue';


// operator
Lparen : '(' ;
Rparen : ')' ;
Lbrkt : '[' ;
Rbrkt : ']' ;
Lbrace : '{' ;
Rbrace : '}' ;
Comma : ',' ;
Semicolon : ';';

Minus : '-';
Addition : '+';
Exclamation : '!';
Multiplication : '*';
Division : '/';
Modulo : '%';
LAND : '&&';
LOR : '||';
EQ : '==';
NEQ : '!=';
LT : '<';
LE : '<=';
GT : '>';
GE : '>=';

// integer, identifier
IntLiteral
    : [0-9]+
    | ('0x'|'0X')[0-9a-fA-F]+
    ;

FloatLiteral
    : ([0-9]+|[0-9]*'.'[0-9]*)([eE]('+'|'-')?[0-9]+)?[fFlL]?
    | ('0x'|'0X')([0-9a-fA-F]*'.'[0-9a-fA-F]*)([pP]('+'|'-')?[0-9]+)?[fFlL]?
    ;


Identifier
    : [a-zA-Z_][a-zA-Z_0-9]*
    ;

WS : 
    [ \t\r\n] -> skip
    ;

LINE_COMMENT : '//' .*? '\r'? '\n' -> skip;
COMMENT      :'/*'.*?'*/'-> skip ;
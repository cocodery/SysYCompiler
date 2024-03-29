grammar SysY;

import SysYToken;

compUnit : (decl | funcDef)* EOF;

decl : constDecl | varDecl;

constDecl : 'const' bType constDef (',' constDef)* ';' ;

bType: 'int' | 'float';

constDef : Identifier ('[' constExp ']')* '=' constInitVal;

constInitVal
    : constExp # scalarConstInitVal
    | '{' (constInitVal (',' constInitVal)* )? '}' # listConstInitVal
    ;

varDecl : bType varDef (',' varDef)* ';';

varDef
    : Identifier ('[' constExp ']')* # uninitVarDef
    | Identifier ('[' constExp ']')* '=' initVal # initVarDef
    ;

initVal
    : exp # scalarInitVal
    | '{' (initVal (',' initVal)* )? '}' # listInitval
    ;

funcDef : funcType Identifier '(' (funcFParams)? ')' block;

funcType : 'void' | 'int' | 'float';

funcFParams : funcFParam (',' funcFParam)*;

funcFParam : bType Identifier ('[' ']' ('[' constExp ']')* )?;

block : '{' (blockItem)* '}';

blockItem : decl | stmt;

stmt
    : lVal '=' exp ';' # assignment
    | (exp)? ';' # expStmt
    | block # blockStmt
    | 'if' '(' cond ')' stmt ('else' stmt)? # ifStmt
    | 'while' '(' cond ')' stmt # whileStmt
    | 'for' '(' (decl)? ';' (cond)? ';' ( exp(',' exp)* )? ')' stmt # forStmt
    | 'do' stmt 'while' '(' cond ')' # dowhileStmt
    | 'break' ';' # breakStmt
    | 'continue' ';' # continueStmt
    | 'return' (exp)? ';' # returnStmt
    ;

exp : addExp;

cond : lOrExp;

lVal : Identifier ('[' exp ']')*;

primaryExp
    : '(' exp ')' # primaryExp1
    | lVal # primaryExp2
    | number # primaryExp3
    ;

number
    : IntLiteral #number1
    | FloatLiteral #number2
    ; 

unaryExp
    : primaryExp # unary1
    | Identifier '(' (funcRParams)? ')' # unary2
    | unaryOp unaryExp # unary3
    ;

unaryOp : '+' | '-' | '!';

funcRParams : funcRParam (',' funcRParam)*;

funcRParam : exp ;

mulExp
    : unaryExp # mul1
    | mulExp ('*' | '/' | '%') unaryExp # mul2
    ;

addExp
    : mulExp # add1
    | addExp ('+' | '-') mulExp # add2
    ;

relExp
    : addExp # rel1
    | relExp ('<' | '>' | '<=' | '>=') addExp # rel2
    ;

eqExp
    : relExp # eq1
    | eqExp ('==' | '!=') relExp # eq2
    ;

lAndExp
    : eqExp # lAnd1
    | lAndExp '&&' eqExp # lAnd2
    ;

lOrExp
    : lAndExp # lOr1
    | lOrExp '||' lAndExp # lOr2
    ;

constExp : addExp ;
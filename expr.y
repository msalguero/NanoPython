%{
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stack>
#include "tree.h"

#define YYERROR_VERBOSE 1
using namespace std;

extern std::stack<int> s;
extern int pending;
extern int lines;

int yylex();
void yyerror(const char *str);

%}

%union{
	char* yystr;
	int yyint;
  Expr* yyExpr;
  Sentence* yySentence;
  ClassDefinition* yyClassDefinition;
  GlobalVariable* yyGlobalVariable;
  GlobalVariableList* yyGlobalVariableList;
  Method* yyMethod;
  MethodList* yyMethodList;
  MethodCallSentence* yyMethodCall;
  ExprList* yyExprList;
}

%token TK_CLASS;
%token TK_DEF;
%token TK_INDENT;
%token TK_DEDENT;
%token TK_NEWLINE;
%token TK_IF;
%token TK_WHILE;
%token TK_STRING;
%token TK_ELSE;
%token<yyint> TK_NUM
%token TK_PRINT
%token<yystr> TK_ID
%type<yyClassDefinition> CLASS
%type<yyGlobalVariableList> VARIABLE_LIST
%type<yyGlobalVariable> VARIABLE
%type<yyMethodList> METHOD_LIST
%type<yyMethod> METHOD
%type<yyMethodCall> METHOD_CALL
%type<yyExprList> OP_EXPR_LIST
%type<yyExprList> EXPR_LIST
%type<yyExpr> E
%type<yySentence> PRINT
%type<yySentence> DECLARATION
%type<yySentence> IF
%type<yySentence> WHILE
%type<yySentence> SENTENCE
%type<yySentence> SENTENCE_LIST
%type<yySentence> BLOCK
%type<yySentence> ELSE

%nonassoc TK_GREATER_EQUAL;
%nonassoc TK_LESS_EQUAL;
%nonassoc TK_EQUAL;
%nonassoc TK_NOT_EQUAL;
%nonassoc '<'
%nonassoc '>'
%left '+'
%left '-'
%left '*'
%left '/'

%%

input : CLASS {
  string code = $1->GenerateCode();
  cout << code << endl; 
  ofstream myfile;
  myfile.open ("generatedCode.asm");
  myfile << code;
  myfile.close();
}

CLASS: TK_CLASS TK_ID ':' TK_NEWLINE TK_INDENT VARIABLE_LIST METHOD_LIST TK_DEDENT { $$ =  new ClassDefinition($6, $7); }
;

VARIABLE_LIST: VARIABLE_LIST VARIABLE { $$ = $1; $$->push_back($2); }
        | VARIABLE { $$ = new GlobalVariableList(); $$->push_back($1); }
;

VARIABLE: TK_ID '=' E TK_NEWLINE { $$ = new GlobalVariable($3, $1); free($1); }
;

METHOD_LIST: METHOD_LIST METHOD { $$ = $1; $$->push_back($2); }
        | METHOD { $$ = new MethodList(); $$->push_back($1); }
;

METHOD: TK_DEF TK_ID '(' OP_PARAMETER_LIST ')' ':' TK_NEWLINE BLOCK { $$ = new Method($2, $8); }
;

OP_PARAMETER_LIST: PARAMETER_LIST 
                  | 
;

PARAMETER_LIST: PARAMETER_LIST ',' PARAMETER
                  | PARAMETER
;

PARAMETER: TK_ID ':' TK_ID 
;

SENTENCE_LIST : SENTENCE SENTENCE_LIST { $1->next = $2; $$ = $1; }
   | SENTENCE { $$ = $1; }
;

SENTENCE: DECLARATION { $$ = $1; }
   | PRINT { $$ = $1; }
   | IF { $$ = $1; }
   | WHILE { $$ = $1; }
   | METHOD_CALL { $$ = $1; }
;

DECLARATION: TK_ID '=' E TK_NEWLINE {$$ = new AssignSentence($3, $1);}
;

METHOD_CALL: TK_ID '(' OP_EXPR_LIST ')' TK_NEWLINE { $$ = new MethodCallSentence($1, $3);}
;

OP_EXPR_LIST: EXPR_LIST { $$ = $1; }
          | { $$ = NULL; }
;

EXPR_LIST: EXPR_LIST ',' E { $$ = $1; $$->push_back($3); }
                  | E { $$ = new ExprList(); $$->push_back($1); }
;

PRINT: TK_PRINT '(' E ')' TK_NEWLINE { $$ = new PrintSentence($3);}
;

IF: TK_IF E ':' TK_NEWLINE BLOCK ELSE { $$ = new IfSentence($2, $5, $6); }
;

ELSE: TK_ELSE TK_NEWLINE BLOCK { $$ = $3; }
    |  { $$ = NULL; }
;

WHILE: TK_WHILE E ':' TK_NEWLINE BLOCK { $$ = new WhileSentence($2, $5);}

BLOCK: TK_INDENT SENTENCE_LIST TK_DEDENT { $$ = $2; }
;

E: E '<' E { $$ = new LessExpression($1, $3); }
  | E '>' E { $$ = new GreaterExpression($1, $3); }
  | E TK_EQUAL E { $$ = new EqualExpression($1, $3); }
  | E TK_NOT_EQUAL E { $$ = new NotEqualExpression($1, $3); }
  | E TK_GREATER_EQUAL E { $$ = new GreaterOrEqualExpression($1, $3); }
  | E TK_LESS_EQUAL E { $$ = new LessOrEqualExpression($1, $3); }
  | E '+' E { $$ = new AddExpression($1, $3); }
  | E '-' E { $$ = new SubExpression($1, $3); }
  | E '*' E { $$ = new MultExpression($1,$3); }
  | E '/' E { $$ = new DivExpression($1,$3); }
  | '(' E ')' { $$ = $2; }
  | TK_NUM { $$ = new NumExpression($1); }
  | TK_ID { $$ = new IdExpression($1);}
;

%%

int main()
{
  s.push(0);
  pending = 0;
  lines = 0;
  yyparse();
  
  return 0;
}

void yyerror(const char *str)
{
    cout << str << "line: " <<to_string(lines) << endl;
}
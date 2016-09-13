%{

#include <iostream>
#include <map>
#include <string>
#include <stack>
#include "tree.h"

#define YYERROR_VERBOSE 1
using namespace std;

extern std::stack<int> s;
extern int pending;

int yylex();
void yyerror(const char *str);

%}

%union{
	char* yystr;
	int yyint;
  Expr* yyExpr;
  Sentence* yySentence;
}

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
input : SENTENCE_LIST { cout << $1->GenerateCodeList() << endl; }

SENTENCE_LIST : SENTENCE SENTENCE_LIST { $1->next = $2; $$ = $1; }
   | SENTENCE { $$ = $1; }
;

SENTENCE: DECLARATION { $$ = $1; }
   | PRINT { $$ = $1; }
   | IF { $$ = $1; }
   | WHILE { $$ = $1; }
;

DECLARATION: TK_ID '=' E TK_NEWLINE {$$ = new AssignSentence($3, $1);}
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

  yyparse();
  
  return 0;
}

void yyerror(const char *str)
{
    cout << str;
}
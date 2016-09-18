%{

#include <cstdio>
#include <cstdlib>
#include <stack>
#include "tree.h"
#include "tokens.h"

  int lines;
  std::stack<int> s;
  int CheckIndent(int, int);
  int pending;
  
%}

%option noyywrap
%x TAB

ID [a-zA-Z_][a-zA-Z0-9_]*
STRING_LITERAL "\"".*"\""

%%

%{
	if(pending > 0){
		pending--;
		s.pop();
		return TK_DEDENT;
	}
%}

[ ]
[\n]  { BEGIN(TAB); lines++; return TK_NEWLINE;}
{STRING_LITERAL} return TK_STRING;
"<" { return '<'; }
">" { return '>'; }
">=" { return TK_GREATER_EQUAL; }
"<=" { return TK_LESS_EQUAL; }
"==" { return TK_EQUAL; }
"!=" { return TK_NOT_EQUAL; }
"+" { return '+'; }
"*" { return '*'; }
"(" { return '('; }
")" { return ')'; }
"=" { return '='; }
":" { return ':'; }
"," { return ','; }
"print" { return TK_PRINT; }
"if" { return TK_IF; }
"else" { return TK_ELSE; }
"while" { return TK_WHILE; }
"class" { return TK_CLASS; }
"def" { return TK_DEF; }
{ID} { yylval.yystr = strdup(yytext); return TK_ID; }
[0-9]+ { yylval.yyint = atoi(yytext); return TK_NUM; }
. { printf("Error: Caracter no valido: %s\n", yytext); }

<TAB>^[ ]* {
			BEGIN(INITIAL);
			int top = s.top();
			int tabCount = strlen(yytext);
			int result = CheckIndent(top, tabCount);
			if(result != -1)
				return result;
		}
<TAB>^[^ ] {
			unput(yytext[0]);
			BEGIN(INITIAL);
			int top = s.top();
			int tabCount = 0;
			int result = CheckIndent(top, tabCount);
			if(result != -1)
				return result;
		}
<TAB><<EOF>> {
			int top = s.top();
			int tabCount = 0;
			int result = CheckIndent(top, tabCount);
			if(result != -1)
				return result;
			return 0;
		}
%%

int CheckIndent(int top, int tabCount){
	if( tabCount > top ){
		if(tabCount - top != 4){
			printf("Error: Cantidad de espacios no valido: %d\n", tabCount);
			exit(0);
		}
		s.push(tabCount);
		return TK_INDENT;
	}else if( tabCount < top){
		s.pop();
		pending = (s.top() -tabCount) / 4;
		return TK_DEDENT;
	}
	return -1;
}
%{
#include "mochi.h"
#include "mochi.tab.h"
#define TOKEN(t) (yylval.token = t)
%}

%%
\n return TOKEN(TNEWLINE);
[\+\-] yylval.string = yytext; return TOP;
[a-z]+ yylval.string = yytext; return TNAME;
[0-9]+ yylval.node = NEW_LIT((VALUE)atoi(yytext)); return TNUMBER;
[ \t]
. printf("Unknown token!: %s\n", yytext); yyterminate();
%%

yywrap() {
  return 1;
}

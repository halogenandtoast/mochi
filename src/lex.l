%{
#include "mochi.tab.h"
#define TOKEN(t) (yylval.token = t)
%}

%%
"+" return TOKEN(TPLUS);
. printf("Unknown token!\n"); yyterminate();
%%

yywrap() {
  return(1);
}

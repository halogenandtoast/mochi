#include <stdio.h>
extern int yylex();

int main(int argc, char** argv) {
  int token;
  printf("Mochi\n");
  yyparse();
  return 0;
}

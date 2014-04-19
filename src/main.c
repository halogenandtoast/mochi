#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "y.tab.h"

extern FILE *yyin;
extern int yyparse();

int main(int argc, char **argv) {
  if(argc > 1) {
    yyin = fopen(argv[1], "r");
  }
  yyparse();
  return EXIT_SUCCESS;
}

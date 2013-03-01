#include <stdio.h>
#include "mochi.h"
extern int yylex();

VALUE mochi_newobj(void) {
  VALUE object;
  return object;
}

NODE* mochi_node_newnode(enum node_type type, VALUE val1, VALUE val2, VALUE val3) {
  NODE* node = (NODE*)mochi_newobj();
  node->value1 = val1;
  return node;
}

int main(int argc, char** argv) {
  yyparse();
  return 0;
}

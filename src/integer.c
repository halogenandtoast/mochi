#include <stdio.h>
#include "mochi.h"
#include "vm.h"
#include "class.h"

VALUE mochi_integer_print(VM *vm, VALUE self, VALUE *argv) {
  printf("%d\n", (int) (self >> 1));
  return self;
}

void init_integer(VM *vm) {
  VALUE cInteger = mochi_create_class(vm, "Integer");
  mochi_define_method(cInteger, "puts", mochi_integer_print);
}

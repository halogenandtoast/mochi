#include <stdio.h>
#include "mochi.h"
#include "string.h"
#include "vm.h"
#include "class.h"

#define SIZEOF_VALUE sizeof(unsigned int *)
#define CHAR_BIT 8
const char mochi_digitmap[] = "0123456789";

VALUE mochi_integer_to_s(VM*, VALUE, VALUE*);

VALUE mochi_integer_print(VM *vm, VALUE self, VALUE *argv) {
  VALUE out = mochi_get_global(vm, "stdout");
  VALUE string = mochi_integer_to_s(vm, self, 0);
  mochi_function_call(vm, "puts", out, 1, string);
  return self;
}

VALUE mochi_integer_to_s(VM *vm, VALUE self, VALUE *argv) {
  char buf[CHAR_BIT * SIZEOF_VALUE + 2], *b = buf + sizeof(buf);
  int base = 10;
  int val = (int) (self >> 1);

  *--b = '\0';
  do {
    *--b = mochi_digitmap[(int)(val % base)];
  } while(val /= base);

  return create_string(vm, b);
}

void init_integer(VM *vm) {
  VALUE cInteger = mochi_create_class(vm, "Integer");
  mochi_define_method(cInteger, "puts", mochi_integer_print);
  mochi_define_method(cInteger, "to_s", mochi_integer_to_s);
}

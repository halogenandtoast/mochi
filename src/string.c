#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mochi.h"
#include "vm.h"
#include "class.h"
#include "string.h"
#include "node.h"

VALUE mochi_string_print(VM *vm, VALUE self, VALUE *argv) {
  VALUE out = mochi_get_global(vm, "stdout");
  mochi_function_call(vm, "puts", out, 1, self);
  return self;
}

VALUE mochi_string_downcase(VM *vm, VALUE self, VALUE *argv) {
  char *str = strdup(((struct MString *) self)->content);
  for(int i = 0; str[i]; i++) str[i] = tolower(str[i]);

  return create_string(vm, str);
}

VALUE mochi_string_upcase(VM *vm, VALUE self, VALUE *argv) {
  char *str = strdup(((struct MString *) self)->content);
  for(int i = 0; str[i]; i++) str[i] = toupper(str[i]);

  return create_string(vm, str);
}

VALUE mochi_string_capitalize(VM *vm, VALUE self, VALUE *argv) {
  VALUE new_string = mochi_string_downcase(vm, self, 0);
  char *content = ((struct MString *) new_string)->content;
  content[0] = toupper(content[0]);
  return new_string;
}

VALUE mochi_string_reverse(VM *vm, VALUE self, VALUE *argv) {
  char temp, *end_ptr;
  char *str = strdup(((struct MString *) self)->content);
  char *start = str;

  end_ptr = str + strlen(str) - 1;

  while( end_ptr > str ) {
    temp = *str;
    *str = *end_ptr;
    *end_ptr = temp;
    str++;
    end_ptr--;
  }

  return create_string(vm, start);
}

VALUE create_string(VM *vm, char *content) {
  struct MString *new_string = (struct MString *) malloc(sizeof(struct MString));
  new_string->header = (struct MObjectHeader *) malloc(sizeof(struct MObjectHeader));
  new_string->header->klass = mochi_get_constant(vm, "String");
  new_string->content = content;
  new_string->length = strlen(content);
  return (VALUE) new_string;
}

void init_string(VM *vm) {
  VALUE cString = mochi_create_class(vm, "String");
  mochi_define_method(cString, "puts", mochi_string_print);
  mochi_define_method(cString, "reverse", mochi_string_reverse);
  mochi_define_method(cString, "downcase", mochi_string_downcase);
  mochi_define_method(cString, "upcase", mochi_string_upcase);
  mochi_define_method(cString, "capitalize", mochi_string_capitalize);
}

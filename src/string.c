#include <stdio.h>
#include <string.h>
#include "mochi.h"
#include "vm.h"
#include "class.h"
#include "string.h"

VALUE mochi_string_print(VM *vm, VALUE self) {
  printf("%s\n", ((struct MString *) self)->content);
  return self;
}

VALUE mochi_string_reverse(VM *vm, VALUE self) {
  char temp, *end_ptr;
  char *str = strdup(((struct MString *) self)->content);
  char *start = str;

  if( str == NULL || !(*str) )
    return (VALUE) 0;

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
}

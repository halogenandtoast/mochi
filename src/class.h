#ifndef _CLASS_H_
#define _CLASS_H_

#include "node.h"
#include "vm.h"

struct MMethod {
  char *name;
  struct MMethod *method;
  VALUE (*c_func)();
};

struct MClass {
  char *name;
  struct MMethod *method;
};

VALUE mochi_create_class(VM *vm, char *name);
void mochi_define_method(VALUE object, char *method_name, VALUE (*c_func)());

#endif

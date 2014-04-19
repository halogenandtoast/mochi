#include "class.h"
#include "node.h"
#include "vm.h"
#include <string.h>
#include <stdlib.h>

VALUE mochi_create_class(VM *vm, char *name) {
  struct MClass *klass = (struct MClass *) malloc(sizeof(struct MClass));
  klass->name = strdup(name);

  define_constant(vm, name, (VALUE) klass);

  return (VALUE) klass;
}

void mochi_define_method(VALUE object, char *method_name, VALUE (*c_func)()) {
  struct MClass *klass = (struct MClass *) object;
  struct MMethod *new_method = (struct MMethod *) malloc(sizeof(struct MMethod));
  new_method->name = method_name;
  new_method->c_func = c_func;
  new_method->method = klass->method;
  klass->method = new_method;
}


#ifndef _VM_H_
#define _VM_H_

#include "node.h"

struct ConstantListItem {
  char *name;
  VALUE value;
  struct ConstantListItem *next;
};

typedef struct VM {
  struct ConstantListItem *constant;
} VM;

VM *vm_init();
void vm_destroy(VM *vm);
void define_constant(VM *vm, char *name, VALUE object);
VALUE mochi_get_constant(VM *vm, char *name);
VALUE mochi_get_class(VM *vm, VALUE value);

#endif

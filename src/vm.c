#include "vm.h"
#include "integer.h"
#include "string.h"
#include "mochi.h"
#include <string.h>
#include <stdio.h>

VM *vm_init() {
  VM *vm = (VM *) malloc(sizeof(VM));
  init_integer(vm);
  init_string(vm);
  return vm;
}

void vm_destroy(VM *vm) {
  free(vm);
}

void define_constant(VM *vm, char *name, VALUE object) {
  struct ConstantListItem *new_constant = (struct ConstantListItem *) malloc(sizeof(struct ConstantListItem));
  new_constant->name = strdup(name);
  new_constant->next = vm->constant;
  new_constant->value = (VALUE) object;

  vm->constant = new_constant;
}

VALUE mochi_get_constant(VM *vm, char *name) {
  struct ConstantListItem *current_constant = vm->constant;
  while(current_constant && strcmp(current_constant->name, name) != 0)
    current_constant = current_constant->next;
  if(current_constant) {
    return current_constant->value;
  }
  fprintf(stderr, "Failed to get constant: %s\n", name);
  return (VALUE) 0;
}

VALUE mochi_get_class(VM *vm, VALUE value) {
  if(value & FIXNUM_FLAG) {
    return mochi_get_constant(vm, "Integer");
  } else {
    struct MObjectHeader *header = ((struct MObject *) value)->header;
    return header->klass;
  }
  return (VALUE) 0;
}

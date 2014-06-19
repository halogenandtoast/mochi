#include "vm.h"
#include "integer.h"
#include "string.h"
#include "io.h"
#include "mochi.h"
#include "node.h"
#include <string.h>
#include <stdio.h>

VM *vm_init() {
  VM *vm = (VM *) malloc(sizeof(VM));
  vm->stack = (VALUE *) malloc(sizeof(VALUE) * STACK_SIZE);
  vm->sp = 0;

  init_integer(vm);
  init_string(vm);
  init_io(vm);

  define_global(vm, "stdin", create_io(vm, fileno(stdin)));
  define_global(vm, "stdout", create_io(vm, fileno(stdout)));

  return vm;
}

void vm_destroy(VM *vm) {
  free(vm->instructions);
  free(vm->stack);
  free(vm);
}

void vm_eval(VM *vm, Instruction *instructions) {
  VALUE right, left, self, result;
  Instruction instruction;
  vm->instructions = instructions;
  vm->ip = 0;

  while(vm->instructions[vm->ip].op != LEAVE) {
    instruction = vm->instructions[vm->ip];
    switch(instruction.op) {
      case PUSH_LITERAL:
        vm->stack[vm->sp++] = instruction.value;
        break;
      case PUSH_STRING:
        vm->stack[vm->sp++] = create_string(vm, instruction.label);
        break;
      case OP_PLUS:
        right = vm->stack[--vm->sp];
        left = vm->stack[--vm->sp];
        result = (left >> 1) + (right >> 1);
        vm->stack[vm->sp++] = INT2FIX(result);
        break;
      case OP_SEND:
        self = vm->stack[--vm->sp];
        vm->stack[vm->sp++] = mochi_function_call(vm, instruction.label, self, 0);
        break;
      case NOOP:
      case LEAVE:
        break;
    }
    vm->ip++;
  }
}

struct vTable *create_vtable_entry(char *name, VALUE value, struct vTable *next) {
  struct vTable *entry = (struct vTable *) malloc(sizeof(struct vTable));
  entry->name = strdup(name);
  entry->next = next;
  entry->value = (VALUE) value;
  return entry;
}

void define_global(VM *vm, char *name, VALUE object) {
  vm->global = create_vtable_entry(name, object, vm->global);
}

void define_constant(VM *vm, char *name, VALUE object) {
  vm->constant = create_vtable_entry(name, object, vm->constant);
}

struct vTable *get_vtable_entry(struct vTable *table, char *name) {
  struct vTable *current_entry = table;
  while(current_entry && strcmp(current_entry->name, name) != 0)
    current_entry = current_entry->next;
  return current_entry;
}

VALUE mochi_get_constant(VM *vm, char *name) {
  struct vTable *constant = get_vtable_entry(vm->constant, name);
  if(constant) {
    return constant->value;
  }
  fprintf(stderr, "Failed to get constant: %s\n", name);
  return (VALUE) 0;
}

VALUE mochi_get_global(VM *vm, char *name) {
  struct vTable *global = get_vtable_entry(vm->global, name);
  if(global) {
    return global->value;
  }
  fprintf(stderr, "Failed to get global: %s\n", name);
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

#include "vm.h"
#include "integer.h"
#include "string.h"
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
        vm->stack[vm->sp++] = mochi_function_call(vm, instruction.label, self, (VALUE) 0);
        break;
      case NOOP:
      case LEAVE:
        break;
    }
    vm->ip++;
  }
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

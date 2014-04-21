#ifndef _VM_H_
#define _VM_H_

#include "mochi.h"

#define STACK_SIZE 10000

enum op_code {
  NOOP,
  PUSH_LITERAL,
#define PUSH_LITERAL PUSH_LITERAL
  PUSH_STRING,
#define PUSH_STRING PUSH_STRING
  OP_PLUS,
#define OP_PLUS OP_PLUS
  OP_SEND,
#define OP_SEND OP_SEND
  LEAVE
#define LEAVE LEAVE
};

struct ConstantListItem {
  char *name;
  VALUE value;
  struct ConstantListItem *next;
};

typedef struct Instruction {
  enum op_code op;
  char *label;
  VALUE value;
} Instruction;

typedef struct VM {
  struct ConstantListItem *constant;
  int sp;
  int ip;
  VALUE *stack;
  Instruction *instructions;
} VM;

VM *vm_init();
void vm_destroy(VM *vm);
void vm_eval(VM *vm, Instruction *instructions);
void define_constant(VM *vm, char *name, VALUE object);
VALUE mochi_get_constant(VM *vm, char *name);
VALUE mochi_get_class(VM *vm, VALUE value);

#endif

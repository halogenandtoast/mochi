#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "mochi.h"
#include "vm.h"
#include "class.h"
#include "string.h"

void create_instruction(Instruction *instruction, enum op_code code) {
  instruction->op = code;
}

void create_send(Instruction *instruction, char *method) {
  create_instruction(instruction, OP_SEND);
  instruction->label = strdup(method); // TODO: free this
}

void create_string_instruction(Instruction *instruction, char *method) {
  create_instruction(instruction, PUSH_STRING);
  instruction->label = strdup(method); // TODO: free this
}

void create_push_instruction(Instruction *instruction, enum op_code code, VALUE value) {
  create_instruction(instruction, code);
  instruction->value = value;
}

VALUE mochi_function_call(VM *vm, char *method, VALUE left, int n, ...) {
  VALUE *argv;
  va_list ar;
  if (n > 0) {
    long i;

    va_start(ar, n);

    argv = calloc(sizeof(VALUE), n);

    for (i = 0; i < n; i++) {
      argv[i] = va_arg(ar, VALUE);
    }
    va_end(ar);
  } else {
    argv = 0;
  }

  VALUE klass = mochi_get_class(vm, left);
  struct MMethod *current_method = ((struct MClass *) klass)->method;

  while(current_method && strcmp(current_method->name, method) != 0)
    current_method = current_method->method;

  if(current_method) {
    return current_method->c_func(vm, left, argv);
  }

  return (VALUE) 0;
}


Node *new_node(enum mochi_node_type type, size_t size, ...) {
  va_list args;
  va_start(args, size);

  Node *node = (Node *) malloc(sizeof(Node));
  node->type = type;

  switch(type) {
    case MOCHI_FUNCTION_CALL_NODE:
      node->u1.string = va_arg(args, char *);
      node->u2.node = va_arg(args, Node *);
      break;
    case MOCHI_EXPRESSION_NODE:
      node->u1.node = va_arg(args, Node *);
      break;
    case MOCHI_STRING_NODE:
      node->u1.string = va_arg(args, char *);
      break;
    case MOCHI_LITERAL_NODE:
      node->u1.value = va_arg(args, VALUE);
      break;
    case MOCHI_DONE_NODE:
      break;
  }

  va_end(args);

  return node;
}

void dump_node(Node *node) {
  switch(node->type) {
    case MOCHI_FUNCTION_CALL_NODE:
      printf("( FUNC_CALL<%s> ", node->u1.string);
      dump_node(node->u2.node);
      printf(")");
      break;
    case MOCHI_EXPRESSION_NODE:
      printf("( EXPRESSION_NODE [");
      if (node->u1.node != NULL) {
        dump_node(node->u1.node);
      }
      if (node->u3.node != NULL) {
        printf("AFTER \n");
        dump_node(node->u3.node);
      }
      printf("] )");
      break;
    case MOCHI_STRING_NODE:
      printf("( \"%s\" )", node->u1.string);
      break;
    case MOCHI_LITERAL_NODE:
      printf("%d", (int) (node->u1.value >> 1));
      break;
    case MOCHI_DONE_NODE:
      break;
  }
}

void mochi_run_node(Instruction **instructions, Node *node) {
  switch(node->type) {
    case MOCHI_FUNCTION_CALL_NODE:
      mochi_run_node(instructions, node->u2.node);
      create_send(*instructions, node->u1.string);
      *instructions = *instructions + 1;
      break;
    case MOCHI_EXPRESSION_NODE:
      mochi_run_node(instructions, node->u1.node);
      if (node->u3.node != NULL) {
        mochi_run_node(instructions, node->u3.node);
      }
      break;
    case MOCHI_STRING_NODE:
      create_string_instruction(*instructions, node->u1.string);
      *instructions = *instructions + 1;
      break;
    case MOCHI_DONE_NODE:
      create_instruction(*instructions, LEAVE);
      break;
    case MOCHI_LITERAL_NODE:
      create_push_instruction(*instructions, PUSH_LITERAL, node->u1.value);
      *instructions = *instructions + 1;
      break;
  }
}

Node *append_node(Node *parent, Node *child) {
  Node *tail = parent;
  while(tail->u3.node != NULL) tail = tail->u3.node;
  tail->u3.node = child;
  return parent;
}

int mochi_run(Node *node) {
  VM *vm = vm_init();
  Instruction *instructions = (Instruction *) malloc(sizeof(Instruction) * 5000);
  Instruction *head = instructions;
  mochi_run_node(&head, node);
  free_node(node);
  vm_eval(vm, instructions);
  vm_destroy(vm);
  return 0;
}

void free_node(Node *node) {
}

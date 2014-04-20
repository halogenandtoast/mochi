#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "mochi.h"
#include "vm.h"
#include "class.h"
#include "string.h"

VALUE mochi_function_call(VM *vm, char *method, VALUE left, VALUE right) {
  VALUE klass = mochi_get_class(vm, left);
  struct MMethod *current_method = ((struct MClass *) klass)->method;

  while(current_method && strcmp(current_method->name, method) != 0)
    current_method = current_method->method;

  if(current_method) {
    return current_method->c_func(vm, left);
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
  }
}

VALUE mochi_run_node(VM *vm, Node *node) {
  VALUE return_value;
  VALUE self;

  switch(node->type) {
    case MOCHI_FUNCTION_CALL_NODE:
      self = mochi_run_node(vm, node->u2.node);
      return_value = mochi_function_call(vm, node->u1.string, self, (VALUE) 0);
      break;
    case MOCHI_EXPRESSION_NODE:
      return_value = mochi_run_node(vm, node->u1.node);
      if(node->u3.node != NULL) {
        return_value = mochi_run_node(vm, node->u3.node);
      }
      break;
    case MOCHI_STRING_NODE:
      return_value = create_string(vm, node->u1.string);
      break;
    case MOCHI_LITERAL_NODE:
      return_value = node->u1.value;
      break;
  }
  return return_value;
}

Node *append_node(Node *parent, Node *child) {
  Node *tail = parent;
  while(tail->u3.node != NULL) tail = tail->u3.node;
  tail->u3.node = child;
  return parent;
}

int mochi_run(Node *node) {
  VM *vm = vm_init();
  mochi_run_node(vm, node);
  free_node(node);
  vm_destroy(vm);
  return 0;
}

void free_node(Node *node) {
}

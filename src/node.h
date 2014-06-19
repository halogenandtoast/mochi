#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>
#include <stdarg.h>
#include "vm.h"
#include "mochi.h"

enum mochi_node_type {
  MOCHI_FUNCTION_CALL_NODE,
#define MOCHI_FUNCTION_CALL_NODE MOCHI_FUNCTION_CALL_NODE
  MOCHI_EXPRESSION_NODE,
#define MOCHI_EXPRESSION_NODE MOCHI_EXPRESSION_NODE
  MOCHI_LITERAL_NODE,
#define MOCHI_LITERAL_NODE MOCHI_LITERAL_NODE
  MOCHI_STRING_NODE,
#define MOCHI_STRING_NODE MOCHI_STRING_NODE
  MOCHI_DONE_NODE,
#define MOCHI_DONE_NODE MOCHI_DONE_NODE
};

typedef struct Node {
  enum mochi_node_type type;
  union {
    char *string;
    int integer;
    struct Node *node;
    VALUE value;
  } u1;
  union {
    char *string;
    int integer;
    struct Node *node;
    VALUE value;
  } u2;
  union {
    char *string;
    int integer;
    struct Node *node;
    VALUE value;
  } u3;
} Node;

Node *new_node(enum mochi_node_type type, size_t size, ...);
Node *append_node(Node *parent, Node *child);
int mochi_run(Node *node);
void free_node(Node *node);
void dump_node(Node *node);
VALUE mochi_function_call(VM *vm, char *method, VALUE left, int len, ...);

#endif

#ifndef MOCHI_H
#define MOCHI_H

enum node_type {
  NODE_LIT,
#define NODE_LIT NODE_LIT
  NODE_CALL,
#define NODE_CALL NODE_CALL
  NODE_ARRAY
#define NODE_ARRAY NODE_ARRAY
};

#define NEW_NODE(type, value1, value2, value3) mochi_node_newnode(type, value1, value2, value3)
#define NEW_LIT(l) NEW_NODE(NODE_LIT,l,0,0)
#define NEW_ARRAY(a) NEW_NODE(NODE_ARRAY,a,0,0)
#define NEW_CALL(receiver, message, arguments) NEW_NODE(NODE_CALL, receiver, message, arguments)

typedef unsigned long ID;
typedef unsigned long VALUE;

typedef struct MNode {
  int value1;
  int value2;
  int value3;
} NODE;

#endif

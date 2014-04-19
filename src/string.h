#ifndef _MSTRING_H_
#define _MSTRING_H_

#include "class.h"

struct MString {
  struct MObjectHeader *header;
  char *content;
  int length;
};

void init_string(VM *vm);
VALUE create_string(VM *vm, char *content);

#endif

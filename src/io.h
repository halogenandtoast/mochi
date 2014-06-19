#ifndef _MIO_H_
#define _MIO_H_

#include "class.h"

struct MIO {
  struct MObjectHeader *header;
  int fd;
};

void init_io(VM *vm);
VALUE create_io(VM *vm, int fd);


#endif

#ifndef _MOCHI_H
#define _MOCHI_H

#define FIXNUM_FLAG 0x01
#define INT2FIX(i) ((VALUE)(i << 1 | FIXNUM_FLAG))

typedef unsigned long VALUE;

struct MObjectHeader {
  VALUE klass;
};

struct MObject {
  struct MObjectHeader *header;
};

#endif

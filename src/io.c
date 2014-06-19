#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "mochi.h"
#include "vm.h"
#include "io.h"

VALUE mochi_io_puts(VM *vm, VALUE self, VALUE *argv) {
  struct MIO *io = ((struct MIO *) self);
  char *cstr = ((struct MString *) *argv)->content;
  write(io->fd, cstr, strlen(cstr));
  write(io->fd, "\n", 1);
  return (VALUE) 0;
}

VALUE create_io(VM *vm, int fd) {
  struct MIO *new_io = (struct MIO *) malloc(sizeof(struct MIO));
  new_io->header = (struct MObjectHeader *) malloc(sizeof(struct MObjectHeader));
  new_io->header->klass = mochi_get_constant(vm, "IO");
  new_io->fd = fd;
  return (VALUE) new_io;
}

void init_io(VM *vm) {
  VALUE cIO = mochi_create_class(vm, "IO");
  mochi_define_method(cIO, "puts", mochi_io_puts);
}

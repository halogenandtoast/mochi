OBJS = src/mochi.tab.o \
			 src/lex.o \
			 src/main.o

all: mochi

src/mochi.tab.h: src/mochi.tab.c

src/mochi.tab.c: src/mochi.y
	yacc -d -o $@ $^

src/lex.c: src/lex.l src/mochi.tab.h
	lex -o $@ $^

%.o: %.c
	gcc -o $@ -c $<

mochi: $(OBJS)
	gcc -o $@ $(OBJS)

clean:
	@rm -f ${OBJS} mochi lex src/lex.c src/mochi.tab.c src/mochi.tab.h

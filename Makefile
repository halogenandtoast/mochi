SRC = src/main.c src/lex.yy.c src/y.tab.c src/node.c src/class.c src/vm.c src/integer.c src/string.c src/io.c
all: mochi

mochi: ${SRC}
	cc -ggdb -o mochi ${SRC}

src/lex.yy.c: src/mochi.l
	flex -o $@ $^

src/y.tab.c: src/parse.y
	bison -d -b src/y src/parse.y

clean:
	rm mochi src/y.tab.c src/y.tab.h src/lex.yy.c

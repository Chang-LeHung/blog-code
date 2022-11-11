

cfiles := $(wildcard *.c ./proc/*.c ./cextension/*.c)
cobjs  := $(patsubst %.c, %.out, $(cfiles))
cc     := clang
cflags := -g -lpthread
all: $(cobjs)


%.out: %.c

	$(cc) $(cflags) $< -o $@

pdf: ./proc/pstree.dot ./proc/pstree.out
	./proc/pstree.out > ./proc/pstree.dot
	dot -Tpdf ./proc/pstree.dot -o ./proc/pstree.pdf

.PHONY: clean

clean:
	rm $(cobjs)

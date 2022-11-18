

cfiles 	:= $(wildcard *.c management/*.c clone/*.c understandthread/*c specific/*c)
cobjs 	:= $(patsubst %.c, %.out, $(cfiles))
cflags 	:= -g -lpthread
cc 			:= clang
all: $(cobjs)



%.out: %.c
	$(cc) $< -o $@ $(cflags)

.PHONY: clean

clean:
	rm $(cobjs)

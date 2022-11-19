

cfiles 	:= $(wildcard *.c address/*c)
cobjs 	:= $(patsubst %.c, %.out, $(cfiles))
cflags 	:= -g -lpthread
cc 			:= clang
all: $(cobjs)



%.out: %.c
	$(cc) $< -o $@ $(cflags)

.PHONY: clean

clean:
	rm $(cobjs)

package main

import (
	"fmt"
	"syscall"
)

func main() {
	umask := 0
	umask = syscall.Umask(umask)
	fmt.Printf("0%o\n", umask)
	syscall.Umask(umask)
}

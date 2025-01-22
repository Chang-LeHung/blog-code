package main

import (
	"fmt"
	"os"
	"os/exec"
	"os/signal"
	"syscall"
)

const (
	EXIT_FAILURE = 1
	EXIT_SUCCESS = 0
)

func Fatal(msg string) {
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(EXIT_FAILURE)
}

func main() {
	args := os.Args[1:]
	signal.Ignore(syscall.SIGHUP)
	command := args[0]
	// get abs command
	absCommand, err := exec.LookPath(command)
	if err != nil {
		Fatal(err.Error())
	}
	command = absCommand
	fmt.Println(command, args)
	err = syscall.Exec(command, args, os.Environ())
	if err != nil {
		Fatal(err.Error())
	}
}

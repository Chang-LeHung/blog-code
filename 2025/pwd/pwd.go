package main

import (
	"fmt"
	"os"
	"runtime"
)

func Fatal(msg string) {
	fmt.Fprintf(os.Stderr, "%s\n", msg)
	os.Exit(1)
}

func AssertError(er error) {
	if er != nil {
		_, file, line, ok := runtime.Caller(1)
		if !ok {
			Fatal("Assertion failed: unknown caller")
		}
		Fatal(fmt.Sprintf("%s:%d: %s", file, line, er.Error()))
	}
}

func main() {
	// print pwd
	pwd, err := os.Getwd()
	AssertError(err)
	fmt.Println(pwd)
}

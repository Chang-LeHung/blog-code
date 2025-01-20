package main

import (
	"fmt"
	"os"
	"path/filepath"
	"runtime"
)

type CommandLine struct {
	Args   []string
	Dir    string
	Parent bool
}

const (
	EXIT_FAILURE = 1
	EXIT_SUCCESS = 0
)

func Fatal(msg string) {
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(EXIT_FAILURE)
}

func Assert(condition bool, msg string) {
	if !condition {
		_, file, line, ok := runtime.Caller(1)
		if !ok {
			Fatal("Assertion failed: unknown caller")
		}
		Fatal(fmt.Sprintf("%s:%d: %s", file, line, msg))
	}
}

func AssertUser(condition bool, msg string) {
	if !condition {
		Fatal(msg)
	}
}

func NewCommandLine(args []string) *CommandLine {
	return &CommandLine{Args: args}
}

func (c *CommandLine) Parse() {
	for ix := 0; ix < len(c.Args); ix++ {
		v := c.Args[ix]
		if v == "-p" {
			c.Parent = true
		} else if v[0] == '-' {
			Fatal("illegal option")
		} else {
			c.Dir = v
		}
	}
	AssertUser(c.Dir != "", "please input directory name")
}

func (c *CommandLine) DoMkdir() {
	if c.Parent {
		os.MkdirAll(c.Dir, 0755)
	} else {
		base := filepath.Dir(c.Dir)
		_, err := os.Stat(base)
		AssertUser(err == nil, fmt.Sprintf("%s is not directory", base))
		os.Mkdir(c.Dir, 0755)
	}
}

func main() {
	c := NewCommandLine(os.Args[1:])
	c.Parse()
	c.DoMkdir()
}

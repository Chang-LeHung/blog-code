package main

import (
	"fmt"
	"os"
	"runtime"
)

type CommandLine struct {
	Args      []string
	Recursive bool
	Filename  string
}

const (
	EXIT_FAILURE = 1
	EXIT_SUCCESS = 0
)

func Fatal(msg string) {
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(EXIT_FAILURE)
}

func NewCommandLine(args []string) *CommandLine {
	cmd := &CommandLine{Args: args}
	cmd.Parse()
	return cmd
}

func (c *CommandLine) Parse() {
	found := false
	for ix := 0; ix < len(c.Args); ix++ {
		v := c.Args[ix]
		if v == "-r" {
			c.Recursive = true
		} else if v[0] == '-' {
			Fatal("illegal option")
		} else {
			if found {
				Fatal("duplicate file name")
			}
			c.Filename = v
			found = true
		}
	}
	if !found {
		Fatal("please input file name")
	}
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

func AssertError(err error) {
	if err != nil {
		_, file, line, ok := runtime.Caller(1)
		if !ok {
			Fatal("Assertion failed: unknown caller")
		}
		Fatal(fmt.Sprintf("%s:%d: %s", file, line, err.Error()))
	}
}

func (c *CommandLine) DoRemove() {
	f, err := os.Open(c.Filename)
	if err != nil {
		Fatal(err.Error())
	}
	// f is dir or regular
	defer f.Close()
	stat, err := f.Stat()
	AssertError(err)
	if stat.IsDir() {
		if c.Recursive {
			os.RemoveAll(c.Filename)
		} else {
			// rm
			Fatal("rm: cannot remove '" + c.Filename + "': Is a directory")
		}
	} else {
		os.Remove(c.Filename)
	}
}

func main() {
	c := NewCommandLine(os.Args[1:])
	c.DoRemove()
}

package main

import (
	"fmt"
	"os"
	"regexp"
	"runtime"
	"strconv"
	"strings"
	"syscall"
)

type CommandLine struct {
	Args        []string
	Pattern     string
	Insensitive bool
	ListProcess bool
	FullMode    bool
	Sig         int
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

func AssertError(err error) {
	if err != nil {
		_, file, line, ok := runtime.Caller(1)
		if !ok {
			Fatal("Assertion failed: unknown caller")
		}
		Fatal(fmt.Sprintf("%s:%d: %s", file, line, err.Error()))
	}
}

func NewCommandLine(args []string) *CommandLine {
	return &CommandLine{
		Args: args,
	}
}

func (c *CommandLine) Parse() {
	patternFound := false
	sigFound := false
	for ix := 0; ix < len(c.Args); ix++ {
		v := c.Args[ix]
		if v == "-i" {
			c.Insensitive = true
		} else if v == "-l" {
			c.ListProcess = true
		} else if v == "-f" {
			Assert(ix+1 < len(c.Args), "Assertion failed: -f option, require pattern")
			c.Pattern = c.Args[ix+1]
			ix++
			c.FullMode = true
		} else if v[0] == '-' {
			rest := v[1:]
			singal, err := strconv.Atoi(rest)
			Assert(err == nil, "Assertion failed: illgeal signal number")
			if sigFound {
				Fatal("Duplicate signal number")
			}
			c.Sig = singal
		} else {
			if patternFound {
				Fatal("Duplicate pattern")
			}
			patternFound = true
			c.Pattern = v
		}
	}
}

func (c *CommandLine) DoKill() {
	base := "/proc"
	files, err := os.ReadDir(base)
	AssertError(err)
	for _, file := range files {
		if file.IsDir() {
			pid, err := strconv.Atoi(file.Name())
			if err != nil {
				// none process dir
				continue
			}
			filename := fmt.Sprintf("/proc/%s/cmdline", file.Name())
			handle, err := os.ReadFile(filename)
			AssertError(err)
			cmd := string(handle)
			args := strings.Split(cmd, "\x00")
			if c.FullMode {
				if c.Insensitive {
					c.Pattern = strings.ToLower(c.Pattern)
					args[0] = strings.ToLower(args[0])
				}
				if cmd == c.Pattern {
					AssertError(err)
					syscall.Kill(pid, syscall.Signal(c.Sig))
					if c.ListProcess {
						fmt.Printf("Killed: %d\t%s\n", pid, cmd)
					}
				}
			} else {
				matched, err := regexp.MatchString(c.Pattern, args[0])
				AssertError(err)
				if matched {
					syscall.Kill(pid, syscall.Signal(c.Sig))
					if c.ListProcess {
						fmt.Printf("Killed: %d\t%s\n", pid, cmd)
					}
				}
			}
		}
	}
}

func main() {
	Assert(runtime.GOOS == "linux", fmt.Sprintf("pkill is not supported on %s", runtime.GOOS))
	c := NewCommandLine(os.Args[1:])
	c.Parse()
	c.DoKill()
}

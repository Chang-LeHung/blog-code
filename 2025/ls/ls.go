package main

import (
	"fmt"
	"os"
	"strings"
)

type CommandLine struct {
	Command   string
	Params    []string
	Remainder []string
	AllInfo   bool
	List      bool
}

type CmdVec interface {
	GetCommand() string
	ContaineArgs(string) bool
}

func (c *CommandLine) ContaineArgs(s string) bool {
	for _, v := range c.Params {
		if v == s {
			return true
		}
	}
	return false
}

func (c *CommandLine) GetCommand() string {
	return c.Command
}

func (c *CommandLine) Parse() {
	for idx, val := range c.Params {
		if strings.HasPrefix(val, "-") {
			rest := c.Params[idx][1:]
			for _, ch := range rest {
				switch ch {
				case 'a':
					c.AllInfo = true
				case 'l':
					c.List = true
				default:
					panic(fmt.Sprintf("unknown option: %c, in %s\n", ch, rest))
				}
			}
			continue
		}
		c.Remainder = c.Params[idx:]
		break
	}
}

type CmdFile struct {
	Cmd   *CommandLine
	Files []os.FileInfo
}

func (f *CmdFile) Dump() {
}

func main() {
	args := os.Args
	c := &CommandLine{
		Command: args[0],
		Params:  args[1:],
	}
	c.Parse()
	if c.ContaineArgs("-h") || c.ContaineArgs("--help") {
		fmt.Println("Usage: ls [OPTION]... [FILE]...")
		os.Exit(0)
	}
}

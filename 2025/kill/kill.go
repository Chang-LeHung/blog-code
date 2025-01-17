package main

import (
	"fmt"
	"os"
	"strconv"
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

type CommandList struct {
	Args    []string
	Pids    []int32
	Signals []int
}

func NewCommandList(args []string) *CommandList {
	return &CommandList{
		Args: args,
	}
}

func (c *CommandList) Parse() {
	for ix := 0; ix < len(c.Args); ix++ {
		arg := c.Args[ix]
		if arg == "-s" {
			continue
		} else if arg[0] == '-' {
			pid, err := strconv.Atoi(arg[1:])
			if err != nil {
				Fatal(fmt.Sprintf("%s can not be translated to int32", arg))
			}
			c.Signals = append(c.Signals, pid)
		} else {
			for ; ix < len(c.Args); ix++ {
				pid, err := strconv.Atoi(arg)
				if err != nil {
					Fatal(fmt.Sprintf("%s can not be translated to int32", arg))
				}
				c.Pids = append(c.Pids, int32(pid))
			}
		}
	}
}

func (c *CommandList) ContaineArgs(arg string) bool {
	for _, v := range c.Args {
		if v == arg {
			return true
		}
	}
	return false
}

func (c *CommandList) DoKill() {
	for _, pid := range c.Pids {
		for _, sig := range c.Signals {
			fmt.Printf("kill -%d %d\n", sig, pid)
			err := syscall.Kill(int(pid), syscall.Signal(sig))
			if err != nil {
				Fatal(fmt.Sprintf("kill %d failed, err=%v", pid, err))
			}
		}
	}
}

func main() {
	c := NewCommandList(os.Args[1:])
	if c.ContaineArgs("-h") || c.ContaineArgs("--help") {
		fmt.Println("Usage: kill [OPTION]... PID...")
		os.Exit(EXIT_SUCCESS)
	}
	c.Parse()
	c.DoKill()
}

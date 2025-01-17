package main

import (
	"bufio"
	"fmt"
	"os"
)

const (
	EXIT_FAILURE = 1
	EXIT_SUCCESS = 0
)

type CommandLine struct {
	Args    []string
	LineNum bool
	Files   []string
}

func Fatal(msg string) {
	_, _ = fmt.Fprintf(os.Stderr, "%s", msg)
	os.Exit(EXIT_FAILURE)
}

func NewCommandLine(args []string) *CommandLine {
	return &CommandLine{
		Args: args,
	}
}

func (c *CommandLine) Parse() {
	for _, arg := range c.Args {
		if arg == "-n" {
			c.LineNum = true
		} else if arg[0] == '-' {
			Fatal("Usage: cat [-n] [FILE]...")
		} else {
			c.Files = append(c.Files, arg)
		}
	}
}

func (c *CommandLine) ContaineArgs(s string) bool {
	for _, v := range c.Args {
		if v == s {
			return true
		}
	}
	return false
}

func (c *CommandLine) Print() {
	for _, file := range c.Files {
		fi, err := os.Stat(file)
		if err != nil {
			Fatal(err.Error())
		}
		if fi.Mode().IsRegular() {
			no := 1
			f, err := os.Open(file)
			if err != nil {
				Fatal(err.Error())

			}
			defer f.Close()
			scanner := bufio.NewScanner(f)
			for scanner.Scan() {
				line := scanner.Text()
				if c.LineNum {
					fmt.Printf("%d\t%s\n", no, line)
					no++
				} else {
					fmt.Println(line)
				}
			}
		} else {
			Fatal(fmt.Sprintf("%s is not a regular file", fi.Name()))
		}
	}
}

func main() {
	c := NewCommandLine(os.Args[1:])
	if c.ContaineArgs("-h") || c.ContaineArgs("--help") {
		fmt.Println("Usage: cat [OPTION]... [FILE]...")
		os.Exit(EXIT_SUCCESS)
	}
	c.Parse()
	c.Print()
}

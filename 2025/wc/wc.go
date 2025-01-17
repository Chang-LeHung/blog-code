package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

const (
	EXIT_SUCCESS = 0
	EXIT_FAILURE = 1
)

func Fatal(msg string) {
	fmt.Fprintln(os.Stderr, msg)
	os.Exit(EXIT_FAILURE)
}

type Data struct {
	Lines int
	Words int
	Bytes int
}

type CommandLine struct {
	Args    []string
	Files   []string
	ByteCnt bool
	WordCnt bool
	LineCnt bool
}

func NewCommandLine(args []string) *CommandLine {
	return &CommandLine{
		Args: args,
	}
}

func (c *CommandLine) ContaineArgs(arg string) bool {
	for _, v := range c.Args {
		if v == arg {
			return true
		}
	}
	return false
}

func (c *CommandLine) Parse() {
	first := true
	for _, arg := range c.Args {
		if arg[0] == '-' {
			switch arg {
			case "-c":
				c.ByteCnt = true
				if first {
					first = false
					c.WordCnt = false
					c.LineCnt = false
				}
			case "-w":
				c.WordCnt = true
				if first {
					first = false
					c.ByteCnt = false
					c.LineCnt = false
				}
			case "-l":
				c.LineCnt = true
				if first {
					first = false
					c.ByteCnt = false
					c.WordCnt = false
				}
			default:
				Fatal("Usage: wc [-cwl] [FILE]...")
			}
		} else {
			c.Files = append(c.Files, arg)
		}
	}
	if first {
		c.ByteCnt = true
		c.WordCnt = true
		c.LineCnt = true
	}
}

func (c *CommandLine) DoCountAndPrintRes() {
	if len(c.Files) == 0 {
		scanner := bufio.NewScanner(os.Stdin)
		bc, wordc, lines := GetStatistic(scanner)
		fmt.Printf("%d %d %d\n", lines, wordc, bc)
	} else {
		for _, v := range c.Files {
			stat, err := os.Stat(v)
			// do pre-checks
			if err != nil {
				Fatal(err.Error())
			}
			if !stat.Mode().IsRegular() {
				Fatal(fmt.Sprintf("%s is not a regular file", stat.Name()))
			}

			f, err := os.Open(v)
			if err != nil {
				Fatal(err.Error())
			}
			defer f.Close()
			scanner := bufio.NewScanner(f)
			bc, wordc, lines := GetStatistic(scanner)
			s := ""
			if c.LineCnt {
				s += fmt.Sprintf("%d ", lines)
			}
			if c.WordCnt {
				s += fmt.Sprintf("%d ", wordc)
			}
			if c.ByteCnt {
				s += fmt.Sprintf("%d ", bc)
			}
			s += v
			fmt.Println(s)
		}
	}
}

func GetStatistic(scanner *bufio.Scanner) (int, int, int) {
	bc := 0
	wordc := 0
	lines := 0
	for scanner.Scan() {
		lines++
		s := scanner.Text()
		bc += len(s) + 1 // newline require a char
		wordc += func() int {
			words := strings.Fields(s)
			cnt := 0
			for _, s := range words {
				if s != "" {
					cnt++
				}
			}
			return cnt
		}()
	}
	return bc, wordc, lines
}

func main() {
	c := NewCommandLine(os.Args[1:])
	if c.ContaineArgs("-h") || c.ContaineArgs("--help") {
		fmt.Println("Usage: wc [OPTION]... [FILE]...")
		os.Exit(EXIT_SUCCESS)
	}
	c.Parse()
	c.DoCountAndPrintRes()
}

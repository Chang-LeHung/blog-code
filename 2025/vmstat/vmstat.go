package main

import (
	"fmt"
	"os"
	"regexp"
	"runtime"
	"strconv"
	"strings"

	"github.com/olekukonko/tablewriter"
)

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

type CommandLine struct {
	Args     []string
	Free     int
	Buffer   int
	Cache    int
	Avaiable int
	Loaded   bool
}

func NewCommandLine(args []string) *CommandLine {
	return &CommandLine{Args: args}
}

func (c *CommandLine) Parse() {
}

func (c *CommandLine) GetMemStatistic() {
	if c.Loaded {
		return
	}
	c.loadMemInfo()
	c.loadHardWareInfo()
	c.Loaded = true
}

func (c *CommandLine) loadHardWareInfo() {

}

func (c *CommandLine) loadMemInfo() {
	filename := "/proc/meminfo"
	file, err := os.ReadFile(filename)
	AssertError(err)
	content := string(file)
	re := regexp.MustCompile(`\d+`)
	for _, line := range strings.Split(content, "\n") {
		if strings.HasPrefix(line, "Buffers:") {
			data := re.FindAll([]byte(line), -1)
			Assert(len(data) == 1, "Buffers: line not found")
			c.Buffer, err = strconv.Atoi(string(data[0]))
			AssertError(err)
		} else if strings.HasPrefix(line, "MemFree:") {
			data := re.FindAll([]byte(line), -1)
			Assert(len(data) == 1, "Buffers: line not found")
			c.Free, err = strconv.Atoi(string(data[0]))
			AssertError(err)
		} else if strings.HasPrefix(line, "SReclaimable:") || strings.HasPrefix(line, "Cached:") {
			data := re.FindAll([]byte(line), -1)
			Assert(len(data) == 1, "Buffers: line not found")
			val, err := strconv.Atoi(string(data[0]))
			AssertError(err)
			c.Cache += val // accumulated cache
		} else if strings.HasPrefix(line, "MemAvailable:") {
			data := re.FindAll([]byte(line), -1)
			Assert(len(data) == 1, "Buffers: line not found")
			val, err := strconv.Atoi(string(data[0]))
			AssertError(err)
			c.Avaiable = val
		}
	}
	Assert(c.Buffer != 0, "Buffers is zero")
	Assert(c.Free != 0, "MemFree is zero")
	Assert(c.Cache != 0, "Cached is zero")
	Assert(c.Avaiable != 0, "MemAvailable is zero")
}

func (c *CommandLine) Print() {
	c.GetMemStatistic()
	table := tablewriter.NewWriter(os.Stdout)
	table.SetHeader([]string{"Buffer", "Free", "Cache", "available"})
	table.Append([]string{
		strconv.Itoa(c.Buffer/1024) + " mB",
		strconv.Itoa(c.Free/1024) + " mB",
		strconv.Itoa(c.Cache/1024) + " mB",
		strconv.Itoa(c.Avaiable/1024) + " mB",
	})
	table.Render()
}

func main() {
	Assert(runtime.GOOS == "linux", fmt.Sprintf("vmstat is not supported on %s", runtime.GOOS))
	c := NewCommandLine(os.Args[1:])
	c.Parse()
	c.Print()
}

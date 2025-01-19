package main

import (
	"fmt"
	"os"
	"regexp"
	"runtime"
	"strconv"
	"strings"
	"time"

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
	Args       []string
	Free       int
	Buffer     int
	Cache      int
	Avaiable   int
	Loaded     bool
	Running    int
	Blocked    int
	Interrupts int
	BootTime   int
	in         int // interrupts per second
	cs         int // context switch
	us         int // user mode
	sy         int // system mode
	id         int // idle
	wa         int // waiting for disk
	st         int // stolen time
	gu         int // gust user mode
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

func Str2Int(val string) int {
	i, err := strconv.Atoi(val)
	AssertError(err)
	return i
}

func (c *CommandLine) loadHardWareInfo() {
	filename := "/proc/stat"
	data, err := os.ReadFile(filename)
	AssertError(err)
	content := string(data)
	for _, line := range strings.Split(content, "\n") {
		if len(line) == 0 {
			continue
		}
		fileds := strings.Fields(line)
		Assert(len(fileds) > 0, fmt.Sprintf("Invalid line, fileds:%v, line:%v", fileds, line))
		if fileds[0] == "cpu" {
			c.us = Str2Int(fileds[1])
			c.sy = Str2Int(fileds[3])
			c.id = Str2Int(fileds[4])
			c.wa = Str2Int(fileds[5])
			c.st = Str2Int(fileds[8])
			c.gu = Str2Int(fileds[10])
		} else if fileds[0] == "ctxt" {
			c.cs = Str2Int(fileds[1])
		} else if fileds[0] == "procs_running" {
			c.Running = Str2Int(fileds[1])
		} else if fileds[0] == "procs_blocked" {
			c.Blocked = Str2Int(fileds[1])
		} else if fileds[0] == "intr" {
			c.Interrupts = Str2Int(fileds[1])
		} else if fileds[0] == "btime" {
			c.BootTime = Str2Int(fileds[1])
		}
	}
	elpased := (int(time.Now().Unix()) - c.BootTime)
	c.in = c.Interrupts / elpased
	c.cs /= elpased
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
	table.SetHeader([]string{"Name", "value"})
	table.AppendBulk(
		[][]string{
			[]string{"Free", fmt.Sprintf("%d kb", c.Free)},
			[]string{"Cache", fmt.Sprintf("%d kb", c.Cache)},
			[]string{"Avaiable", fmt.Sprintf("%d kb", c.Avaiable)},
			[]string{"Buffer", fmt.Sprintf("%d kb", c.Buffer)},
			[]string{"Running", fmt.Sprintf("%d", c.Running)},
			[]string{"Blocked", fmt.Sprintf("%d", c.Blocked)},
			[]string{"in", fmt.Sprintf("%d", c.in)},
			[]string{"cs", fmt.Sprintf("%d", c.cs)},
			[]string{"us", fmt.Sprintf("%d", c.us)},
			[]string{"sy", fmt.Sprintf("%d", c.sy)},
			[]string{"wa", fmt.Sprintf("%d", c.wa)},
			[]string{"st", fmt.Sprintf("%d", c.st)},
			[]string{"gu", fmt.Sprintf("%d", c.gu)},
		})
	table.Render()
}

func main() {
	Assert(runtime.GOOS == "linux", fmt.Sprintf("vmstat is not supported on %s", runtime.GOOS))
	c := NewCommandLine(os.Args[1:])
	c.Parse()
	c.Print()
}

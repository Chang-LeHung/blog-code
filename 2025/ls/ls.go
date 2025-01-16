package main

import (
	"fmt"
	"os"
	"os/user"
	"sort"
	"strconv"
	"strings"
	"syscall"
)

const (
	EXIT_SUCCESS = 0
	EXIT_FAILURE = -1
)

func Fatal(msg string) {
	_, _ = fmt.Fprintf(os.Stderr, "%s", msg)
	os.Exit(EXIT_FAILURE)
}

type CommandLine struct {
	Command   string
	Params    []string
	Remainder []string
	AllInfo   bool
	List      bool
	Sorted    bool
	Reverse   bool
	Human     bool
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
				case 's':
					c.Sorted = true
				case 'S':
					c.Reverse = true
					c.Sorted = true
				case 'h':
					c.Human = true
				default:
					Fatal(fmt.Sprintf("unknown option: %c, in %s\n", ch, rest))
				}
			}
			continue
		}
		c.Remainder = c.Params[idx:]
		break
	}
}

type CmdFile struct {
	Cmd    *CommandLine
	Files  []os.FileInfo
	Lines  []string
	Parsed bool
}

func (c *CmdFile) DoListFiles() {
	files := make([]string, 0)
	if len(c.Cmd.Remainder) == 0 {
		files = append(files, ".")
	} else {
		files = c.Cmd.Remainder
	}
	for idx := 0; idx < len(files); idx++ {
		file := files[idx]
		fi, err := os.Stat(file)
		if err != nil {
			Fatal(fmt.Sprintf("ls: %s: %s\n", file, err))
		}
		if fi.IsDir() {
			c.Files = append(c.Files, GetFileInfos(file)...)
		} else {
			c.Files = append(c.Files, fi)
		}
	}
}

func (c *CmdFile) String() string {
	if c.Parsed {
		return strings.Join(c.Lines, "\n")
	}
	if c.Cmd.Sorted {
		if c.Cmd.Reverse {
			sort.Slice(c.Files, func(i, j int) bool {
				return c.Files[i].Size() > c.Files[j].Size()
			})
		} else {
			sort.Slice(c.Files, func(i, j int) bool {
				return c.Files[i].Size() < c.Files[j].Size()
			})
		}
	}
	for _, f := range c.Files {
		if !c.Cmd.AllInfo && f.Name() == "." && f.Name() == ".." {
			// ignore . and ..
			continue
		}
		c.Lines = append(c.Lines, c.ParseFile(f))
	}
	c.Parsed = true
	return c.String()
}

func (c *CmdFile) ParseFile(f os.FileInfo) string {
	if !c.Cmd.List {
		return f.Name()
	}
	var res string
	// parse file type
	switch {
	case f.Mode().IsRegular():
		res += "-"
	case f.Mode().IsDir():
		res += "d"
	default:
		m := f.Mode()
		if m&os.ModeSymlink != 0 {
			res += "l"
		} else if m&os.ModeNamedPipe != 0 {
			res += "p"
		} else if m&os.ModeSocket != 0 {
			res += "s"
		} else if m&os.ModeDevice != 0 {
			res += "b"
		} else if m&os.ModeCharDevice != 0 {
			res += "c"
		} else {
			res += "?"
		}
	}
	perm := f.Mode().Perm()
	perms := "rwx"
	for i := 8; i >= 0; i-- {
		if perm&(1<<i) != 0 {
			res += string(perms[i%3])
		} else {
			res += "-"
		}
	}
	res += "\t"
	// get file hard link count
	res += fmt.Sprintf("%d\t", f.Sys().(*syscall.Stat_t).Nlink)
	// get user and group
	uid := f.Sys().(*syscall.Stat_t).Uid
	gid := f.Sys().(*syscall.Stat_t).Gid
	res += fmt.Sprintf("%s\t%s\t", GetUserName(int(uid)), GetGroupName(int(gid)))
	res += fmt.Sprintf("%s\t%s\t", GetFileSize(int(f.Size()), c.Cmd.Human),
		f.ModTime().Format("2006-01-02 15:04:05"))
	res += f.Name()
	if f.Mode()&os.ModeSymlink != 0 {
		link, err := os.Readlink(f.Name())
		if err != nil {
			Fatal(fmt.Sprintf("ls: %s: %s\n", f.Name(), err))
		}
		res += fmt.Sprintf(" -> %s", link)
	}
	return res
}

func GetFileSize(val int, h bool) string {
	if !h {
		return strconv.Itoa(val)
	}
	var unit string
	var size float32
	if val < 1024 {
		unit = "B"
		size = float32(val)
	} else if val < 1024*1024 {
		unit = "K"
		size = float32(val / 1024)
	} else if val < 1024*1024*1024 {
		unit = "M"
		size = float32(val / 1024 / 1024)
	} else {
		unit = "G"
		size = float32(val / 1024 / 1024 / 1024)
	}
	return fmt.Sprintf("%.1f%s", size, unit)
}

func GetUserName(uid int) string {
	u, err := user.LookupId(strconv.Itoa(uid))
	if err != nil {
		Fatal(err.Error())
	}
	return u.Username
}

func GetGroupName(gid int) string {
	group, err := user.LookupGroupId(strconv.Itoa(gid))
	if err != nil {
		Fatal(err.Error())
	}
	return group.Name
}

func GetFileInfos(dir string) []os.FileInfo {
	files, err := os.ReadDir(dir)
	if err != nil {
		Fatal(fmt.Sprintf("ls: %s: %s\n", dir, err))
	}
	var res []os.FileInfo
	for _, file := range files {
		fi, err := file.Info()
		if err != nil {
			Fatal(fmt.Sprintf("ls: %s: %s\n", file, err))
		}
		res = append(res, fi)
	}
	return res
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
	cmd := &CmdFile{
		Cmd: c,
	}
	cmd.DoListFiles()
	fmt.Println(cmd.String())
}

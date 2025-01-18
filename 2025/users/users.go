package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"strings"
)

// Utmp 记录的结构体映射
type Utmp struct {
	Type   int16
	PID    int32
	Device [32]byte
	User   [32]byte
	ID     [4]byte
	Line   [32]byte
	Addr   [4]byte
	Time   int32
	Host   [256]byte
}

// 常见的记录类型
const (
	USER_LOGIN = 7
)

// 读取 /var/run/utmp 文件并打印登录的用户信息
func readUtmp() ([]string, error) {
	// 打开 /var/run/utmp 文件
	file, err := os.Open("/var/run/utmp")
	if err != nil {
		return nil, fmt.Errorf("failed to open utmp file: %v", err)
	}
	defer file.Close()

	// 存储登录的用户名
	var users []string

	// 按照 Utmp 记录的大小读取数据
	recordSize := int64(binary.Size(Utmp{}))
	var utmpRecord Utmp

	for {
		// 从文件中读取一条记录
		err := binary.Read(file, binary.LittleEndian, &utmpRecord)
		if err != nil {
			// 结束文件读取
			if err.Error() == "EOF" {
				break
			}
			return nil, fmt.Errorf("failed to read utmp record: %v", err)
		}

		// 判断记录是否是用户登录类型
		if utmpRecord.Type == USER_LOGIN {
			// 提取用户名并去掉空格
			username := strings.TrimSpace(string(utmpRecord.User[:]))
			if username != "" {
				users = append(users, username)
			}
		}

		// 跳到下一个记录
		_, err = file.Seek(recordSize, os.SEEK_CUR)
		if err != nil {
			return nil, fmt.Errorf("failed to seek file: %v", err)
		}
	}

	return users, nil
}

func main() {
	// 读取 /var/run/utmp 中的所有登录用户
	users, err := readUtmp()
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	// 打印当前登录的用户
	fmt.Println("Current logged in users:")
	for _, user := range users {
		fmt.Println(user)
	}
}

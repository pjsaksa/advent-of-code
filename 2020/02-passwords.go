package main

import (
	"fmt"
	"os"
	"strings"
)

type Password struct {
	i1, i2 int
	ch byte
	password string
}

type Passwords []Password

func (passwords *Passwords) ReadInput(fileName string) {
	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	var item Password

	for {
		if count,err := fmt.Fscanf(file, "%d-%d %c: %s", &item.i1, &item.i2, &item.ch, &item.password);
		count == 4 && err == nil {
			*passwords = append(*passwords, item)
		} else {
			break
		}
	}
}

func (pw *Password) Valid_a() (bool) {
	count := strings.Count(pw.password, string(pw.ch))
	return count >= pw.i1 && count <= pw.i2
}

func (pw *Password) Valid_b() (bool) {
	check1 := pw.password[pw.i1 - 1] == pw.ch
	check2 := pw.password[pw.i2 - 1] == pw.ch

	return (check1 && !check2) || (!check1 && check2)
}

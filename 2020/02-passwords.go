package main

import (
	"fmt"
	"io"
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
	const (
		ReadChunk = 50
	)

	//

	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	var chunk [ReadChunk]Password

	for {
		for i := 0; i < ReadChunk; i++ {
			_,err := fmt.Fscanf(file, "%d-%d %c: %s", &chunk[i].i1, &chunk[i].i2, &chunk[i].ch, &chunk[i].password)

			if err == io.EOF {
				*passwords = append(*passwords, chunk[0:i]...)
				return
			} else if err != nil {
				panic(err)
			}
		}

		*passwords = append(*passwords, chunk[0:ReadChunk]...)
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

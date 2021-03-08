package main

import (
	"bufio"
	"os"
	"strings"
)

type TreeMap []string

func (tmap *TreeMap) ReadInput(fileName string) {
	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	in := bufio.NewReader(file)

	var item string

	for {
		if item,err = in.ReadString('\n')
		err == nil {
			*tmap = append(*tmap, strings.TrimSuffix(item, string('\n')))
		} else {
			break
		}
	}
}

func (tmap *TreeMap) Hit(x, y int) (bool) {
	width := len((*tmap)[0])

	return (*tmap)[y][x % width] == '#'
}

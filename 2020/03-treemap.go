package main

import (
	"bufio"
	"io"
	"os"
	"strings"
)

type TreeMap []string

func (tmap *TreeMap) ReadInput(fileName string) {
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

	in := bufio.NewReader(file)

	var chunk [ReadChunk]string

	for {
		for i := 0; i < ReadChunk; i++ {
			chunk[i],err = in.ReadString('\n')

			if err == io.EOF {
				*tmap = append(*tmap, chunk[0:i]...)
				return
			} else if err != nil {
				panic(err)
			}

			chunk[i] = strings.TrimSuffix(chunk[i], string('\n'))
		}

		*tmap = append(*tmap, chunk[0:ReadChunk]...)
	}
}

func (tmap *TreeMap) Hit(x, y int) (bool) {
	width := len((*tmap)[0])

	return (*tmap)[y][x % width] == '#'
}

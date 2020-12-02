package main

import (
	"fmt"
	"io"
	"os"
)

type Entries []int

func (entries *Entries) ReadInput(fileName string) {
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

	var chunk [ReadChunk]int

	for {
		for i := 0; i < ReadChunk; i++ {
			_,err := fmt.Fscanln(file, &chunk[i])

			if err == io.EOF {
				*entries = append(*entries, chunk[0:i]...)
				return
			} else if err != nil {
				panic(err)
			}
		}

		*entries = append(*entries, chunk[0:ReadChunk]...)
	}
}

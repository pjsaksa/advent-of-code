package main

import (
	"fmt"
	"io"
	"os"
	"sort"
)

type Entries []int

func (entries *Entries) readInput(fileName string) {
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

func main() {
	var entries Entries

	entries.readInput("input-01.txt")

	if len(entries) == 0 {
		panic("empty input")
	}

	begin := 0
	end := len(entries) - 1

	sort.Ints(entries)

	for begin < end {
		sum := entries[begin] + entries[end]

		if sum == 2020 {
			break
		} else if sum > 2020 {
			end--
		} else {
			begin++
		}
	}

	if begin >= end {
		fmt.Printf("No match found\n")
		os.Exit(1)
	}

	fmt.Printf("Match: %v + %v = %v\n", entries[begin], entries[end], entries[begin] + entries[end])
	fmt.Printf("Match: %v * %v = %v\n", entries[begin], entries[end], entries[begin] * entries[end])
}

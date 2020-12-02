package main

import (
	"fmt"
	"os"
	"sort"
)

func main() {
	var entries Entries

	entries.ReadInput("input-01.txt")

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

	fmt.Println("Match:")
	fmt.Println("-", entries[begin], "+", entries[end], "=", entries[begin] + entries[end])
	fmt.Println("-", entries[begin], "+", entries[end], "=", entries[begin] * entries[end])
}

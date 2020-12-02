package main

import (
	"fmt"
	"os"
)

func findTriplet(entries *Entries) (int, int, int) {
	for i := 0; i < len(*entries); i++ {
		for j := i+1; j < len(*entries); j++ {
			for k := j+1; k < len(*entries); k++ {
				sum := (*entries)[i] + (*entries)[j] + (*entries)[k]

				if sum == 2020 {
					return (*entries)[i], (*entries)[j], (*entries)[k]
				}
			}
		}
	}
	return 0,0,0
}

func main() {
	var entries Entries

	entries.ReadInput("input-01.txt")

	if len(entries) == 0 {
		panic("empty input")
	}

	a,b,c := findTriplet(&entries)

	if a == 0 || b == 0 || c == 0 {
		fmt.Printf("No match found\n")
		os.Exit(1)
	}

	fmt.Println("Match:")
	fmt.Println("-", a, "+", b, "+", c, "=", a + b + c)
	fmt.Println("-", a, "*", b, "*", c, "=", a * b * c)
}

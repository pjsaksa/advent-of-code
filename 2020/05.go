package main

import (
	"fmt"
	"io"
	"os"
	"sort"
)

type Seats []int

func main() {
	var seats Seats
	seats.ReadInput("input-05.txt")

	min := seats[0]
	max := seats[len(seats)-1]
	fmt.Println("There are", len(seats), "seats and the numbers go from", min, "to", max)

	last := min - 1
	for _,v := range seats {
		if v != last + 1 {
			fmt.Println("Missing seat", last + 1)
		}

		last = v
	}
}

func (seats *Seats) ReadInput(fileName string) {
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
	var in string

	for {
		for i := 0; i < ReadChunk; i++ {
			_,err := fmt.Fscanln(file, &in)

			chunk[i] = convertFromBSP(in)

			if err == io.EOF {
				*seats = append(*seats, chunk[0:i]...)

				sort.Slice(*seats, func(a, b int) bool { return (*seats)[a] < (*seats)[b] })

				return
			} else if err != nil {
				panic(err)
			}
		}

		*seats = append(*seats, chunk[0:ReadChunk]...)
	}
}

func convertFromBSP(in string) (total int) {
	for _,ch := range in {
		total *= 2

		switch ch {
		case 'B': fallthrough
		case 'R': total++
		}
	}
	return
}

package main

import (
	"fmt"
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
	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	var in string

	for {
		if count,err := fmt.Fscanln(file, &in)
		count == 1 && err == nil {
			*seats = append(*seats, convertFromBSP(in))
		} else {
			sort.Slice(*seats, func(a, b int) bool { return (*seats)[a] < (*seats)[b] })
			break
		}
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

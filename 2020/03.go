package main

import (
	"fmt"
)

func main() {
	var tmap TreeMap

	tmap.ReadInput("input-03.txt")

	fmt.Printf("Map of trees has %d rows and %d columns\n", len(tmap), len(tmap[0]))

	mult := 1

	mult *= CheckSlope(&tmap, 1, 1)
	mult *= CheckSlope(&tmap, 1, 3)
	mult *= CheckSlope(&tmap, 1, 5)
	mult *= CheckSlope(&tmap, 1, 7)
	mult *= CheckSlope(&tmap, 2, 1)

	fmt.Println("Multiplication of hits:", mult)
}

func CheckSlope(tmap *TreeMap, slopeX, slopeY int) int {
	x,y := 0,0
	hits := 0

	for y < len(*tmap) {
		if (*tmap).Hit(x, y) {
			hits++
		}

		y += slopeX
		x += slopeY
	}

	fmt.Printf("With slope %d:%d, there are %d hits\n", slopeX, slopeY, hits)
	return hits
}

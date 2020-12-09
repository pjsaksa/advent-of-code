package main

import (
	"fmt"
)

func main() {
	var passports Passports

	passports.ReadInput("input-04.txt")

	fmt.Println("Total number of passports:", len(passports))

	// a

	{
		valid := 0
		for _,pp := range passports {
			if pp.Valid_a() {
				valid++
			}
		}

		fmt.Println("...of which", valid, "are valid (1st check)")
	}

	// b

	{
		valid := 0
		for _,pp := range passports {
			if pp.Valid_b() {
				valid++
			}
		}

		fmt.Println("...of which", valid, "are valid (2nd check)")
	}
}

package main

import (
	"fmt"
)

func main() {
	var passwords Passwords

	passwords.ReadInput("input-02.txt")

	fmt.Println("Password count:", len(passwords));

	// a

	{
		validCount := 0

		for i := 0; i < len(passwords); i++ {
			if passwords[i].Valid_a() {
				validCount++
			}
		}

		fmt.Println("Valid passwords:", validCount);
	}

	// b

	{
		validCount := 0

		for i := 0; i < len(passwords); i++ {
			if passwords[i].Valid_b() {
				validCount++
			}
		}

		fmt.Println("Valid passwords:", validCount);
	}
}

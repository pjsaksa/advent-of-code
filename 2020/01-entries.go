package main

import (
	"fmt"
	"os"
)

type Entries []int

func (entries *Entries) ReadInput(fileName string) {
	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	var item int

	for {
		if count,err := fmt.Fscanln(file, &item);
		count == 1 && err == nil {
			*entries = append(*entries, item)
		} else {
			break
		}
	}
}

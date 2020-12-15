package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
)

type Answers [26]bool
type Group []Answers
type Groups []Group

func main() {
	var groups Groups
	groups.ReadInput("input-06.txt")

	fmt.Println("There are", len(groups), "groups")

	// calculate OR-counts and AND-counts

	orCounts := make(Group, len(groups), len(groups))
	andCounts := make(Group, len(groups), len(groups))

	for i1,group := range groups {

		for i := range andCounts[i1] {
			andCounts[i1][i] = true
		}

		for _,ans := range group {
			for i3,t := range ans {
				if t {
					orCounts[i1][i3] = true
				} else {
					andCounts[i1][i3] = false
				}
			}
		}
	}

	// print counts and calculate total

	fmt.Println("Answer counts in groups of 100:")

	PrintCounts("- Any user answers 'yes':", &orCounts)
	PrintCounts("- All users answer 'yes':", &andCounts)
}

func (groups *Groups) ReadInput(fileName string) {
	const (
		ReadChunk = 20
	)

	//

	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	var chunk [ReadChunk]Group
	c := 0

	in := bufio.NewReader(file)

	for {
		line,err := in.ReadString('\n')

		if err == io.EOF {
			*groups = append(*groups, chunk[0 : c+1]...)
			return
		} else if err != nil {
			panic(err)
		}

		//

		if line == "\n" {
			c++

			if c == ReadChunk {
				*groups = append(*groups, chunk[0 : ReadChunk]...)
				c = 0
			}

			chunk[c] = Group{}
		} else {
			chunk[c] = append(chunk[c], parseAnswers(line))
		}
	}
}

func parseAnswers(line string) (ans Answers) {
	for _,ch := range line {
		if ch >= 'a' && ch <= 'z' {
			ans[ch-'a'] = true
		}
	}
	return
}

func (ans *Answers) Count() (c int) {
	for _,t := range *ans {
		if t {
			c++
		}
	}

	return
}

func PrintCounts(title string, group *Group) {
	const (
		BatchSize = 70
	)

	total := 0
	total_batch := 0

	fmt.Print(title)

	for i,ans := range *group {
		if i % BatchSize == 0 && i > 0 {
			if i == BatchSize {
				fmt.Printf("   ")
			} else {
				fmt.Printf(" + ")
			}

			fmt.Printf("%-4d", total_batch)

			total += total_batch
			total_batch = 0
		}

		total_batch += ans.Count()
	}
	total += total_batch

	fmt.Printf(" + %-4d = %d\n", total_batch, total)
}

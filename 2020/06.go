package main

import (
	"bufio"
	"fmt"
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
	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	in := bufio.NewReader(file)

	for running := true ; running ; {
		var item Group
		itemEmpty := true

		for {
			line,err := in.ReadString('\n')

			if err != nil {
				running = false
				break
			}

			//

			if line == "\n" {
				break
			}

			item = append(item, parseAnswers(line))
			itemEmpty = false
		}

		if !itemEmpty {
			*groups = append(*groups, item)
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

	totalCount := 0
	batchCount := 0

	fmt.Print(title)

	for i,ans := range *group {
		if i % BatchSize == 0 && i > 0 {
			if i == BatchSize {
				fmt.Printf("   ")
			} else {
				fmt.Printf(" + ")
			}

			fmt.Printf("%-4d", batchCount)

			totalCount += batchCount
			batchCount = 0
		}

		batchCount += ans.Count()
	}
	totalCount += batchCount

	fmt.Printf(" + %-4d = %d\n", batchCount, totalCount)
}

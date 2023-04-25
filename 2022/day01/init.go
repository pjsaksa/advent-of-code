package day01

import (
	"aoc-2022/util"

	"fmt"
	"os"
	"sort"
)

func (page *page) parseInput(fileName string) {
	f, err := os.Open(fileName)
	if err != nil {
		util.Error("%s is not available for reading", fileName)
	}
	defer f.Close()

	var meals []int
	for {
		var oneMeal int
		scannedItems, err := fmt.Fscanf(f, "%d", &oneMeal)
		if err != nil {
			// add collected meals to page.elves
			if len(meals) > 0 {
				// calculate total calories
				var totalCalories int
				for _, m := range meals {
					totalCalories += m
				}

				// create new elf
				elf := elf{
					meals:         meals,
					totalCalories: totalCalories,
				}
				page.elves = append(page.elves, elf)

				page.updateMinMax(totalCalories)

				// reset internal counter
				meals = nil
			}

			switch err.Error() {
			case "unexpected newline":
				continue
			case "EOF":
			default:
				util.Error("reading input: %s", err.Error())
			}
			break
		} else if scannedItems == 1 {
			meals = append(meals, oneMeal)
		} else {
			util.Error("reading input: strange parsing result, scannedItems == %d", scannedItems)
		}
	}
}

func (page *page) updateIndices() {
	// create unsorted index
	page.sortedElves = make([]int, len(page.elves))
	for i := 0; i < len(page.elves); i++ {
		page.sortedElves[i] = i
	}

	// sort the index
	sort.SliceStable(
		page.sortedElves,
		func(si, sj int) bool {
			i := page.sortedElves[si]
			j := page.sortedElves[sj]
			return page.elves[i].totalCalories < page.elves[j].totalCalories
		})

	// calculate average
	var total int
	for i := 0; i < len(page.elves); i++ {
		total += page.elves[i].totalCalories
	}
	page.averageCalories = total / len(page.elves)
}

func (page *page) updateMinMax(totalCalories int) {
	if len(page.elves) == 1 {
		page.minCalories = totalCalories
		page.maxCalories = totalCalories
	} else {
		if totalCalories < page.minCalories {
			page.minCalories = totalCalories
		}
		if totalCalories > page.maxCalories {
			page.maxCalories = totalCalories
		}
	}
}

package day01

import (
	"aoc-2022/util"

	"fmt"
	"os"
	"sort"
)

// Read puzzle input and store the data in its original order in a slice
// 'page.elves'. Also calculate statistics in 'page' and each 'elf' in the
// meanwhile.
func (page *page) readPuzzleInput(fileName string) {

	// Open the input file.
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
			// Failed to scan a number, either because the line was empty or
			// end of file was reached. Either way collected meals should be
			// added to the list of elves.

			// Add collected meals to 'page.elves'.
			if len(meals) > 0 {

				// Calculate total calories.
				var totalCalories int
				for _, m := range meals {
					totalCalories += m
				}

				// Create new 'elf' struct instance.
				elf := elf{
					meals:         meals,
					totalCalories: totalCalories,
				}
				page.elves = append(page.elves, elf)

				// Update statistics in 'page'.
				page.updateMinMax(totalCalories)

				// Reset the counter of collected meals.
				meals = nil
			}

			// Some file handling errors are expected here, so avoid panicing
			// unless necessary.
			switch err.Error() {
			case "unexpected newline":
				continue
			case "EOF":
			default:
				util.Error("reading input: %s", err.Error())
			}
			break
		} else if scannedItems == 1 {
			// Scanning a number succeeded, so add this to the list of collected
			// meals and keep on scanning.

			meals = append(meals, oneMeal)
		} else {
			// NOTE: This is unreachable, as far as I know.

			util.Error("reading input: strange parsing result, scannedItems == %d", scannedItems)
		}
	}

	// Calculate average calories.
	var total int
	for i := 0; i < len(page.elves); i++ {
		total += page.elves[i].totalCalories
	}
	page.averageCalories = total / len(page.elves)
}

// Use 'page.sortedElves' for an additional indexing to 'page.elves'. Then sort
// the indexing based on 'elf.totalCalories'. So, after this elves can be
// accessed unsorted:
//
//	elf = &page.elves[n]
//
// and sorted:
//
//	elf = &page.elves[page.sortedElves[n]]
//
// where 'n >= 0' and 'n < len(page.elves)'.
func (page *page) sortIndex() {

	// Create unsorted index.
	page.sortedElves = make([]int, len(page.elves))
	for i := 0; i < len(page.elves); i++ {
		page.sortedElves[i] = i
	}

	// Sort the index.
	sort.SliceStable(
		page.sortedElves,
		func(si, sj int) bool {
			i := page.sortedElves[si]
			j := page.sortedElves[sj]
			return page.elves[i].totalCalories < page.elves[j].totalCalories
		})
}

// Update 'page.minCalories' and 'page.maxCalories'.
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

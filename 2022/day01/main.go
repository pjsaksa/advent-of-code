// Contains the solution to [day 1 year 2022 puzzle] of [Advent of Code].
//
// In this puzzle a group of elves are carrying meals. The number of meals for
// each elf varies, and the amount of calories for each meal varies. The input
// data for this puzzle is a list of calories of meals carried by each elf.
//
// The first part of this puzzle is to find the elf carrying the most calories,
// and answer how many calories that elf is carrying.
//
// The second part of this puzzle is to find the top three elves carrying the
// most calories, and answer how many calories are those elves carrying in
// total.
//
// [day 1 year 2022 puzzle]: https://adventofcode.com/2022/day/1
// [Advent of Code]: https://adventofcode.com/
package day01

import (
	"aoc-2022/util"
)

// Initialize day 1 puzzle data structures and configure HTTP server to serve
// day 1 puzzle requests.
func InitPage01(hmap util.HandlerMap) {
	page := &page{}

	// Add HTTP request handlers to 'hmap'.
	hmap["/01"] = page.renderText             // text.go
	hmap["/01/css"] = page.renderCSS          // css.go
	hmap["/01/graph"] = page.renderGraph      // graph.go
	hmap["/01/graph.js"] = page.renderGraphJs // graph-js.go
	hmap["/01/info"] = page.renderInfo        // info.go

	// Read and process puzzle input.
	page.readPuzzleInput("data/input-01.txt")
	page.sortIndex()
}

// ------------------------------------------------------------

type page struct {
	elves       []elf
	sortedElves []int

	minCalories     int
	maxCalories     int
	averageCalories int
}

type elf struct {
	meals         []int
	totalCalories int
}

// ------------------------------------------------------------

// Width of the graph image in pixels.
func (page *page) imgWidth() int {
	return len(page.elves)*3 + 1
}

// Height of the graph image in pixels.
func (page *page) imgHeight() int {
	return 400
}

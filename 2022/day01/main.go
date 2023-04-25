package day01

import (
	"aoc-2022/util"
)

func InitPage01(hmap util.HandlerMap) {
	page := &page{}

	hmap["/01"] = page.renderText             // text.go
	hmap["/01/css"] = page.renderCSS          // css.go
	hmap["/01/graph"] = page.renderGraph      // graph.go
	hmap["/01/graph.js"] = page.renderGraphJs // graph-js.go

	hmap["/01/api/info"] = page.renderInfo // info.go

	//

	page.parseInput("data/input-01.txt")
	page.updateIndices()
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

func (page *page) imgWidth() int {
	return len(page.elves)*3 + 1
}

func (page *page) imgHeight() int {
	return 400
}

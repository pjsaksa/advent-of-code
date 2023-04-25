package day01

import (
	"fmt"
	"io"
	"net/http"
)

func (page *page) isRangeOk(begin, end int) bool {
	return begin >= 0 && begin < len(page.elves) && end >= 0 && end <= len(page.elves) && begin <= end
}

func (page *page) renderInfo(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		sorted := (req.URL.Query().Get("sorted") == "true")

		var bar1, bar2 int
		if n, err := fmt.Sscanf(req.URL.Query().Get("n"), "%d_%d", &bar1, &bar2); n == 2 && err == nil && page.isRangeOk(bar1, bar2) {
			out.Header().Set("Content-Type", "application/json")
			page.printElfRangeInfo(out, bar1, bar2, sorted)

		} else if n, err := fmt.Sscanf(req.URL.Query().Get("n"), "%d", &bar1); n == 1 && err == nil && page.isRangeOk(bar1, bar1) {
			out.Header().Set("Content-Type", "application/json")
			page.printElfRangeInfo(out, bar1, bar1, sorted)

		} else {
			http.Error(out, "Bad query", http.StatusBadRequest)
		}
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

func (page *page) printElfRangeInfo(out io.Writer, begin int, end int, sorted bool) {
	fmt.Fprint(out, "[\n")
	for bar := begin; bar <= end; bar++ {
		if bar > begin {
			fmt.Fprint(out, ",\n")
		}

		var elfN int
		if sorted {
			elfN = page.sortedElves[bar]
		} else {
			elfN = bar
		}

		page.printOneElfInfo(
			out,
			bar,
			elfN,
			&page.elves[elfN])
	}
	fmt.Fprint(out, "\n]\n")
}

func (page *page) printOneElfInfo(out io.Writer, bar int, elfN int, elf *elf) {
	barHeight := int(float64(elf.totalCalories) / float64(page.maxCalories) * float64(page.imgHeight()))

	fmt.Fprintf(
		out,
		"\t{ \"bar\": %d, \"elf\": %d, \"elves\": %d, \"totalCalories\": %d, \"barHeight\": %d, \"meals\":\n\t\t[ ",
		bar,
		elfN,
		len(page.elves),
		elf.totalCalories,
		barHeight)

	for i, m := range elf.meals {
		if i > 0 {
			fmt.Fprintf(out, ", ")
		}
		fmt.Fprintf(out, "%d", m)
	}
	fmt.Fprintf(out, " ]\n\t}")
}

package day01

import (
	"fmt"
	"io"
	"net/http"

	"github.com/pjsaksa/go-utils/log"
)

// Return true if the range of elves sent by browser is a valid range based on
// puzzle input.
func (page *page) isRangeOk(begin, end int) bool {
	return begin >= 0 &&
		begin < len(page.elves) &&
		end >= 0 &&
		end <= len(page.elves) &&
		begin <= end
}

func (page *page) renderInfo(out http.ResponseWriter, req *http.Request) log.Message {
	switch req.Method {
	case "", "GET":
		sorted := (req.URL.Query().Get("sorted") == "true")
		queryN := req.URL.Query().Get("n")

		var bar1, bar2 int
		if verbs, err := fmt.Sscanf(queryN, "%d_%d", &bar1, &bar2); verbs == 2 && err == nil {
			// Found match to pattern "%d_%d": a range with a begin and an end.

			if page.isRangeOk(bar1, bar2) {
				out.Header().Set("Content-Type", "application/json")

				// Produce the data.
				page.printElfRangeInfo(out, bar1, bar2, sorted)
				return log.DebugMsg("Ok")
			} else {
				http.Error(out, "Bad Query String", http.StatusBadRequest)
				return log.WarningMsg("Bad Query String (n=%s)", queryN)
			}
		} else if verbs, err := fmt.Sscanf(queryN, "%d", &bar1); verbs == 1 && err == nil {
			// Found match to pattern "%d": selection of a single elf.

			if page.isRangeOk(bar1, bar1) {
				out.Header().Set("Content-Type", "application/json")

				// Produce the data.
				page.printElfRangeInfo(out, bar1, bar1, sorted)
				return log.DebugMsg("Ok")
			} else {
				http.Error(out, "Bad Query String", http.StatusBadRequest)
				return log.WarningMsg("Bad Query String (n=%s)", queryN)
			}
		} else {
			http.Error(out, "Bad Query String", http.StatusBadRequest)
			return log.WarningMsg("Bad Query String (n=%s)", queryN)
		}
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
		return log.WarningMsg("Method Not Allowed (%s)", req.Method)
	}
}

// Produce JSON array containing info of elves in a range.
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

// Produce JSON object containing info of one elf.
func (page *page) printOneElfInfo(out io.Writer, bar int, elfN int, elf *elf) {
	barHeight := int(float64(elf.totalCalories) / float64(page.maxCalories) * float64(page.imgHeight()))

	fmt.Fprintf(
		out,
		`	{ "bar": %d, "elf": %d, "elves": %d, "totalCalories": %d, "barHeight": %d, "meals":
		[ `,
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

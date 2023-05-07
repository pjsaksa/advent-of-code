package day01

import (
	"image"
	"image/color"
	"image/png"
	"net/http"

	"aoc-2022/util/log"
)

// Render a image in PNG format containing bar graph where each bar represents
// total calories carried by each elf. Elves are either drawn in the original
// order or in sorted order, depending on URL query parameter "sorted".
//
// The image is otherwise transparent, so the background (color) is set by the
// web page in which this image is shown.
func (page *page) renderGraph(out http.ResponseWriter, req *http.Request) log.Message {
	var barColor = color.NRGBA{0x80, 0x80, 0x80, 0xFF}

	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-type", "image/x-png")

		// Check if sorted.
		sorted := (req.URL.Query().Get("sorted") == "true")

		img := image.NewNRGBA(image.Rect(0, 0, page.imgWidth(), page.imgHeight()))

		// Iterate through elves.
		var maxX = len(page.elves)
		var maxY = page.imgHeight()
		for x := 0; x < maxX; x++ {
			var elf *elf

			if sorted {
				elf = &page.elves[page.sortedElves[x]]
			} else {
				elf = &page.elves[x]
			}

			// Draw a bar of proper height, pixel by pixel.
			barHeight := int(float64(elf.totalCalories) / float64(page.maxCalories) * float64(maxY))
			for y := 0; y < barHeight; y++ {
				img.Set(3*x+1, maxY-y-1, barColor)
				img.Set(3*x+2, maxY-y-1, barColor)
			}
		}

		// Encode the image as PNG into HTTP response.
		png.Encode(out, img)

		return log.DebugMsg("Ok")

	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
		return log.WarningMsg("Method Not Allowed (%s)", req.Method)
	}
}

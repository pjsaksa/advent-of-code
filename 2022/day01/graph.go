package day01

import (
	"image"
	"image/color"
	"image/png"
	"net/http"
)

func (page *page) renderGraph(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-type", "image/x-png")

		// render graph image
		var maxX = len(page.elves)
		var maxY = page.imgHeight()
		img := image.NewNRGBA(image.Rect(0, 0, page.imgWidth(), page.imgHeight()))

		sorted := (req.URL.Query().Get("sorted") == "true")

		for x := 0; x < maxX; x++ {
			var elf *elf

			if sorted {
				elf = &page.elves[page.sortedElves[x]]
			} else {
				elf = &page.elves[x]
			}

			barHeight := int(float64(elf.totalCalories) / float64(page.maxCalories) * float64(maxY))
			for y := 0; y < barHeight; y++ {
				img.Set(3*x+1, maxY-y-1, color.NRGBA{0x80, 0x80, 0x80, 0xFF})
				img.Set(3*x+2, maxY-y-1, color.NRGBA{0x80, 0x80, 0x80, 0xFF})
			}
		}

		// encode PNG
		png.Encode(out, img)

	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

package day01

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"io"
	"net/http"
	"os"
	"sort"

	"aoc-2022/util"
)

func InitPage01(hmap util.HandlerMap) {
	page := &page{}

	hmap["/01"] = page.renderText
	hmap["/01/css"] = page.renderCSS
	hmap["/01/graph"] = page.renderGraph
	hmap["/01/graph.js"] = page.renderGraphJs

	hmap["/01/api/info"] = page.renderInfo

	//

	page.parseInput()
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

func (page *page) parseInput() {
	const fileName string = "data/input-01.txt"

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

func (page *page) imgWidth() int {
	return len(page.elves)*3 + 1
}

func (page *page) imgHeight() int {
	return 400
}

func (page *page) isRangeOk(begin, end int) bool {
	return begin >= 0 && begin < len(page.elves) && end >= 0 && end <= len(page.elves) && begin <= end
}

func (page *page) renderText(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/html")

		// render text
		fmt.Fprintf(out,
			`<!doctype html>
<html>
<head>
<title>Day 1 - Advent of Over-engineering 2022</title>
<link rel="stylesheet" href="/default.css" />
<link rel="stylesheet" href="/01/css" />
<script src="/01/graph.js"></script>
</head>

<body onload="init()">

<table id="statistics">
<tr><th>Elves</th><td colspan="3">%d</td></tr>
<tr><th rowspan="2">Calories</th><th>min</th><th>avg</th><th>max</th></tr>
<tr><td>%d</td><td>%d</td><td>%d</td></tr>
</table>

<div>
<label><input type="checkbox" onchange="setSorted(!sorted)">sorted?</label>
<div id="view1">
<canvas class="ui" width="%d" height="%d"></canvas>
<img src="/01/graph" width="%d" height="%d" />
</div>
</div>

<div id="info" hidden></div>
</body>
</html>
`,
			len(page.elves),
			page.minCalories,
			page.averageCalories,
			page.maxCalories,
			page.imgWidth(),
			page.imgHeight(),
			page.imgWidth(),
			page.imgHeight())
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

func (page *page) renderCSS(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/css")

		// render text
		fmt.Fprintf(out,
			`#statistics {
	border-collapse: collapse;
	margin: 0px 2em;
}
#statistics th,
#statistics td {
	border: 1px #c0c0c0 solid;
	padding: 0px 1em;
	text-align: center;
}
#statistics th {
	background: white;
}
#statistics td {
	background: white;
}
#view1 img {
	background: black;
	border: 4px #40b0f0 solid;
}
#view1 canvas {
	position: absolute;
	border: 4px #40b0f0 solid;
	z-index: 1;
}
#info {
	background: white;
	border: 1px #c0c0c0 solid;
	padding: 0px 1em;
}
#info ul {
	list-style-type: none;
	padding: 0px;
}
#unlock {
	margin: 2px 2em;
	border: 2px #c0c0c0 solid;
	background: Fffd0d0;
	color: #a00000;
	font-family: sans-serif;
	font-variant: small-caps;
}
`)
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

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

func (page *page) renderGraphJs(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-type", "text/javascript")

		fmt.Fprintf(out,
			`let dragBegin = null
let fetchPending = false
let hoverBar = null
let showLocked = false
let shownBar = null
let sorted = false
let uiCanvas = null

function setSorted(newSorted) {
	sorted = newSorted

	const img = document.querySelector("#view1 > img")

	if (sorted) {
		img.src = "/01/graph?sorted=true"
	} else {
		img.src = "/01/graph"
	}

	hlClear()
}

function hlClear() {
	const ctx = uiCanvas.getContext("2d")

	ctx.clearRect(
		0,
		0,
		uiCanvas.width,
		uiCanvas.height)

	const info = document.querySelector("#info")
	if (info) {
		info.innerHTML = ""
		info.hidden = true
	}

	hoverBar = null
	shownBar = null
	dragBegin = null
	showLocked = false
}

function hlBar(range, data) {
	const ctx = uiCanvas.getContext("2d")

	ctx.clearRect(
		0,
		0,
		uiCanvas.width,
		uiCanvas.height)

	ctx.fillStyle = "#40b0f0"
	for (let idx = 0; idx < data.length; idx++) {
		ctx.fillRect(
			data[idx].bar*3 + 1,
			uiCanvas.height - data[idx].barHeight,
			2,
			data[idx].barHeight)
	}

	shownBar = range

	//

	const info = document.querySelector("#info")
	if (info) {
		let infoContent = "<button id=\"unlock\" onclick=\"hlClear()\" hidden>Clear selection</button>\n"
		let totalCalories = 0

		infoContent += "<ul>\n"
		for (let idx = 0; idx < data.length; idx++) {
			totalCalories += data[idx].totalCalories

			infoContent += "<li>Elf #" + (data[idx].elf+1) + " : " + data[idx].totalCalories + " = "
			for (let m = 0; m < data[idx].meals.length; m++) {
				if (m > 0) {
					infoContent += " + "
				}
				infoContent += data[idx].meals[m]
			}
			infoContent += "</li>\n"
		}

		if (data.length > 1) {
			infoContent += "<li>These " + data.length + " elves have " + totalCalories + " calories in total</li>\n"
		}

		infoContent += "</ul>\n"

		//

		info.innerHTML = infoContent
		info.hidden = false
	}
}

function hoverEvent(e) {
	if (event.type !== "mousemove"
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	if (event.offsetX >= 0)
	{
		const bar = Math.floor(event.offsetX / 3)

		if (bar >= 0
		    && bar < %d
		    && bar !== hoverBar)
		{
			hoverBar = bar

			//

			if (dragBegin !== null)
			{
				let dragRange
				if (dragBegin < hoverBar) {
					dragRange = dragBegin + "_" + hoverBar
				} else if (dragBegin > hoverBar) {
					dragRange = hoverBar + "_" + dragBegin
				} else {
					dragRange = hoverBar
				}

				fetchBar(dragRange)
			} else {
				fetchBar(hoverBar)
			}
		}
	}
}

function fetchBar(bar) {
	if (bar === shownBar
	    || fetchPending)
	{
		return
	}
	fetchPending = true

	//

	const query = ("n="+bar) + (sorted ? "&sorted=true" : "")
	fetch("/01/api/info?" + query)
		.then((response) => response.json())
		.then((json) => {
			fetchPending = false
			hlBar(bar, json)
			if (hoverBar !== null && dragBegin === null) {
				fetchBar(hoverBar)
			}
		})
		.catch((error) => {
			fetchPending = false
			console.error("Error fetching info: " + error.message)
		})
}

function dragBeginEvent(event) {
	if (event.type !== "mousedown"
	    || event.button != 0
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	dragBegin = hoverBar
}

function dragEndEvent(event) {
	if (event.type !== "mouseup"
	    || event.button != 0
	    || dragBegin === null
	    || showLocked)
	{
		return
	}
	event.preventDefault()

	//

	dragBegin = null
	showLocked = true

	document.querySelector("#unlock").hidden = false
}

function init() {
	uiCanvas = document.querySelector("#view1 > .ui")
	const ctx = uiCanvas.getContext("2d")

	ctx.lineCap = "square"

	uiCanvas.addEventListener("mousemove", hoverEvent, { capture: true })
	uiCanvas.addEventListener("mouseleave", (event) => { if (!showLocked) hlClear() }, { capture: true })
	uiCanvas.addEventListener("mousedown", dragBeginEvent, { capture: true })
	uiCanvas.addEventListener("mouseup", dragEndEvent, { capture: true })
}
`,
			len(page.elves))

	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
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

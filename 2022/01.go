package main

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"io"
	"net/http"
	"os"
	"sort"
)

func initPage01(hmap HandlerMap) {
	page := &Page01{}

	hmap["/01"] = page.RenderText
	hmap["/01/css"] = page.RenderCSS
	hmap["/01/graph"] = page.RenderGraph
	hmap["/01/graph.js"] = page.RenderGraphJs

	hmap["/01/api/info"] = page.RenderInfo

	//

	page.parseInput()
	page.updateIndices()
}

// ------------------------------------------------------------

type Page01 struct {
	elves       []Page01_Elf
	sortedElves []int

	minCalories     int
	maxCalories     int
	averageCalories int
}

type Page01_Elf struct {
	meals         []int
	totalCalories int
}

func (page *Page01) parseInput() {
	f, err := os.Open("input-01.txt")
	if err != nil {
		Error("input-01.txt is not available for reading")
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
				elf := Page01_Elf{
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
				Error("reading input: %s", err.Error())
			}
			break
		} else if scannedItems == 1 {
			meals = append(meals, oneMeal)
		} else {
			Error("reading input: strange parsing result, scannedItems == %d", scannedItems)
		}
	}
}

func (page *Page01) updateIndices() {
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

func (page *Page01) updateMinMax(totalCalories int) {
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

func (page *Page01) ImgWidth() int {
	return len(page.elves)*3 + 1
}

func (page *Page01) ImgHeight() int {
	return 400
}

func (page *Page01) IsRangeOk(begin, end int) bool {
	return begin >= 0 && begin < len(page.elves) && end >= 0 && end <= len(page.elves) && begin <= end
}

func (page *Page01) RenderText(out http.ResponseWriter, req *http.Request) {
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
			page.ImgWidth(),
			page.ImgHeight(),
			page.ImgWidth(),
			page.ImgHeight())
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

func (page *Page01) RenderCSS(out http.ResponseWriter, req *http.Request) {
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
	background: white;
	border: 1px #c0c0c0 solid;
}
#view1 canvas {
	position: absolute;
	border: 1px #c0c0c0 solid;
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

func (page *Page01) RenderGraph(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-type", "image/x-png")

		// render graph image
		var maxX = len(page.elves)
		var maxY = page.ImgHeight()
		img := image.NewNRGBA(image.Rect(0, 0, page.ImgWidth(), page.ImgHeight()))

		sorted := (req.URL.Query().Get("sorted") == "true")

		for x := 0; x < maxX; x++ {
			var elf *Page01_Elf

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

func (page *Page01) RenderGraphJs(out http.ResponseWriter, req *http.Request) {
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

	ctx.fillStyle = "red"
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

func (page *Page01) RenderInfo(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		sorted := (req.URL.Query().Get("sorted") == "true")

		var bar1, bar2 int
		if n, err := fmt.Sscanf(req.URL.Query().Get("n"), "%d_%d", &bar1, &bar2); n == 2 && err == nil && page.IsRangeOk(bar1, bar2) {
			out.Header().Set("Content-Type", "application/json")
			page.PrintElfRangeInfo(out, bar1, bar2, sorted)

		} else if n, err := fmt.Sscanf(req.URL.Query().Get("n"), "%d", &bar1); n == 1 && err == nil && page.IsRangeOk(bar1, bar1) {
			out.Header().Set("Content-Type", "application/json")
			page.PrintElfRangeInfo(out, bar1, bar1, sorted)

		} else {
			http.Error(out, "Bad query", http.StatusBadRequest)
		}
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

func (page *Page01) PrintElfRangeInfo(out io.Writer, begin int, end int, sorted bool) {
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

		page.PrintOneElfInfo(
			out,
			bar,
			elfN,
			&page.elves[elfN])
	}
	fmt.Fprint(out, "\n]\n")
}

func (page *Page01) PrintOneElfInfo(out io.Writer, bar int, elfN int, elf *Page01_Elf) {
	barHeight := int(float64(elf.totalCalories) / float64(page.maxCalories) * float64(page.ImgHeight()))

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

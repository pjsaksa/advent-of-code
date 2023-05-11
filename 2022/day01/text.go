package day01

import (
	"fmt"
	"net/http"
	"os"

	"aoc-2022/util/log"
)

func (page *page) renderText(out http.ResponseWriter, req *http.Request) log.Message {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/html")

		if closeSvg, errCloseSvg := os.ReadFile("static/img/close-outline.svg"); errCloseSvg == nil {
			fmt.Fprintf(
				out,
				`<!doctype html>
<html>
<head>
<title>Day 1 - Advent of Over-engineering 2022</title>
<link rel="stylesheet" href="/default.css" />
<link rel="stylesheet" href="/01/css" />
<script src="/01/graph.js"></script>
</head>

<body onload="init()">

<div id="close"><a href="/">%s</a></div>

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
				closeSvg,
				len(page.elves),
				page.minCalories,
				page.averageCalories,
				page.maxCalories,
				page.imgWidth(),
				page.imgHeight(),
				page.imgWidth(),
				page.imgHeight())

			return log.DebugMsg("Ok")
		} else {
			errorMsg := "Internal Server Error: Reading file 'static/img/close-outline.svg' failed"
			http.Error(
				out,
				errorMsg,
				http.StatusInternalServerError)
			return log.ErrorMsg(errorMsg)
		}

	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
		return log.WarningMsg("Method Not Allowed (%s)", req.Method)
	}
}

package day01

import (
	"net/http"

	"aoc-2022/util/log"
)

func (page *page) renderCSS(out http.ResponseWriter, req *http.Request) log.Message {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/css")
		http.ServeFile(out, req, "static/01/01.css")
		return log.DebugMsg("Ok")
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
		return log.WarningMsg("Method Not Allowed (%s)", req.Method)
	}
}

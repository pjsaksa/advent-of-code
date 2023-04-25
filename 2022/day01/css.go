package day01

import (
	"net/http"
)

func (page *page) renderCSS(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/css")
		http.ServeFile(out, req, "static/01/01.css")
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

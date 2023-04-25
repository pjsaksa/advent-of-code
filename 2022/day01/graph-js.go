package day01

import (
	"fmt"
	"net/http"
	"os"
)

func (page *page) renderGraphJs(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-type", "text/javascript")

		if fileContent, err := os.ReadFile("static/01/graph.js"); err == nil {
			fmt.Fprintf(out,
				string(fileContent),
				len(page.elves))
		} else {
			http.Error(out, "Internal Server Error: Reading file 'static/01/graph.js' failed", http.StatusInternalServerError)
		}

	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

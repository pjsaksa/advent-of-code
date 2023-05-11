package util

import (
	"fmt"
	"html/template"
	"net/http"

	"aoc-2022/util/log"
)

type IndexPage struct {
	indexHtml *template.Template
	pages     []pageInfo
}

type pageInfo struct {
	Url   string
	Label string
	Desc  string
}

func InitIndex(hmap HandlerMap) *IndexPage {
	index := &IndexPage{}

	// Install HTTP request handlers.
	hmap["/"] = index.renderText
	hmap["/index.css"] = ServeStaticFile("index.css", "text/css")

	// Read template file for the index HTML page.
	var err error
	if index.indexHtml, err = template.ParseFiles("static/index.html"); err != nil {
		log.FATAL("Parsing template file 'static/index.html' failed: %s", err.Error())
	}

	return index
}

func (index *IndexPage) AddPage(url, label, desc string) {
	index.pages = append(
		index.pages,
		pageInfo{url, label, desc})
}

func (index *IndexPage) renderText(out http.ResponseWriter, req *http.Request) log.Message {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/html")
		if err := index.indexHtml.Execute(out, index.pages); err == nil {
			return log.DebugMsg("Ok")
		} else {
			errorMsg := fmt.Sprintf("Internal Server Error: Executing 'index.html' template failed: %s", err.Error())
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

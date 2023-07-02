package main

import (
	"fmt"
	"net/http"
	"time"

	"aoc-2022/day01"
	"aoc-2022/util"
	"github.com/pjsaksa/go-utils/log"
)

func main() {
	srv := newServer()
	srv.pages["/default.css"] = RenderCSS
	//srv.pages["/img/close.svg"] = util.ServeStaticFile("img/close-outline.svg", "image/svg+xml")

	indexPage := util.InitIndex(srv.pages)
	day01.InitPage01(srv.pages, indexPage)

	// Start the server.
	log.INFO("Listening HTTP at %s", srv.httpServer.Addr)
	if err := srv.httpServer.ListenAndServe(); err != http.ErrServerClosed {
		panic(err.Error())
	}
}

// ------------------------------------------------------------

type Server struct {
	httpServer http.Server
	pages      util.HandlerMap
}

func newServer() *Server {
	srv := &Server{
		pages: util.HandlerMap{},
	}
	srv.httpServer = http.Server{
		Addr:           ":8000",
		Handler:        srv,
		ReadTimeout:    10 * time.Second,
		WriteTimeout:   10 * time.Second,
		MaxHeaderBytes: 1 << 15,
	}
	return srv
}

func (srv *Server) ServeHTTP(out http.ResponseWriter, req *http.Request) {
	reqMsg := log.EventMsg(
		"[%s] %s %s %s",
		req.RemoteAddr,
		req.Method,
		req.URL.EscapedPath(),
		req.URL.RawQuery)
	respMsg := log.FatalMsg("Handler is missing response log message")

	if handler, ok := srv.pages[req.URL.EscapedPath()]; ok {
		respMsg = handler(out, req)
	} else {
		http.NotFound(out, req)
		respMsg = log.WarningMsg("Not Found")
	}

	log.LOG(reqMsg, respMsg)
}

// ------------------------------------------------------------

func RenderCSS(out http.ResponseWriter, req *http.Request) log.Message {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/css")
		fmt.Fprintf(out,
			`body {
	background: #a0a0a0;
}
`)
		return log.DebugMsg("Ok")
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
		return log.WarningMsg("Method Not Allowed (%s)", req.Method)
	}
}

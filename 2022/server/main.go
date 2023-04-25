package main

import (
	"fmt"
	"net/http"
	"time"

	"aoc-2022/day01"
	"aoc-2022/util"
)

func main() {
	srv := newServer()

	//

	srv.pages["/default.css"] = RenderCSS
	day01.InitPage01(srv.pages)

	//

	fmt.Println("Listening HTTP at", srv.httpServer.Addr)

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
	fmt.Printf("[%s] %s %s %s\n",
		req.RemoteAddr,
		req.Method,
		req.URL.EscapedPath(),
		req.URL.RawQuery)

	if handler, ok := srv.pages[req.URL.EscapedPath()]; ok {
		handler(out, req)
	} else {
		http.NotFound(out, req)
	}
}

// ------------------------------------------------------------

func RenderCSS(out http.ResponseWriter, req *http.Request) {
	switch req.Method {
	case "", "GET":
		out.Header().Set("Content-Type", "text/css")
		fmt.Fprintf(out,
			`body {
	background: #a0a0a0;
}
`)
	default:
		out.Header().Add("Allow", "GET")
		http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
	}
}

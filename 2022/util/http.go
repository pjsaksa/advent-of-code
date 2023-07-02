package util

import (
	"net/http"

	"github.com/pjsaksa/go-utils/log"
)

func ServeStaticFile(fileName, contentType string) HandlerFunc {
	return func(out http.ResponseWriter, req *http.Request) log.Message {
		switch req.Method {
		case "", "GET":
			out.Header().Set("Content-Type", contentType)
			http.ServeFile(out, req, "static/"+fileName)
			return log.DebugMsg("Ok")
		default:
			out.Header().Add("Allow", "GET")
			http.Error(out, "Method Not Allowed", http.StatusMethodNotAllowed)
			return log.WarningMsg("Method Not Allowed (%s)", req.Method)
		}
	}
}

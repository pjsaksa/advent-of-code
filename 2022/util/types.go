package util

import (
	"net/http"

	"github.com/pjsaksa/go-utils/log"
)

type HandlerFunc func(http.ResponseWriter, *http.Request) log.Message
type HandlerMap map[string]HandlerFunc

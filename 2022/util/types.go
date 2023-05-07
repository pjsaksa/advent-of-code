package util

import (
	"net/http"

	"aoc-2022/util/log"
)

type HandlerFunc func(http.ResponseWriter, *http.Request) log.Message
type HandlerMap map[string]HandlerFunc

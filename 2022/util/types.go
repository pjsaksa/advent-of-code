package util

import (
	"net/http"
)

type HandlerMap map[string]http.HandlerFunc

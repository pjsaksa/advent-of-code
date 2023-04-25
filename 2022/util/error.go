package util

import (
	"fmt"
	"log"
)

func Error(format string, v ...any) {
	message := fmt.Sprintf(format, v...)
	log.Println(message)
	panic(message)
}

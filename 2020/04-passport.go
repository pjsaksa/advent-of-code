package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"regexp"
	"strings"
)

const (
	f_BYR = iota // Birth Year
	f_CID        // Country ID
	f_ECL        // Eye Color
	f_EYR        // Expiration Year
	f_HCL        // Hair Color
	f_HGT        // Height
	f_IYR        // Issue Year
	f_PID        // Passport ID
	//
	f_size
)

type Passport struct {
	fields [f_size]string
}

type Passports []Passport

func parseFieldKey(str string) int {
	switch str {
	case "byr": return f_BYR
	case "cid": return f_CID
	case "ecl": return f_ECL
	case "eyr": return f_EYR
	case "hcl": return f_HCL
	case "hgt": return f_HGT
	case "iyr": return f_IYR
	case "pid": return f_PID
	}
	return -1
}

func (pps *Passports) ReadInput(fileName string) {
	const (
		ReadChunk = 10
	)

	//

	file,err := os.Open(fileName)

	if err != nil {
		panic(err)
	}
	defer file.Close()

	//

	in := bufio.NewReader(file)

	for {
		var chunk [ReadChunk]Passport

		for ci := 0; ci < ReadChunk; {
			line,err := in.ReadString('\n')

			if err == io.EOF {
				ci++
				*pps = append(*pps, chunk[0:ci]...)
				return
			} else if err != nil {
				panic(err)
			}

			line = strings.TrimSpace(line)

			if len(line) > 0 {
				for _,f := range strings.Split(line, " ") {
					keys := strings.Split(f, ":")
					if len(keys) == 2 {
						key,value := parseFieldKey(keys[0]),keys[1]

						if key < 0 {
							panic("invalid key")
						}

						chunk[ci].fields[key] = value
					} else {
						panic("number of ':' is not 1")
					}
				}
			} else {
				ci++
			}
		}

		*pps = append(*pps, chunk[0:ReadChunk]...)
	}
}

func (pp *Passport) Valid_a() bool {
	for key,value := range pp.fields {
		if key == f_CID {
			continue
		}

		if len(value) == 0 {
			return false
		}
	}

	return true
}

// ------------------------------------------------------------

func Valid_b_4digit(s string, min, max int) bool {
	if ok,err := regexp.MatchString("^[[:digit:]]{4}$", s)
		err != nil ||
			!ok {
		return false
	}

	var value int
	scanNum,scanErr := fmt.Sscanf(s, "%d", &value)

	return scanErr == nil &&
		scanNum == 1 &&
		value >= min &&
		value <= max
}

func Valid_b_hgt(s string) bool {
	var height int
	var unit string

	scanNum,scanErr := fmt.Sscanf(s, "%d%s", &height, &unit)

	if scanErr != nil ||
		scanNum != 2 {
		return false
	}

	switch unit {
	case "cm": return height >= 150 && height <= 193
	case "in": return height >= 59 && height <= 76
	}

	return false
}

func Valid_b_hcl(s string) bool {
	regexOk,err := regexp.MatchString("^#[[:xdigit:]]{6}$", s)
	return err == nil &&
		regexOk
}

func Valid_b_ecl(s string) bool {
	return s == "amb" ||
		s == "blu" ||
		s == "brn" ||
		s == "gry" ||
		s == "grn" ||
		s == "hzl" ||
		s == "oth"
}

func Valid_b_pid(s string) bool {
	regexOk,err := regexp.MatchString("^[[:digit:]]{9}$", s)
	return err == nil &&
		regexOk
}

func (pp *Passport) Valid_b() bool {
	return Valid_b_4digit(pp.fields[f_BYR], 1920, 2002) &&
		Valid_b_4digit(pp.fields[f_IYR], 2010, 2020) &&
		Valid_b_4digit(pp.fields[f_EYR], 2020, 2030) &&
		Valid_b_hgt(pp.fields[f_HGT]) &&
		Valid_b_hcl(pp.fields[f_HCL]) &&
		Valid_b_ecl(pp.fields[f_ECL]) &&
		Valid_b_pid(pp.fields[f_PID])
}

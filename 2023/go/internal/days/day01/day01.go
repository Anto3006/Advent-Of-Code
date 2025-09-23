package day01

import (
	"aoc_2023/internal/aoc"
	"unicode"
)

func init() {
	aoc.Register(1, solver{})
}

type solver struct{}

func (solver) Part1(input string) any {
	splits := split(string(input), '\n')
	calibrationSum := 0
	useOnlyDigits := true
	for _, match := range splits {
		calibration := obtainCalibration(match, useOnlyDigits)
		calibrationSum += calibration
	}
	return calibrationSum
}

func (solver) Part2(input string) any {
	splits := split(string(input), '\n')
	calibrationSum := 0
	useOnlyDigits := false
	for _, match := range splits {
		calibrationSum += obtainCalibration(match, useOnlyDigits)
	}
	return calibrationSum
}

func checkWrittenDigit(calibrationLine string, pos int) int {
	if len(calibrationLine) >= pos+3 {
		threeRunes := calibrationLine[pos : pos+3]
		if threeRunes == "one" {
			return 1
		} else if threeRunes == "two" {
			return 2
		} else if threeRunes == "six" {
			return 6
		}
	}
	if len(calibrationLine) >= pos+4 {
		fourRunes := calibrationLine[pos : pos+4]
		if fourRunes == "four" {
			return 4
		} else if fourRunes == "five" {
			return 5
		} else if fourRunes == "nine" {
			return 9
		} else if fourRunes == "zero" {
			return 0
		}
	}
	if len(calibrationLine) >= pos+5 {
		fiveRunes := calibrationLine[pos : pos+5]
		if fiveRunes == "three" {
			return 3
		} else if fiveRunes == "seven" {
			return 7
		} else if fiveRunes == "eight" {
			return 8
		}
	}
	return -1
}

func obtainDigit(calibrationLine string, runeValue rune, pos int, useOnlyDigits bool) int {
	if unicode.IsDigit(runeValue) {
		return int(runeValue - '0')
	} else if !useOnlyDigits {
		return checkWrittenDigit(calibrationLine, pos)
	} else {
		return -1
	}
}

func obtainCalibration(calibrationLine string, useOnlyDigits bool) int {
	var firstDigit int = -1
	var lastDigit int = -1
	for pos, runeValue := range calibrationLine {
		digit := obtainDigit(calibrationLine, runeValue, pos, useOnlyDigits)
		if digit != -1 {
			lastDigit = digit
			if firstDigit == -1 {
				firstDigit = digit
			}
		}
	}
	return firstDigit*10 + lastDigit
}

func split(text string, char rune) []string {
	var splits []string = make([]string, 0)
	var initial_pos int = 0
	for pos, runeValue := range text {
		if runeValue == char {
			splits = append(splits, text[initial_pos:pos])
			initial_pos = pos + 1
		}
	}
	splits = append(splits, text[initial_pos:len(text)])
	return splits
}

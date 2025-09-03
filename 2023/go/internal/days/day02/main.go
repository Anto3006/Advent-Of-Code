package main

import (
	"fmt"
	"os"
	"strconv"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func split(text string, char rune) []string {
	var splits []string = make([]string, 0)
	var initial_pos int = 0
	for pos, runeValue := range text {
		if runeValue == char && pos != initial_pos {
			splits = append(splits, text[initial_pos:pos])
			initial_pos = pos + 1
		}
	}
	if initial_pos < len(text) {
		splits = append(splits, text[initial_pos:])
	}
	return splits
}

func strip(text string) string {
	initial_pos := 0
	final_pos := len(text)
	for pos, runeValue := range text {
		if runeValue == ' ' {
			initial_pos = pos + 1
		} else {
			break
		}
	}

	for pos := len(text) - 1; pos > 0; pos-- {
		if text[pos] == ' ' {
			final_pos = pos - 1
		} else {
			break
		}
	}
	return text[initial_pos:final_pos]
}

func obtainIdSets(line string) (int, []string) {
	var lineData []string = split(line, ':')
	var sets []string = split(lineData[1], ';')
	id, e := strconv.Atoi(strip(split(lineData[0], ' ')[1]))
	check(e)
	return id, sets
}

func checkAllSetsPossible(sets []string, redCubes int, blueCubes int, greenCubes int) bool {
	areSetsPossible := true
	for _, set := range sets {
		cubes := split(set, ',')
		for _, cube := range cubes {
			cubeInfo := split(cube, ' ')
			color := cubeInfo[1]
			amount, err := strconv.Atoi(strip(cubeInfo[0]))
			check(err)
			if color == "red" {
				areSetsPossible = (amount <= redCubes)
			} else if color == "blue" {
				areSetsPossible = (amount <= blueCubes)
			} else if color == "green" {
				areSetsPossible = (amount <= greenCubes)
			} else {
				fmt.Println("Color: ", color)
			}
			if !areSetsPossible {
				break
			}
		}
		if !areSetsPossible {
			break
		}
	}
	return areSetsPossible
}

func minCubesNeeded(sets []string) (int, int, int) {
	redCubes := 0
	blueCubes := 0
	greenCubes := 0
	for _, set := range sets {
		cubes := split(set, ',')
		for _, cube := range cubes {
			cubeInfo := split(cube, ' ')
			color := cubeInfo[1]
			amount, err := strconv.Atoi(strip(cubeInfo[0]))
			check(err)
			if color == "red" && amount > redCubes {
				redCubes = amount
			} else if color == "blue" && amount > blueCubes {
				blueCubes = amount
			} else if color == "green" && amount > greenCubes {
				greenCubes = amount
			}
		}
	}
	return redCubes, blueCubes, greenCubes
}

func powerSetCubesGame(game string) int {
	_, sets := obtainIdSets(game)
	redCubes, blueCubes, greenCubes := minCubesNeeded(sets)
	return redCubes * blueCubes * greenCubes
}

func checkPossibleGame(line string, redCubes int, blueCubes int, greenCubes int) int {
	id, sets := obtainIdSets(line)
	areAllSetsPossible := checkAllSetsPossible(sets, redCubes, blueCubes, greenCubes)
	if areAllSetsPossible {
		return id
	} else {
		return 0
	}

}

func main() {
	data, err := os.ReadFile("input.txt")
	check(err)
	lines := split(string(data), '\n')
	idSum := 0
	powerSum := 0
	redCubes := 12
	blueCubes := 14
	greenCubes := 13
	for _, line := range lines {
		idSum += checkPossibleGame(line, redCubes, blueCubes, greenCubes)
	}
	for _, line := range lines {
		powerSum += powerSetCubesGame(line)
	}
	fmt.Println("Sum id possible", idSum)
	fmt.Println("Sum power cubes", powerSum)
}

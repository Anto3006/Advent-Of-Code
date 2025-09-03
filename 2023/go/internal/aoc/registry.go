package aoc

import "fmt"

type Solver interface {
	Part1(input string) any
	Part2(input string) any
}

var reg = map[int]Solver{}

func Register(day int, s Solver) {
	if _, exists := reg[day]; exists {
		panic(fmt.Sprintf("duplicate solver for %02d", day))
	}
	reg[day] = s
}

func Get(day int) (Solver, bool) {
	s, ok := reg[day]
	return s, ok
}

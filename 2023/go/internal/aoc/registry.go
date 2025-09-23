package aoc

import "fmt"

type Solver interface {
	Part1(input string) any
	Part2(input string) any
}

var solver_registry = map[int]Solver{}

func Register(day int, solver Solver) {
	if _, exists := solver_registry[day]; exists {
		panic(fmt.Sprintf("duplicate solver for %02d", day))
	}
	solver_registry[day] = solver
}

func GetSolver(day int) (Solver, bool) {
	solver, ok := solver_registry[day]
	return solver, ok
}

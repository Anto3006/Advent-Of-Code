package main

import (
	"flag"
	"fmt"
	"os"
	"strings"

	"aoc_2023/internal/aoc"
)

var (
	day  = flag.Int("day", 1, "day (1-25)")
	part = flag.Int("part", 1, "part (1 or 2)")
	file = flag.String("file", "", "optional path to input file; if empty uses embedded input")
)

func main() {
	flag.Parse()

	solver, ok := aoc.Get(*day)
	if !ok {
		fmt.Fprintf(os.Stderr, "No solver for day %d\n", *day)
		os.Exit(1)
	}

	var input string

	if *file != "" {
		b := aoc.Must(os.ReadFile(*file))
		input = string(b)
	} else {
		type hasInput interface{ Input() string }
		if hi, ok := solver.(hasInput); ok {
			input = hi.Input()
		} else {
			fmt.Fprintln(os.Stderr, "No embedded input file; pass -file")
			os.Exit(2)
		}
	}

	input = strings.TrimRight(input, "\n\r")

	var ans any
	switch *part {
	case 1:
		ans = solver.Part1(input)
	case 2:
		ans = solver.Part2(input)
	default:
		fmt.Fprintln(os.Stderr, "-part must be 1 or 2")
		os.Exit(3)
	}

	fmt.Println(ans)
}

package day03

import "testing"

const test_input = `467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..`

func TestPart1(t *testing.T) {
	got := solver{}.Part1(test_input)
	expected := 4361
	if got != expected {
		t.Fatalf("Part 1 = %d, wants %d", got, expected)
	}
}

func TestPart2(t *testing.T) {
	got := solver{}.Part2(test_input)
	expected := 467835
	if got != expected {
		t.Fatalf("Part 1 = %d, wants %d", got, expected)
	}
}

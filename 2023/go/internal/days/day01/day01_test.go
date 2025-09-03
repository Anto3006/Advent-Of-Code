package day01

import "testing"

const test_input = `1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet
`

func TestPart1(t *testing.T) {
	got := solver{}.Part1(test_input)
	if got != 142 {
		t.Fatalf("Part 1 = %d, wants 142", got)
	}
}

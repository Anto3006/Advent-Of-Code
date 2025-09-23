package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

type handOfCards struct {
	cards [5]int
	bid   int
}

func (hand *handOfCards) readFromLine(line string) {
	data := split(line, ' ')
	cards := [5]int{0, 0, 0, 0, 0}
	for pos, runeVal := range strip(data[0]) {
		cardValue := 0
		if runeVal >= '2' && runeVal <= '9' {
			cardValue = int(runeVal - '0')
		} else if runeVal == 'T' {
			cardValue = 10
		} else if runeVal == 'J' {
			cardValue = 0
		} else if runeVal == 'Q' {
			cardValue = 11
		} else if runeVal == 'K' {
			cardValue = 12
		} else if runeVal == 'A' {
			cardValue = 13
		}
		cards[pos] = cardValue
	}
	bid, err := strconv.Atoi(strip(data[1]))
	check(err)
	hand.cards = cards
	hand.bid = bid
}

func (hand *handOfCards) calculateValue() int {
	countVector := make([]int, 15)
	handValue := 0
	for _, card := range hand.cards {
		countVector[card] += 1
	}
	fiveOfAKind := countRepeats(countVector[1:], 5)
	fourOfAKind := countRepeats(countVector[1:], 4)
	threeOfAKind := countRepeats(countVector[1:], 3)
	twoOfAKind := countRepeats(countVector[1:], 2)
	numberOfJokers := countVector[0]
	if fiveOfAKind == 1 || (fourOfAKind == 1 && numberOfJokers == 1) || (threeOfAKind == 1 && numberOfJokers == 2) || (twoOfAKind == 1 && numberOfJokers == 3) || (numberOfJokers >= 4) {
		handValue = 6
	} else if fourOfAKind == 1 || (threeOfAKind == 1 && numberOfJokers == 1) || (twoOfAKind == 1 && numberOfJokers == 2) || (numberOfJokers >= 3) {
		handValue = 5
	} else if (threeOfAKind == 1 && twoOfAKind == 1) || (twoOfAKind == 2 && numberOfJokers == 1) {
		handValue = 4
	} else if threeOfAKind == 1 || (twoOfAKind == 1 && numberOfJokers == 1) || numberOfJokers >= 2 {
		handValue = 3
	} else if twoOfAKind == 2 {
		handValue = 2
	} else if twoOfAKind == 1 || numberOfJokers == 1 {
		handValue = 1
	}
	return handValue
}

type byCard []handOfCards

func (slice byCard) Len() int      { return len(slice) }
func (slice byCard) Swap(i, j int) { slice[i], slice[j] = slice[j], slice[i] }
func (slice byCard) Less(i, j int) bool {
	for pos := 0; pos < 5; pos++ {
		if slice[i].cards[pos] > slice[j].cards[pos] {
			return false
		} else if slice[i].cards[pos] < slice[j].cards[pos] {
			return true
		}
	}
	return true
}

func countRepeats(vector []int, value int) int {
	count := 0
	for _, number := range vector {
		if value == number {
			count += 1
		}
	}
	return count
}

func split(text string, char rune) []string {
	var splits []string = make([]string, 0)
	var initial_pos int = 0
	for pos, runeValue := range text {
		if runeValue == char {
			if initial_pos != pos {
				splits = append(splits, text[initial_pos:pos])
			}
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

func main() {
	args := os.Args
	if len(args) > 1 {
		data, err := os.ReadFile(args[1])
		check(err)
		lines := split(string(data), '\n')
		hands := make([]handOfCards, 0)
		handByValue := make(map[int][]handOfCards)
		for _, line := range lines {
			newHand := handOfCards{}
			newHand.readFromLine(line)
			hands = append(hands, newHand)
		}
		for _, hand := range hands {
			value := hand.calculateValue()
			handByValue[value] = append(handByValue[value], hand)
		}
		rank := 1
		acc := 0
		for value := 0; value < 7; value++ {
			handOfSameValue := handByValue[value]
			sort.Sort(byCard(handOfSameValue))
			for _, hand := range handOfSameValue {
				acc += hand.bid * rank
				rank += 1
			}
		}
		fmt.Println(acc)
	}

}

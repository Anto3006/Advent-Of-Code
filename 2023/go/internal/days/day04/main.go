package main

import (
	"fmt"
	"os"
	"strconv"
)

type scratchcard struct {
	id             int
	winningNumbers []int
	cardNumbers    []int
}

func (sc *scratchcard) obtainWinningNumbersInCard() []int {
	winningNumbersInCard := make([]int, 0)
	for _, cardNumber := range sc.cardNumbers {
		for _, winningNumber := range sc.winningNumbers {
			if cardNumber == winningNumber {
				winningNumbersInCard = append(winningNumbersInCard, cardNumber)
				break
			}
		}
	}
	return winningNumbersInCard
}

func createScratchcardFromLine(line string) *scratchcard {
	gameInfo := split(line, ':')
	idInfo := split(gameInfo[0], ' ')
	numbersInfo := split(gameInfo[1], '|')
	winningNumbers, cardNumbers := processNumbers(numbersInfo)
	id, err := strconv.Atoi(strip(idInfo[1]))
	check(err)
	c := scratchcard{id: id, winningNumbers: winningNumbers, cardNumbers: cardNumbers}
	return &c
}

func processNumbers(numbers []string) ([]int, []int) {
	winningNumbersString := numbers[0]
	cardNumbersString := numbers[1]
	winningNumbers := convertStringListToInt(split(winningNumbersString, ' '))
	cardNumbers := convertStringListToInt(split(cardNumbersString, ' '))
	return winningNumbers, cardNumbers
}

func convertStringListToInt(list []string) []int {
	convertedList := make([]int, len(list))
	for pos, value := range list {
		convertedValue, err := strconv.Atoi(strip(value))
		check(err)
		convertedList[pos] = convertedValue
	}
	return convertedList
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func powerOfTwo(n int) int {
	result := 1
	for n > 0 {
		result *= 2
		n -= 1
	}
	return result
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

func sumList(list []int) int {
	sum := 0
	for _, value := range list {
		sum += value
	}
	return sum
}

func main() {
	data, err := os.ReadFile("input.txt")
	check(err)
	cardLines := split(string(data), '\n')
	cardCopies := make([]int, len(cardLines))

	for pos, cardLine := range cardLines {
		card := createScratchcardFromLine(cardLine)
		winningNumbersInCard := card.obtainWinningNumbersInCard()
		cardCopies[pos] += 1
		copies := cardCopies[pos]
		for additionalCard := 1; additionalCard <= len(winningNumbersInCard) && pos+additionalCard < len(cardCopies); additionalCard++ {
			cardCopies[pos+additionalCard] += copies
		}
	}
	fmt.Println(sumList(cardCopies))
}

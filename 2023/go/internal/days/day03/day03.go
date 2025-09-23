package day03

import (
	"aoc_2023/internal/aoc"
	"unicode"
)

func init() {
	aoc.Register(3, solver{})
}

type solver struct{}

func (solver) Part1(input string) any {
	var matrix [][]rune = loadMatrix(string(input))
	return sumNumbersNearSymbols(matrix)
}

func (solver) Part2(input string) any {
	var matrix [][]rune = loadMatrix(string(input))
	return sumGearRatio(matrix)
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func loadMatrix(text string) [][]rune {
	var matrix [][]rune = make([][]rune, 0)
	var matrixLine []rune = make([]rune, 0)
	for _, runeValue := range text {
		if runeValue == '\n' {
			matrix = append(matrix, matrixLine)
			matrixLine = make([]rune, 0)
		} else {
			matrixLine = append(matrixLine, runeValue)
		}
	}
	if len(matrixLine) > 0 {
		matrix = append(matrix, matrixLine)
	}
	return matrix
}

func sumNumbersNearSymbols(matrix [][]rune) int {
	var currentNumber int = 0
	var sum int = 0
	var isNeighborSymbol bool = false
	for row := 0; row < len(matrix); row++ {
		for col := 0; col < len(matrix[row]); col++ {
			if unicode.IsDigit(matrix[row][col]) {
				currentNumber = currentNumber*10 + int(matrix[row][col]-'0')
				isNeighborSymbol = isNeighborSymbol || checkNeighborsSymbol(matrix, row, col)
			} else {
				if isNeighborSymbol {
					sum += currentNumber
				}
				currentNumber = 0
				isNeighborSymbol = false
			}
		}
	}
	return sum
}

func numberPositionsAroundTile(matrix [][]rune, row int, col int) [][]int {
	var positions [][]int = make([][]int, 0)
	var rowAmount int = len(matrix)
	var colAmount int = len(matrix[row])
	var digitAdjacent bool = false
	for r := row - 1; r <= row+1; r++ {
		for c := col - 1; c <= col+1; c++ {
			if r == row && c == col {
				digitAdjacent = false
			}
			if r < rowAmount && r >= 0 && c < colAmount && c >= 0 && (r != row || c != col) {
				if unicode.IsDigit(matrix[r][c]) {
					if !digitAdjacent {
						digitAdjacent = true
						var newPosition []int = []int{r, c}
						positions = append(positions, newPosition)
					}
				} else {
					digitAdjacent = false
				}
			}
		}
		digitAdjacent = false
	}
	return positions
}

func getFullNumberFromPosition(matrix [][]rune, position []int) int {
	var number int = 0
	var row int = position[0]
	var col int = position[1]
	var initialCol int = col
	for c := col; c >= 0; c-- {
		if unicode.IsDigit(matrix[row][c]) {
			initialCol = c
		} else {
			break
		}
	}
	for c := initialCol; c < len(matrix[row]); c++ {
		if unicode.IsDigit(matrix[row][c]) {
			number = number*10 + int(matrix[row][c]-'0')
		} else {
			break
		}
	}
	return number
}

func sumGearRatio(matrix [][]rune) int {
	var sum int = 0
	for row := 0; row < len(matrix); row++ {
		for col := 0; col < len(matrix[row]); col++ {
			if matrix[row][col] == '*' {
				var numberPositions [][]int = numberPositionsAroundTile(matrix, row, col)
				if len(numberPositions) == 2 {
					var firstNumber int = getFullNumberFromPosition(matrix, numberPositions[0])
					var secondNumber int = getFullNumberFromPosition(matrix, numberPositions[1])
					sum += firstNumber * secondNumber
				}
			}
		}
	}
	return sum
}

func checkNeighborsSymbol(matrix [][]rune, row int, col int) bool {
	var rowAmount int = len(matrix)
	var colAmount int = len(matrix[row])
	var isNeighborSymbol bool = false
	for r := row - 1; r <= row+1 && !isNeighborSymbol; r++ {
		for c := col - 1; c <= col+1 && !isNeighborSymbol; c++ {
			if r < rowAmount && r >= 0 && c < colAmount && c >= 0 && (r != row || c != col) {
				if matrix[r][c] != '.' && !unicode.IsDigit(matrix[r][c]) {
					isNeighborSymbol = true
				}
			}
		}
	}
	return isNeighborSymbol
}

package main

import (
	"fmt"
	"os"
)

type node struct {
	name      string
	leftNode  string
	rightNode string
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func split(text string, pattern string) []string {
	var splits []string = make([]string, 0)
	var initial_pos int = 0
	var patternSize int = len(pattern)
	var textSize int = len(text)
	for pos, _ := range text {

		if pos > initial_pos && patternSize+pos < textSize && text[pos:pos+patternSize] == pattern {
			splits = append(splits, text[initial_pos:pos])
			initial_pos = pos + patternSize
		}
	}
	if initial_pos < len(text) {
		splits = append(splits, text[initial_pos:])
	}
	return splits
}

func strip(text string) string {
	initial_pos := 0
	final_pos := len(text) - 1
	for pos, runeValue := range text {
		if runeValue == ' ' || runeValue == '\n' || runeValue == '\t' {
			initial_pos = pos + 1
		} else {
			break
		}
	}

	for pos := len(text) - 1; pos > 0; pos-- {
		runeValue := text[pos]
		if runeValue == ' ' || runeValue == '\n' || runeValue == '\t' {
			final_pos = pos - 1
		} else {
			break
		}
	}
	return text[initial_pos : final_pos+1]
}

func processData(data string) ([]rune, map[string]node) {
	readData := split(data, "\n\n")
	directionsString := readData[0]
	nodesString := readData[1]
	nodes := make(map[string]node)
	for _, nodeLine := range split(nodesString, "\n") {
		nodeInfo := split(nodeLine, "=")
		nodeName := strip(nodeInfo[0])
		neighbors := split(nodeInfo[1], ",")
		leftNeighbor := strip(neighbors[0])[1:]
		rightNeighbor := strip(neighbors[1])[:3]
		newNode := node{name: nodeName, leftNode: leftNeighbor, rightNode: rightNeighbor}
		nodes[nodeName] = newNode
	}
	directions := make([]rune, 0)
	for _, direction := range directionsString {
		directions = append(directions, direction)
	}
	return directions, nodes
}

func gcd(number1, number2 uint64) uint64 {
	for number2 > 0 {
		number1, number2 = number2, number1%number2
	}
	return number1
}

func calculateLoopLength(startingNode string, nodes map[string]node, directions []rune) uint64 {
	looped := false
	endNode := ""
	currentNode := startingNode
	directionPos := 0
	var loopLength uint64 = 0
	for !looped || endNode == currentNode {
		direction := directions[directionPos]
		if direction == 'L' {
			currentNode = nodes[currentNode].leftNode
		} else {
			currentNode = nodes[currentNode].rightNode
		}
		if endNode != "" {
			loopLength += 1
		}
		if currentNode == endNode {
			looped = true
		}
		if currentNode[2] == 'Z' && !looped {
			endNode = currentNode
		}
		directionPos = (directionPos + 1) % len(directions)
	}
	return loopLength - 1
}

func main() {

	args := os.Args
	if len(args) > 1 {
		data, err := os.ReadFile(args[1])
		check(err)
		directions, nodes := processData(string(data))
		currentNodes := make([]string, 0)
		for nodeName, _ := range nodes {
			if nodeName[2] == 'A' {
				currentNodes = append(currentNodes, nodeName)
			}
		}
		var steps uint64 = 1
		for _, currentNode := range currentNodes {
			loopLength := calculateLoopLength(currentNode, nodes, directions)
			steps = (steps / gcd(steps, loopLength)) * loopLength
		}
		fmt.Println(steps)
	}
}

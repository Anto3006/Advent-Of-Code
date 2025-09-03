package main

import (
	"fmt"
	"math"
	"os"
	"strconv"
)

type intervalMap struct {
	sourceInterval      []int
	destinationInterval []int
	length              int
}

type multipleIntervalMap struct {
	intervalMaps []intervalMap
}

func (intMap *intervalMap) applyMapping(number int) int {
	sourceBegin := intMap.sourceInterval[0]
	sourceEnd := intMap.sourceInterval[1]
	if sourceBegin <= number && number < sourceEnd {
		destinationBegin := intMap.destinationInterval[0]
		return destinationBegin + (number - sourceBegin)
	}
	return number
}

func (intMap *intervalMap) minMappingFromRange(numberRange []int) int {
	minValue := math.MaxInt32
	minRange := numberRange[0]
	maxRange := numberRange[1]
	if intMap.sourceInterval[0] <= minRange && minRange < intMap.sourceInterval[1] {
		minValue = intMap.applyMapping(minRange)
		if maxRange >= intMap.sourceInterval[1] {
			minValue = min(minValue, intMap.sourceInterval[1])
		}
	} else if minRange <= intMap.sourceInterval[0] && intMap.sourceInterval[0] < maxRange {
		minValue = intMap.applyMapping(intMap.sourceInterval[0])
		minValue = min(minValue, minRange)
	} else {
		minValue = minRange
	}
	return minValue
}

func (splitLinMap *multipleIntervalMap) applyMapping(number int) int {
	for _, intMap := range splitLinMap.intervalMaps {
		mappedNumber := intMap.applyMapping(number)
		if number != mappedNumber {
			return mappedNumber
		}
	}
	return number
}

func (splitLinMap *multipleIntervalMap) minMappingFromRange(numberRange []int) int {
	minValue := math.MaxInt32
	for _, intMap := range splitLinMap.intervalMaps {
		minValue = min(minValue, intMap.minMappingFromRange(numberRange))
	}
	return minValue
}

func composeMultipleIntervalMaps(firstMap, secondMap multipleIntervalMap) (composedMap multipleIntervalMap) {
	for _, firstIntervalMap := range firstMap.intervalMaps {
		unchangedFirstIntervalMap, unchangedSecondMap, composedIntervalMap := composeIntervalMapMultipleIntervalMap(firstIntervalMap, secondMap)
		secondMap = unchangedSecondMap
		composedMap.intervalMaps = append(composedMap.intervalMaps, unchangedFirstIntervalMap.intervalMaps...)
		composedMap.intervalMaps = append(composedMap.intervalMaps, composedIntervalMap.intervalMaps...)
	}
	composedMap.intervalMaps = append(composedMap.intervalMaps, secondMap.intervalMaps...)
	return
}

func composeIntervalMapMultipleIntervalMap(firstMap intervalMap, secondMap multipleIntervalMap) (unchangedFirstMap, unchangedSecondMap, composedMap multipleIntervalMap) {
	unchangedIntervalsFirstMap := make([]multipleIntervalMap, 0)
	if len(secondMap.intervalMaps) > 0 {
		for _, intervalMapSecondMap := range secondMap.intervalMaps {
			unchangedIntervalMap, unchangedIntervalMapSecondMap, composition := composeIntervalMapping(firstMap, intervalMapSecondMap)
			if composition.length > 0 {
				composedMap.intervalMaps = append(composedMap.intervalMaps, composition)
			}
			unchangedSecondMap.intervalMaps = append(unchangedSecondMap.intervalMaps, unchangedIntervalMapSecondMap.intervalMaps...)
			unchangedIntervalsFirstMap = append(unchangedIntervalsFirstMap, unchangedIntervalMap)
		}
	} else {
		unchangedIntervalsFirstMap = append(unchangedIntervalsFirstMap, multipleIntervalMap{intervalMaps: []intervalMap{firstMap}})
	}
	unchangedFirstMap = intersectionListMultipleIntervalMaps(unchangedIntervalsFirstMap)
	return
}

func intersectionListMultipleIntervalMaps(maps []multipleIntervalMap) multipleIntervalMap {
	result := maps[0]
	for pos, multIntMap := range maps {
		if pos > 0 {
			result = intersectionMultipleIntervalMaps(result, multIntMap)
		}
	}
	return result
}

func intersectionMultipleIntervalMaps(firstMap, secondMap multipleIntervalMap) multipleIntervalMap {
	result := multipleIntervalMap{}
	for _, intervalMapFirstMap := range firstMap.intervalMaps {
		firstSourceBegin := intervalMapFirstMap.sourceInterval[0]
		firstSourceEnd := intervalMapFirstMap.sourceInterval[1]
		for _, intervalMapSecondMap := range secondMap.intervalMaps {
			secondSourceBegin := intervalMapSecondMap.sourceInterval[0]
			secondSourceEnd := intervalMapSecondMap.sourceInterval[1]
			intersectionBegin, intersectionEnd, length := intersectionInterval(firstSourceBegin, firstSourceEnd, secondSourceBegin, secondSourceEnd)
			if length > 0 {
				sourceBeginOffset := intersectionBegin - firstSourceBegin
				sourceEndOffset := firstSourceEnd - intersectionEnd
				intersectionMap := intervalMap{sourceInterval: []int{intersectionBegin, intersectionEnd},
					destinationInterval: []int{intervalMapFirstMap.destinationInterval[0] + sourceBeginOffset, intervalMapFirstMap.destinationInterval[1] - sourceEndOffset},
					length:              length}
				result.intervalMaps = append(result.intervalMaps, intersectionMap)
			}
		}
	}
	return result
}

func composeIntervalMapping(firstMap, secondMap intervalMap) (unchangedFirstMap, unchangedSecondMap multipleIntervalMap, composedMap intervalMap) {
	firstMapDestinationBegin := firstMap.destinationInterval[0]
	firstMapDestinationEnd := firstMap.destinationInterval[1]
	secondMapSourceBegin := secondMap.sourceInterval[0]
	secondMapSourceEnd := secondMap.sourceInterval[1]

	intersectionBegin, intersectionEnd, length := intersectionInterval(firstMapDestinationBegin, firstMapDestinationEnd, secondMapSourceBegin, secondMapSourceEnd)

	if length == -1 {
		unchangedFirstMap = multipleIntervalMap{intervalMaps: []intervalMap{firstMap}}
		unchangedSecondMap = multipleIntervalMap{intervalMaps: []intervalMap{secondMap}}
		composedMap.length = -1
		return
	} else {
		unchangedFirstMap = removeIntersectionDestinationInterval(firstMap, intersectionBegin, intersectionEnd)
		unchangedSecondMap = removeIntersectionSourceInterval(secondMap, intersectionBegin, intersectionEnd)
		sourceOffset := intersectionBegin - firstMapDestinationBegin
		destinationOffset := intersectionBegin - secondMapSourceBegin
		composedMap.sourceInterval = []int{firstMap.sourceInterval[0] + sourceOffset, firstMap.sourceInterval[0] + sourceOffset + length}
		composedMap.destinationInterval = []int{secondMap.destinationInterval[0] + destinationOffset, secondMap.destinationInterval[0] + destinationOffset + length}
		composedMap.length = length
		return
	}
}

func intersectionInterval(firstIntervalBegin, firstIntervalEnd, secondIntervalBegin, secondIntervalEnd int) (intersectionBegin, intersectionEnd, length int) {
	intersectionBegin = 0
	intersectionEnd = 0
	length = -1
	if firstIntervalBegin <= secondIntervalBegin && secondIntervalBegin < firstIntervalEnd {
		intersectionBegin = secondIntervalBegin
		intersectionEnd = min(secondIntervalEnd, firstIntervalEnd)
		length = intersectionEnd - intersectionBegin
	} else if secondIntervalBegin <= firstIntervalBegin && firstIntervalBegin < secondIntervalEnd {
		intersectionBegin = firstIntervalBegin
		intersectionEnd = min(secondIntervalEnd, firstIntervalEnd)
		length = intersectionEnd - intersectionBegin
	}
	return
}

func removeIntersectionSourceInterval(intMap intervalMap, intersectionBegin, intersectionEnd int) (result multipleIntervalMap) {
	offsetSourceBegin := intersectionBegin - intMap.sourceInterval[0]
	offsetSourceEnd := intMap.sourceInterval[1] - intersectionEnd
	firstIntervalMap := intervalMap{sourceInterval: []int{intMap.sourceInterval[0], intersectionBegin},
		destinationInterval: []int{intMap.destinationInterval[0], intMap.destinationInterval[0] + offsetSourceBegin},
		length:              offsetSourceBegin}
	secondIntervalMap := intervalMap{sourceInterval: []int{intersectionEnd, intMap.sourceInterval[1]},
		destinationInterval: []int{intMap.destinationInterval[1] - offsetSourceEnd, intMap.destinationInterval[1]},
		length:              offsetSourceEnd}
	if firstIntervalMap.length > 0 {
		result.intervalMaps = append(result.intervalMaps, firstIntervalMap)
	}
	if secondIntervalMap.length > 0 {
		result.intervalMaps = append(result.intervalMaps, secondIntervalMap)
	}
	return
}

func removeIntersectionDestinationInterval(intMap intervalMap, intersectionBegin, intersectionEnd int) (result multipleIntervalMap) {
	offsetDestinationBegin := intersectionBegin - intMap.destinationInterval[0]
	offsetDestinationEnd := intMap.destinationInterval[1] - intersectionEnd
	firstIntervalMap := intervalMap{sourceInterval: []int{intMap.sourceInterval[0], intMap.sourceInterval[0] + offsetDestinationBegin},
		destinationInterval: []int{intMap.destinationInterval[0], intersectionBegin},
		length:              offsetDestinationBegin}
	secondIntervalMap := intervalMap{sourceInterval: []int{intMap.sourceInterval[1] - offsetDestinationEnd, intMap.sourceInterval[1]},
		destinationInterval: []int{intersectionEnd, intMap.destinationInterval[1]},
		length:              offsetDestinationEnd}
	if firstIntervalMap.length > 0 {
		result.intervalMaps = append(result.intervalMaps, firstIntervalMap)
	}
	if secondIntervalMap.length > 0 {
		result.intervalMaps = append(result.intervalMaps, secondIntervalMap)
	}
	return
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
	final_pos := len(text)
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
	return text[initial_pos:final_pos]
}

func processSeedsFileData(data string) ([]int, []multipleIntervalMap) {
	var information []string = split(data, "\n\n")
	var seedLine string = split(information[0], ":")[1]
	var seeds []int = convertStringListToInt(split(seedLine, " "))
	var maps []multipleIntervalMap = processMapValues(information[1:])
	return seeds, maps
}

func obtainSeedRanges(seedList []int) [][]int {
	seedRanges := make([][]int, 0)
	for pos, value := range seedList {
		if pos%2 == 0 {
			begin := value
			end := value + seedList[pos+1]
			seedRange := []int{begin, end}
			seedRanges = append(seedRanges, seedRange)
		}
	}
	return seedRanges
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

func processMapValues(mapsInformation []string) []multipleIntervalMap {
	var maps []multipleIntervalMap = make([]multipleIntervalMap, 0)
	for _, mapInformation := range mapsInformation {
		var nextMap multipleIntervalMap = multipleIntervalMap{}
		var mapLines []string = split(mapInformation, "\n")[1:]
		for _, line := range mapLines {
			var mapInformation []int = convertStringListToInt(split(line, " "))
			var destinationBegin int = mapInformation[0]
			var sourceBegin int = mapInformation[1]
			var length int = mapInformation[2]
			var newIntervalMap intervalMap = intervalMap{sourceInterval: []int{sourceBegin, sourceBegin + length},
				destinationInterval: []int{destinationBegin, destinationBegin + length},
				length:              length,
			}
			nextMap.intervalMaps = append(nextMap.intervalMaps, newIntervalMap)
		}
		maps = append(maps, nextMap)
	}
	return maps
}

func composeMaps(maps []multipleIntervalMap) multipleIntervalMap {
	composedMap := maps[0]
	for pos, currentMap := range maps {
		if pos > 0 {
			composedMap = composeMultipleIntervalMaps(composedMap, currentMap)
		}
	}
	return composedMap
}

func main() {

	args := os.Args
	if len(args) > 1 {
		data, err := os.ReadFile(args[1])
		check(err)
		seeds, maps := processSeedsFileData(string(data))
		seedRanges := obtainSeedRanges(seeds)
		composedMap := composeMaps(maps)
		minLocation := math.MaxInt32
		for _, seedRange := range seedRanges {
			minLocation = min(minLocation, composedMap.minMappingFromRange(seedRange))
		}
		fmt.Println("Min location:", minLocation)
	} else {
		//interMap1 := intervalMap{sourceInterval: []int{0, 200}, destinationInterval: []int{30, 230}, length: 200}
		//interMap2 := intervalMap{sourceInterval: []int{50, 160}, destinationInterval: []int{0, 110}, length: 110}
		data, err := os.ReadFile("test_input.txt")
		check(err)
		seeds, maps := processSeedsFileData(string(data))
		composed := composeMultipleIntervalMaps(maps[0], maps[1])
		composed = composeMultipleIntervalMaps(composed, maps[2])
		composed = composeMultipleIntervalMaps(composed, maps[3])
		composed = composeMultipleIntervalMaps(composed, maps[4])
		composed = composeMultipleIntervalMaps(composed, maps[5])
		composed = composeMultipleIntervalMaps(composed, maps[6])
		for _, seed := range seeds {
			location := composed.applyMapping(seed)
			fmt.Println(seed, location)
		}
	}
}

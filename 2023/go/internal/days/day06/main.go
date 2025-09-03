package main

import (
	"fmt"
)

func obtainDistanceFunction(maxTime int) func(int) int {
	return func(time int) int {
		return (maxTime - time) * time
	}
}

func numberOfWaysBeatRecord(maxTime, record int) int {
	distanceFunction := obtainDistanceFunction(maxTime)
	numberOfWays := 0
	for time := 0; time <= maxTime; time++ {
		distance := distanceFunction(time)
		if distance > record {
			numberOfWays++
		}
	}
	return numberOfWays
}

func main() {
	fmt.Println(numberOfWaysBeatRecord(61709066, 643118413621041))
}

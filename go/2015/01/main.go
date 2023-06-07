package main

import (
	"fmt"
	"os"
)

func main() {
	a()
	b()
}

func a() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	var floor int

	for _, value := range data {
		if value == '(' {
			floor++
		} else {
			floor--
		}
	}

	fmt.Println(floor)
}

func b() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	var floor int

	for k, value := range data {
		if value == '(' {
			floor++
		} else {
			floor--
		}

		if floor == -1 {
			fmt.Println(k + 1)
			break
		}
	}
}

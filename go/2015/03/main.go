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

	var visistedHouses = make(map[[2]int]bool)
	var x, y int

	for _, direction := range data {
		switch direction {
		case '^':
			y++
		case 'v':
			y--
		case '<':
			x--
		case '>':
			x++
		}

		visistedHouses[[2]int{x, y}] = true
	}

	fmt.Println(len(visistedHouses))
}

func b() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	var visistedHouses = make(map[[2]int]bool)
	var x1, y1 int
	var x2, y2 int

	for k, direction := range data {
		switch direction {
		case '^':
			if k%2 == 0 {
				y1++
			} else {
				y2++
			}
		case 'v':
			if k%2 == 0 {
				y1--
			} else {
				y2--
			}
		case '<':
			if k%2 == 0 {
				x1--
			} else {
				x2--
			}
		case '>':
			if k%2 == 0 {
				x1++
			} else {
				x2++
			}
		}

		if k%2 == 0 {
			visistedHouses[[2]int{x1, y1}] = true
		} else {
			visistedHouses[[2]int{x2, y2}] = true
		}
	}

	fmt.Println(len(visistedHouses))
}

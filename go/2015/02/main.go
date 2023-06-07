package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
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

	lines := strings.Split(string(data), "\n")
	var squareFeet int

	for _, line := range lines {
		dimensions := strings.Split(line, "x")
		l, _ := strconv.Atoi(dimensions[0])
		h, _ := strconv.Atoi(dimensions[1])
		w, _ := strconv.Atoi(dimensions[2])
		lw := l * w
		wh := w * h
		hl := h * l

		squareFeet += 2*lw + 2*wh + 2*hl + min(lw, wh, hl)
	}

	fmt.Println(squareFeet)
}

func min(values ...int) int {
	min := values[0]

	for _, value := range values {
		if value < min {
			min = value
		}
	}

	return min
}

func b() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	lines := strings.Split(string(data), "\n")
	var squareFeet int

	for _, line := range lines {
		dimensions := strings.Split(line, "x")
		l, _ := strconv.Atoi(dimensions[0])
		h, _ := strconv.Atoi(dimensions[1])
		w, _ := strconv.Atoi(dimensions[2])
		lw := l * w
		wh := w * h
		hl := h * l
		min := min(lw, wh, hl)

		if min == lw {
			squareFeet += 2*l + 2*w
		} else if min == wh {
			squareFeet += 2*w + 2*h
		} else {
			squareFeet += 2*h + 2*l
		}

		squareFeet += l * h * w
	}

	fmt.Println(squareFeet)
}

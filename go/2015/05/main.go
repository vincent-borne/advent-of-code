package main

import (
	"fmt"
	"os"
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
	vowels := "aeiou"
	nbOfNiceStrings := 0

	for _, line := range lines {
		if strings.Contains(line, "ab") || strings.Contains(line, "cd") || strings.Contains(line, "pq") || strings.Contains(line, "xy") {
			continue
		}

		nbOfVowels := 0
		hasDouble := false

		for k, char := range line {
			if strings.Contains(vowels, string(char)) {
				nbOfVowels++
			}

			if (k > 0 && char == rune(line[k-1])) || (k < len(line)-1 && char == rune(line[k+1])) {
				hasDouble = true
			}
		}

		if nbOfVowels < 3 || !hasDouble {
			continue
		}

		nbOfNiceStrings++
	}

	fmt.Println(nbOfNiceStrings)
}

func b() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	lines := strings.Split(string(data), "\n")
	nbOfNiceStrings := 0

	for _, line := range lines {
		hasFirstCondition := false
		hasSecondCondition := false

		for k, char := range line {
			if k > 0 && strings.Contains(line[k+1:], string(line[k-1])+string(char)) {
				hasFirstCondition = true
			}

			if len(line) > k+2 && char == rune(line[k+2]) {
				hasSecondCondition = true
			}
		}

		if hasFirstCondition && hasSecondCondition {
			nbOfNiceStrings++
		}
	}

	fmt.Println(nbOfNiceStrings)
}

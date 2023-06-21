package main

import (
	"crypto/md5"
	"encoding/hex"
	"fmt"
	"io"
	"os"
	"strconv"
	"sync"
)

func main() {
	a()
	aMultiThread()
	b()
}

func aMultiThread() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	i := 0

	exit := make(chan bool)
	var wg sync.WaitGroup

	for {
		wg.Add(1)

		go func(i int) {
			defer wg.Done()

			hash := md5.New()
			code := string(data) + strconv.Itoa(i)
			io.WriteString(hash, code)
			sum := hex.EncodeToString(hash.Sum(nil))

			for j := 0; j < 5; j++ {
				if sum[j] != '0' {
					return
				}
			}

			fmt.Println(i)
			exit <- true
		}(i)

		i++

		select {
		case <-exit:
			wg.Wait() // Wait for all goroutines to finish
			return
		default:
			// Continue the loop
		}
	}
}

func a() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	i := 0

main:
	for {
		hash := md5.New()
		code := string(data) + strconv.Itoa(i)
		io.WriteString(hash, code)
		sum := hex.EncodeToString(hash.Sum(nil))
		i++

		for j := 0; j < 5; j++ {
			if sum[j] != '0' {
				continue main
			}
		}

		fmt.Println(i - 1)
		break
	}
}

func b() {
	data, err := os.ReadFile("input.txt")
	if err != nil {
		panic("Error reading file")
	}

	i := 0

main:
	for {
		hash := md5.New()
		code := string(data) + strconv.Itoa(i)
		io.WriteString(hash, code)
		sum := hex.EncodeToString(hash.Sum(nil))
		i++

		for j := 0; j < 6; j++ {
			if sum[j] != '0' {
				continue main
			}
		}

		fmt.Println(i - 1)
		break
	}
}

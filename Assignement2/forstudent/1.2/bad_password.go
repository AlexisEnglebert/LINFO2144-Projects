package main

import (
	"bufio"
	"crypto/sha256"
	"encoding/json"
	"fmt"
	"os"
	"runtime"
	"sync"

	"github.com/schollz/progressbar/v3"
	"golang.org/x/crypto/pbkdf2"
)

const (
	iterations = 1004
	targetHash = "3b88d322a919604ae9a2be5b0f493ec7774e22413baae90d1f759cf2b8fba2f0"
	salt       = "41aca911e41ad94780f689b44f8a5bc0"
	uppercase  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	lowercase  = "abcdefghijklmnopqrstuvwxyz"
	numbers    = "0123456789"
	special    = "`*$|"
)

func get_colors() []string {
	file, err := os.Open("groupQ/colors_sorted.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	var colors []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		colors = append(colors, line)
	}

	if err := scanner.Err(); err != nil {
		panic(err)
	}

	return colors
}

func pbkf_hash(password string, salt string) string {
	h := pbkdf2.Key([]byte(password), []byte(salt), iterations, 32, sha256.New)
	return fmt.Sprintf("%x", h)
}

func find_ite_nbr(maxIter int) {
	file, err := os.Open("groupQ/database.txt")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		var data map[string]string

		if err := json.Unmarshal([]byte(line), &data); err != nil {
			panic(err)
		}

		for i := 1; i <= maxIter + 1; i++ {
			h := pbkdf2.Key([]byte(data["password"]), []byte(salt), i, 32, sha256.New)
			if fmt.Sprintf("%x", h) == data["hash"] {
				fmt.Printf("Number of iterations: %d for password: %s\n", i, data["password"])
				break
			}
		}
	}
}

type Candidate struct {
	password string
}

func main() {
	colors := get_colors()
	nbr_workers := runtime.NumCPU()
	candidateCh := make(chan Candidate, nbr_workers)
	var wg sync.WaitGroup

	// find_ite_nbr(10000)

	bar := progressbar.Default(int64(len(colors)*len(uppercase)*len(lowercase)*len(numbers)*len(special)), "Trying passwords")

	for range nbr_workers {
		wg.Add(1)

		go func() {
			defer wg.Done()
			for candidate := range candidateCh {
				h := pbkf_hash(candidate.password, salt)

				if h == targetHash {
					msg := fmt.Sprintf("Found password: %s (hash: %s)\n", candidate.password, h)
					fmt.Println("\n\n", msg)

					file, err := os.Create("found_password.txt")
					if err != nil {
						fmt.Println("Error creating file:", err)
					}

					writer := bufio.NewWriter(file)
					_, err = writer.WriteString(msg)
					if err != nil {
						fmt.Println("Error writing to file:", err)
					}

					writer.Flush()
					file.Close()
					os.Exit(0)
				}
			}
		}()
	}

	go func() {
		for _, color := range colors {
			for _, number := range numbers {
				for _, upper := range uppercase {
					for _, lower := range lowercase {
						for _, specialChar := range special {
							pswd := fmt.Sprintf("xkcd:%s-%c-%c-%c-%c", color, number, upper, specialChar, lower)
							candidateCh <- Candidate{
								password: pswd,
							}
							bar.Describe(fmt.Sprintf("Trying password : %s", pswd))
							bar.Add(1)
						}
					}
				}
			}
		}
		close(candidateCh)
	}()
	wg.Wait()
	fmt.Println("No password found")
}

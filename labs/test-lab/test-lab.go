package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) <= 1 {
		fmt.Println("Error, 1 argument expected.")
		return
	} else {
		name := ""
		for i := 1; i < len(os.Args); i++ {
			name += os.Args[i] + " "
		}

		fmt.Printf("Welcome %s to the jungle \n", name)
	}

}

package main

import (
	"fmt"
	"os"
	"strings"
	"log"
	"io"
	"net"
)


func main() {
	if len(os.Args) < 2 {
		fmt.Println("Wrong number of arguments, expected at least 2.")
		os.Exit(1)
	}

	channel := make(chan int)
	for _, arg := range os.Args[1:] {
		port := arg[strings.LastIndex(arg, "=") + 1:]
		fmt.Println("Port: ", port)
		city := arg[:strings.LastIndex(arg, "=")]
		fmt.Println("City: ", city)
		conn, err := net.Dial("tcp", port)
		if err != nil {
			log.Fatal(err)
		}
		defer conn.Close()
		go sendData(os.Stdout, conn, city)
	}
	n := 1
	n = <- channel
	fmt.Println("Channel closed: ", n)
	close(channel)

}

func sendData(destination io.Writer, source io.Reader, city string) {
	_, err := io.WriteString(destination, city)
	_, err = io.Copy(destination, source)
	fmt.Println("channel Closed")
	if err != nil {
		log.Fatal(err)
	}
}
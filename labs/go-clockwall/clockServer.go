// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"fmt"
	"flag"
)

func handleConn(c net.Conn, locale *time.Location) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, fmt.Sprintf("Time zone - %-4s : %s", locale.String(), time.Now().In(locale).Format("15:04:05\n")))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	
	// getting flag from argument.
	port := flag.String("port", "9090", "Server port")
	flag.Parse()
	timeZone := os.Getenv("TZ")
	locale, err := time.LoadLocation(timeZone)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Print("Time zone: ")
	fmt.Println(locale)

	// listening on port:
	listener, err := net.Listen("tcp", "localhost:"+*port)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Listening on port: " + *port)


	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, locale) // handle connections concurrently
	}
}

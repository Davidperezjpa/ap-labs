package main

import (
    "fmt"
    "os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)

func calc(operator int, values []int) int {
	result := 0
	
    if operator == 0 {
        for  i := 0; i < len(values); i++ {
            result = result + values[i]
        }

    } else if operator == 1 {
        for i := 0; i < len(values); i++ {
            result = result - values[i]
        }

    } else if operator == 2 {
        result = 1
        for i := 0; i < len(values); i++ {
            result = result * values[i]
        }
    } else{}

    fmt.Println("result: ", result);

	return result
}

func convertStrArrayToInt(strValues []string) []int{
	var intValues []int
	sizeOfArray := len(strValues)

    for i := 2; i < sizeOfArray; i++ {
		intValue, err := strconv.Atoi(strValues[i])
		if err != nil {
			fmt.Println("Expected integer, but received: ", strValues[i], "\nTerminating program...\n");
			os.Exit(-1);
		} else{
			intValues = append(intValues, intValue);
		}
    }
    return intValues;
}

func main() {

	if len(os.Args) < 3 {
        fmt.Println("expected atleast 2 arguments (operator, digits)")
		os.Exit(-1);
    } else {
        if os.Args[1] == "add" {
            // fmt.Println("its addition\n")
            calc(0, convertStrArrayToInt(os.Args));

        } else if os.Args[1] == "sub" {
            // fmt.Println("its substraction\n")
            calc(1, convertStrArrayToInt(os.Args));

        } else if os.Args[1] == "mult" {
            // fmt.Println("its multiplication\n")
            calc(2, convertStrArrayToInt(os.Args));

        } else{
            fmt.Println("operator not recognized")
			os.Exit(-1);
        }
    }

}

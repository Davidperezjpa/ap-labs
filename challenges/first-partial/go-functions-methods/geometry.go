// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"math"
	"fmt"
    "os"
	"strconv"
	"math/rand"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			fmt.Printf("%.2f + ",path[i-1].Distance(path[i]))
			sum += path[i-1].Distance(path[i])
		}
	}
	fmt.Printf("= ")
	return sum
}


func generateRandom() float64 {
	seed1 := rand.NewSource(time.Now().UnixNano())
    r1 := rand.New(seed1)
	min := -1000
	max := 1000
	randomNumber := (r1.Intn(max - min) + min)
	randomNumberFloat := float64(randomNumber) / 10
	return randomNumberFloat
}

func min(a, b float64) float64 {
    if a < b {
        return a
    }
    return b
}

func max(a, b float64) float64 {
    if a > b {
        return a
    }
    return b
}

func onSegment(p Point, q Point, r Point) bool {
    if q.X() <= max(p.X(), r.X()) && q.X() >= min(p.X(), r.X()) && q.Y() <= max(p.Y(), r.Y()) && q.Y() >= min(p.Y(), r.Y()){
		return true
	}
    return false
}


// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
func orientation(p Point, q Point, r Point) int {
    val := (q.Y() - p.Y()) * (r.X() - q.X()) - (q.X() - p.X()) * (r.Y() - q.Y())
 
    if val == 0 {
		return 0  // colinear
	} else if val > 0 {
		return 1	//clock wise
	} else{
		return 2	// counterclock wise
	}
}

func doIntersect(p1 Point, q1 Point, p2 Point, q2 Point) bool {
	// fmt.Printf("Checking Line: (%f, %f) , (%f, %f)\n", p1.X(), p1.Y(), p2.X(), p2.Y())
    // Find the four orientations needed for general and
    // special cases
    o1 := orientation(p1, q1, p2)
    o2 := orientation(p1, q1, q2)
    o3 := orientation(p2, q2, p1)
    o4 := orientation(p2, q2, q1)
 
    // General case
    if o1 != o2 && o3 != o4 {
		return true
	}
        
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if o1 == 0 && onSegment(p1, p2, q1) {
		return true
	} 
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if o2 == 0 && onSegment(p1, q2, q1) {
		return true
	} 
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if o3 == 0 && onSegment(p2, p1, q2) {
		return true
	}

     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if o4 == 0 && onSegment(p2, q1, q2) {
		return true
	}
 
    return false // Doesn't fall in any of the above cases
}


func checkPointsNotColliding(pointsSlice []Point, currentPoint Point, isLastPoint bool) bool {
	if len(pointsSlice) < 3 {
		return true
	} else if !isLastPoint {
		for i := 0; i < len(pointsSlice) - 2; i++{
			intersected := doIntersect(currentPoint, pointsSlice[len(pointsSlice)-1], pointsSlice[i], pointsSlice[i + 1])
			if intersected {
				// fmt.Println("Intersected!")
				return false
			}
		}
	}else{
		for i := 1; i < len(pointsSlice) - 2; i++{
			intersected := doIntersect(currentPoint, pointsSlice[len(pointsSlice)-1], pointsSlice[i], pointsSlice[i + 1])
			if intersected {
				// fmt.Println("Intersected!")
				return false
			}
		}
	}
	return true
}

func generateGeometry(sides int) Path {
	var pointsSlice []Point
	for i := 0 ; i < sides ; i++ {
		p1 := Point{generateRandom(), generateRandom()}
		if (!checkPointsNotColliding(pointsSlice, p1, false)) {
			i--
		} else {
			pointsSlice = append(pointsSlice, p1)
			// fmt.Printf("PointGenerated: (%f, %f)\n" , p1.X(), p1.Y())
		}
	}
	finalPointP2 := pointsSlice[0]
	if (!checkPointsNotColliding(pointsSlice, finalPointP2, true)) {
		// fmt.Println("Wrong geometry")
		return generateGeometry(sides)
	}else{
		pointsSlice = append(pointsSlice, finalPointP2)
		var fullPath Path = pointsSlice
		return fullPath
	}
	
}

func printPath(fullPath Path){
	fmt.Println("Figure's vertices")
	for i := 0 ; i < len(fullPath) - 1 ; i++ {
		fmt.Printf("   (%.1f, %.1f)\n", fullPath[i].X(), fullPath[i].Y())
	}
}

func main() {
	if (len(os.Args) != 2){
		fmt.Println("Illegal number of arguments, expected 1 argument.")
		os.Exit(-1)
	}
	sides, err := strconv.Atoi(os.Args[1])
    if err != nil {
        // handle error
        fmt.Println("Invalid argument. Only positive integer is allowed as argument.")
        os.Exit(-1)
    }else if (sides < 3){
        fmt.Println("Invalid argument. Sides must be greater than 3. <Sides>")
        os.Exit(-1)
	}
	fmt.Printf("Generating a [%d] sides figure\n", sides)
	// fmt.Println("Number of sides:", sides)
	
	var fullPath Path = generateGeometry(sides);
	printPath(fullPath)
	fmt.Println("Figure's Perimeter")
	fmt.Printf("%.2f\n",fullPath.Distance())

	
	
	

}

//!-path

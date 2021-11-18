package main

var mybool bool;

func main() {
    var arg1 int;
	arg1, _ = strconv.Atoi(os.Args[1])
    lmao = 120
    fmt.Println("Golang test")
        
	return 0
}

var myint int;

func add(n1 int, n2 int) int {
    return n1 + n2
}

func empty() {}

func factorial(n int) int {
    if n == 0 {
        return 1
    }
    return n * factorial(n-1)
}
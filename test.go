package main

var mybool bool;
var poggers, kekw, pogchamp int;

func main() {
    var arg1 int;
	arg1, _ = strconv.Atoi(os.Args[1])
    lmao = 120
    fmt.Println("Golang test")
    if 1 {
        fmt.Println("Stmt 1");
        fmt.Println("Stmt 2");
        fmt.Println("Stmt 3");
    }
    
    for (i < 10) {
        i = i + 1
    }

    for i < 10 {
        i = i + 1
        i = i - 1
    }


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

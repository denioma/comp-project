./gocompiler.out -t < testcases/$1.dgo | diff testcases/$1.out - -y

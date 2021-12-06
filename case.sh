if [ "$1" = "-l" ]; then
    path=meta1
    flag="$1"
elif [ "$1" = "-t" ]; then
    path=meta2
    flag="$1"
else
    path=meta3
    flag="-s"
fi

./gocompiler $flag < $path/$2.dgo $sort | diff $path/$2.out - -y

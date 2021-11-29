if [ "$1" = "-l" ]; then
    path=meta1
    flag="$1"
elif [ "$1" = "-t" ]; then
    path=meta2
    flag="$1"
elif [ "$1" = "-s" ]; then
    path=meta3
    flag="-s"
else 
    echo "No flag given!"
    exit
fi

make # for making sure that the compiler is updated
echo "Testcases"
echo "---------------------------------------------"

for input in $path/*.dgo; do
    filename=`echo $input | cut -d "." -f1`
    out=$(./gocompiler $flag < $filename.dgo | diff $filename.out -)
    if [ $? -eq 0 ]; then
        echo "✅ PASSED $filename.dgo"
    else
        echo "❌ FAILED $filename.dgo"
    fi
done

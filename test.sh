make # for making sure that the compiler is updated
echo "Testcases"
echo "---------------------------------------------"
if [ "$1" = "-l" ]; then
    path=testcases/meta1
    flag="-l"
else
    path=testcases/meta2
    flag="-t"
fi

for input in $path/*.dgo; do
    filename=`echo $input | cut -d "." -f1`
    out=$(./gocompiler.out $flag < $filename.dgo | diff $filename.out -)
    if [ $? -eq 0 ]; then
        echo "✅ PASSED $filename.dgo"
    else
        echo "❌ FAILED $filename.dgo"
    fi
done

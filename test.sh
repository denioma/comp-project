make # for making sure that the compiler is updated
echo "Testcases"
echo "---------------------------------------------"
if [ $1 -eq "-1" ]; then
    path=testcases/meta1
else
    path=testcases
fi

for input in $path/*.dgo; do
    filename=`echo $input | cut -d "." -f1`
    out=$(./gocompiler.out -l < $filename.dgo | diff $filename.out -)
    if [ $? -eq 0 ]; then
        echo "✅ PASSED $filename.dgo"
    else
        echo "❌ FAILED $filename.dgo"
    fi
done

for input in testcases/*.dgo; do
    filename=`echo $input | cut -d "." -f1`
    out=$(./gocompiler.out -l < $filename.dgo | diff $filename.out -)
    if [ $? -eq 0 ]; then
        echo "✅ PASSED $filename.dgo"
    else
        echo "❌ FAILED $filename.dgo"
    fi
done

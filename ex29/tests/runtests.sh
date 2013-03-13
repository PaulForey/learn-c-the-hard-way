mv tests/tests.log tests/tests.log.old
touch tests/tests.log
echo "Cleared test log (previous log renamed to 'tests.log.old')."

echo "Running unit tests:"

for i in tests/$TESTPAT
do
    if test -f $i
    then
        if $VALGRIND ./$i &> tests/tests.log
        then
            echo "$i PASS."
        else
            echo "ERROR in $i. Here's tests/tests.log:"
            echo "------------"
            cat tests/tests.log
            echo "------------"
            exit 1
        fi
    fi
done

#echo ""

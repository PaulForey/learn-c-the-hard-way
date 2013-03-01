echo "Clearing test log (old log renamed to 'log.old')."
mv tests/tests.log tests/tests.log.old
touch tests/tests.log

echo "Running unit tests:"

for i in tests/*_test
do
    if test -f $i
    then
        if $VALGRIND ./$i 2>> tests/tests.log
        then
            echo $i PASS
        else
            echo "ERROR in $i. Here's tests/tests.log:"
            echo "------------"
            tail tests/tests.log
            exit 1
        fi
    fi
done

echo ""

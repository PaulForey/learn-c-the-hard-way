mv tests/tests.log tests/tests.log.old
touch tests/tests.log
echo "Cleared test log (previous log renamed to 'tests.log.old')."

echo "Running unit tests:"

EXITCODE=0

for i in tests/$TESTPAT
do
    if test -f $i
    then
		echo "----------------------"
		echo "RUNNING: $i"
        if $VALGRIND ./$i 2>> tests/tests.log
        then
            echo "PASSED: $i"
        else
            echo -e "\tERROR in $i."
            EXITCODE=1
        fi
    fi
done

echo "----------------------"
if test $EXITCODE -gt 0
then
	echo -e "\tERRORS FOUND, here's tests/tests.log:"
	echo "=================="
	cat tests/tests.log
	echo "=================="
fi

exit $EXITCODE

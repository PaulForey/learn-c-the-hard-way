mv test/tests.log test/tests.log.old
touch test/tests.log

echo "Running unit tests:"

ERRORS=0

for i in test/*_test
do
    if test -f $i
    then
		echo "----------------------"
		echo "Running $i..."
        if $VALGRIND ./$i 2>> test/tests.log
		then
            echo "$i passed."
        else
            echo "ERROR in $i."
			((ERRORS++))
        fi
    fi
done

echo "----------------------"


if test $ERRORS -gt 0
then
	echo -e "\t$ERRORS tests FAILED, here's tests/tests.log:"
	echo "======================================================="
	cat test/tests.log
	echo "======================================================="
	exit 1
fi

if test -n "$VALGRIND"
then
	echo "VALGRIND ran, here's tests/tests.log:"
	echo "======================================================="
	cat test/tests.log
	echo "======================================================="
	exit 0
fi

echo "All tests passed! You win a hot dog!"
exit 0

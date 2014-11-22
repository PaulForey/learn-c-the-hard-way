# Some useful variables:
HORIZONTAL_RULE="======================================================="
DASH_RULE="-------------------------"
ERRORS=0

# Backup recent test log and prepare new log:
mv test/tests.log test/tests.log.old
touch test/tests.log
echo $HORIZONTAL_RULE >> test/tests.log

# Start running the tests:
echo "Running unit tests:"
for i in test/$TEST_PAT
do
    if [ -f $i ]
    then
        echo $DASH_RULE
        echo "Running $i..."
        if $MEMTEST ./$i 2>> test/tests.log
        then
            echo "$i passed."
            echo $HORIZONTAL_RULE >> test/tests.log
        else
            echo "ERROR in $i."
            ERRORS=`expr $ERRORS + 1`
            break
        fi
    fi
done

# Finish off the output:
echo $DASH_RULE

# If there were errors, show the log and exit with an error code:
if [ $ERRORS -gt 0 ]
then
    echo "\t$ERRORS tests FAILED, here's tests/tests.log:"
    cat test/tests.log
    exit 1
fi

# If the memory test was ran, show the log and exit normally:
if [ -n "$MEMTEST" ]
then
    echo "\"$MEMTEST\" ran, here's tests/tests.log:"
    cat test/tests.log
    exit 0
fi

# If we've got here, everything is okay. You can relax now:
echo "All tests passed! You win a hot dog!"
exit 0

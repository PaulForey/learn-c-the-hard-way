HORIZONTAL_RULE="======================================================="
DASH_RULE="-------------------------"

mv test/tests.log test/tests.log.old
touch test/tests.log
echo $HORIZONTAL_RULE >> test/tests.log

echo "Running unit tests:"

ERRORS=0

for i in test/$TEST_PAT
do
    if [ -f $i ]
    then
        echo $DASH_RULE
        echo "Running $i..."
        if [ -n "$MEMTEST" ]
        then
            if $MEMTEST ./$i 2>> test/tests.log
            then
                echo "$i passed."
            else
                echo "ERROR in $i."
                ((ERRORS++))
                break
            fi
        else
            if ./$i 2>> test/tests.log
            then
                echo "$i passed."
            else
                echo "Error in $i."
                ((ERRORS++))
            fi
        fi
    fi
done

echo $HORIZONTAL_RULE >> test/tests.log
echo $DASH_RULE

if [ $ERRORS -gt 0 ]
then
    echo -e "\t$ERRORS tests FAILED, here's tests/tests.log:"
    cat test/tests.log
    exit 1
fi

if [ -n "$MEMTEST" ]
then
    echo "\"$MEMTEST\" ran, here's tests/tests.log:"
    cat test/tests.log
    exit 0
fi

echo "All tests passed! You win a hot dog!"
exit 0

Write-Host "Clearing test log (old log renamed to 'log.old')."
Copy-Item test/tests.log test/tests.log.old
Clear-Content test/tests.log

Write-Host "Running Unit Tests:"

$errors = 0

foreach($i in Get-ChildItem test -filter "*_test.exe") {
        if($ENV:DRMEMORY) {
                echo "$ENV:DRMEMORY./test/$i"
                $result = & "$ENV:DRMEMORY./test/$i" 2> test/tests.log
        }
        else {
                echo ./test/$i
                $result = & "./test/$i" > test/tests.log
        }
        #$result = & "drmemory -batch ./test/$i" 2> test/tests.log
        #$result = $i > test/tests.log
        if($result) {
                Write-Host $i PASS
        }
        else {
                Write-Host "ERROR in test $i"
                $errors++
        }
}

Write-Host ""
if($errors -gt 0) {
        Write-Host "$errors tests failed. Here's test/tests.log`:"
        Write-Host "==========================="
        Get-Content test/tests.log
        Write-Host "==========================="
        exit 1
}
else {
        Write-Host "All tests passed! You win a hot dog!"
        Get-Content test/tests.log
}


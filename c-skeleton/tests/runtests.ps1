Write-Host "Clearing test log (old log renamed to 'log.old')."
Copy-Item tests/tests.log tests/tests.log.old
Clear-Content tests/tests.log

Write-Host "Running Unit Tests:"

foreach($i in Get-ChildItem tests -filter "*_test*") {
        $result = drmemory -batch tests/$i 2>> tests/tests.log
        if($result) {
                Write-Host $i PASS
        }
        else {
                Write-Host "ERROR in test $i: Here's tests/tests.log"
                Write-Host "------"
                Get-Content tests/tests.log
                exit 1
        }
}

Write-Host ""

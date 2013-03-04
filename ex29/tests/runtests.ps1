Write-Host "Clearing test log (old log renamed to 'log.old')."
Copy-Item tests/tests.log tests/tests.log.old
Clear-Content tests/tests.log

Write-Host "Running Unit Tests:"

foreach($i in Get-ChildItem tests -filter "*_test.exe") {
        $result = drmemory -batch tests/$i > tests/tests.log
        #$result = $i > tests/tests.log
        if($result) {
                Write-Host $i PASS
                Write-Host "Here's tests/tests.log`:"
                Get-Content tests/tests.log
        }
        else {
                Write-Host "ERROR in test $i. Here's tests/tests.log`:"
                Write-Host "------"
                Get-Content tests/tests.log
                exit 1
        }
}

Write-Host ""

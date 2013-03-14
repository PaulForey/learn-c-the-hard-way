#include <bstrlib.h>
#include "minunit.h"
#include "libex30.h"

char* test_functions()
{
	int rc = 1;
	rc = print_a_message(bfromcstr("Happy days!"));
	mu_assert(rc == 0, "print_a_message failed.");
	rc = uppercase(bfromcstr("Happy days!"));
	mu_assert(rc == 0, "uppercase failed.");
	rc = lowercase(bfromcstr("Happy days!"));
	mu_assert(rc == 0, "lowercase failed.");
	return NULL;
}

char* test_failures()
{
	int rc = 1;
	rc = fail_on_purpose();
	mu_assert(rc == 1, "Fail on purpose didn't fail?!");
	return NULL;
}

char* all_tests() {
	mu_suite_start();
	mu_run_test(test_functions);
	mu_run_test(test_failures);
	return NULL;
}

RUN_TESTS(all_tests);

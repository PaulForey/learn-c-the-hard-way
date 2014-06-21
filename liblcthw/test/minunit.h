#ifndef _minunit_h
#define _minunit_h
#define DEBUG

#include <stdio.h>
#include <dbg.h>
#include <stdlib.h>

#define mu_suite_start() char* message = NULL;
#define mu_assert(test, message) \
    if (!(test)) { \
    log_err(message); \
    return message; \
    }
#define mu_run_test(test) \
    message = test(); \
    tests_run++; \
    if(message) { \
        debug("------%s FAILED.", " "#test); \
        return message; \
    } else { \
        debug("------%s passed.", " "#test); \
    }
#define RUN_TESTS(name) \
    int main(int argc, char* argv[]) { \
        argc = argc; /* squashes compiler warnings */ \
        debug("------ RUNNING: %s", argv[0]); \
        char* result = name(); \
        if (result != 0) { \
            debug("------ FAILED: %s", argv[0]); \
            printf("Test %d FAILED: %s\n", tests_run, result); \
        } \
        else { \
            debug("------ PASSED: %s", argv[0]); \
            printf("%d tests ran. All tests passed.\n", tests_run); \
        } \
        exit(result != 0); \
    }

int tests_run;

#endif // _minunit_h

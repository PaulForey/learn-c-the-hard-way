#include <stdio.h>
#include <Windows.h>
#include "dbg.h"

#define LIB_FILE "libex29"

int test_print_a_message(HMODULE lib)
{
    int rc = 0;
    FARPROC func = GetProcAddress(lib, "print_a_message");
    check(func != NULL, "Did not find print_a_message function in the library %s", LIB_FILE);

    debug("Zero");
    rc = func("Print this message!");
    debug("One");
    check(rc == 0, "Failed to print message.");
    debug("Two");

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    //rc = func(broken_string);
    check(rc == 0, "Failed to print message from broken string.");
    debug("three");

    return 0;

error:
    return 1;
}

int test_uppercase(HMODULE lib)
{
    int rc = 0;

    FARPROC func = GetProcAddress(lib, "uppercase");
    check(func != NULL, "Did not find uppercase function in the library %s", LIB_FILE);

    rc = func("What is all this about?");
    check(rc == 0, "Failed to change case.");

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    //rc = func(broken_string);
    check(rc == 0, "Failed to change case of broken string.");

    return 0;

error:
    return 1;
}

int test_lowercase(HMODULE lib)
{
    int rc = 0;

    FARPROC func = GetProcAddress(lib, "lowercase");
    check(func != NULL, "Did not find lowercase function in the library %s", LIB_FILE);

    func("What? What?!? Where are the CATS gone?");
    check(rc == 0, "Failed to lower case.");

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    //func(broken_string);
    check(rc == 0, "Failed to lower case of broken string.");

    return 0;

error:
    return 1;
}

int test_fail_on_purpose(HMODULE lib)
{
    int rc = 0;

    FARPROC func = GetProcAddress(lib, "fail_on_purpose");
    check(func != NULL, "Did not find fail_on_purpose function in the library %s", LIB_FILE);

    func("It just doesn't matter!");
    check(rc == 0, "fail_on_purpose failed! Who have thunk it?");

    return 0;

error:
    return 1;
}

int main()
{
    int rc = 0;

    debug("Hello!");

    HMODULE lib = LoadLibrary(TEXT(LIB_FILE));
    debug("Error?");
    check(lib != NULL, "Failed to open the library %s", LIB_FILE);

    /// Start some tests:
    //lib_function func = GetProcAddress(lib, func_to_run);
    rc = test_print_a_message(lib);
    check(rc == 0, "print_a_message failed it's test.");
    rc = test_uppercase(lib);
    check(rc == 0, "uppercase failed it's test.");
    rc = test_lowercase(lib);
    check(rc == 0, "lowercase failed it's test.");
    rc = test_fail_on_purpose(lib);
    check(rc == 0, "fail_on_purpose failed it's test.");

    rc = FreeLibrary(lib);
    check(rc != 0, "Failed to close %s", LIB_FILE);

    debug("Goodbye!");

    return 0;

error:
    return 1;
}
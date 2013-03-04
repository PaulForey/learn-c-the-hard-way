#include <stdio.h>
#include <dlfcn.h>
#include "dbg.h"

#define LIB_FILE "build/libex29.so"

typedef int (*lib_function)(const char* data, int length);

int test_print_a_message(void* lib)
{
    int rc = 0;
	char msg[] = "Print this message!";
    lib_function func = dlsym(lib, "print_a_message");
    check(func != NULL, "Did not find print_a_message function in the library %s", LIB_FILE);

	debug("%d", sizeof(msg));
    rc = func(msg, sizeof(msg));
    check(rc == 0, "Failed to print message.");

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    rc = func(broken_string, 4);
    check(rc == 0, "Failed to print message from broken string.");

    return 0;

error:
    return 1;
}

int test_uppercase(void* lib)
{
    int rc = 0;
	char msg[] = "What is all this about?";

    lib_function func = dlsym(lib, "uppercase");
    check(func != NULL, "Did not find uppercase function in the library %s", LIB_FILE);

    rc = func(msg, sizeof(msg));
    check(rc == 0, "Failed to change case.");

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    rc = func(broken_string, 4);
    check(rc == 0, "Failed to change case of broken string.");

    return 0;

error:
    return 1;
}

int test_lowercase(void* lib)
{
    int rc = 0;
	char msg[] = "What? What?!? Where are the CATS gone?";

    lib_function func = dlsym(lib, "lowercase");
    check(func != NULL, "Did not find lowercase function in the library %s", LIB_FILE);

    rc = func(msg, sizeof(msg));
    check(rc == 0, "Failed to lower case.");
	debug("rc = %d", rc);

    char broken_string[4] = {'a', 'b', 'c', 'd'};

    func(broken_string, 4);
    check(rc == 0, "Failed to lower case of broken string.");

    return 0;

error:
    return 1;
}

int test_fail_on_purpose(void* lib)
{
    int rc = 0;
	char msg[] = "It just doesn't matter.";

    lib_function func = dlsym(lib, "fail_on_purpose");
    check(func != NULL, "Did not find fail_on_purpose function in the library %s", LIB_FILE);

    func(msg, sizeof(msg));
    check(rc == 0, "fail_on_purpose failed! Who have thunk it?");

    return 1;

error:
    return 0;
}

int main()
{
    int rc = 0;


    void* lib = dlopen(LIB_FILE, RTLD_NOW);
    check(lib != NULL, "Failed to open the library %s: %s", LIB_FILE, dlerror());

    /// Start some tests:
    //lib_function func = GetProcAddress(lib, func_to_run);
    rc = test_print_a_message(lib);
    check(rc == 0, "print_a_message failed it's test.");
    rc = test_uppercase(lib);
    check(rc == 0, "uppercase failed it's test.");
    rc = test_lowercase(lib);
    check(rc == 0, "lowercase failed it's test.");
    rc = test_fail_on_purpose(lib);
    check(rc == 1, "fail_on_purpose didn't fail. This is a failure.");

    rc = dlclose(lib);
    check(rc == 0, "Failed to close %s: %s", LIB_FILE, dlerror());

    return 0;

error:
    return 1;
}

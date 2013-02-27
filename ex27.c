#include <stdio.h>
#include <assert.h>
#include "dbg.h"

/*
 * Naive copy that assumes that inputs are always valid.
 * Taken from K&R C and cleaned up a bit.
 */
void copy(char to[], char from[])
{
	int i = 0;
	// While loop will not end if the from string isn't '\0' terminated:
	while((to[i] = from[i]) != '\0') {
		++i;
	}
}

/*
 * A safer version that checks for common errors using the
 * length of each string to control the loops and termination.
 */
int safercopy(int from_len, char* from, int to_len, char* to)
{
    assert(from != NULL && to != NULL && "from and to can't be NULL");
    int i = 0;
    int max = from_len > to_len -1 ? to_len - 1 : from_len;

    // to_len must have at least one byte:
    if(from_len < 0 || to_len <= 0) return -1;
    
    for(i = 0; i < max; i++) {
        to[i] = from[i];
    }

    to[to_len -1] = '\0';

    return 1;
}


/*
 * Now we try some things out.
 */
int main(int argc, char* argv[])
{
    // Be careful to understand why we get these sizes:
    char from[] = "0123456789";
    int from_len = sizeof(from);

    // Notice that it's 7 chars + \0
    char to[] = "6543210";
    int to_len = sizeof(to);

    debug("Copying '%s':%d to '%s':%d", from, from_len, to, to_len);

    int rc = safercopy(from_len, from, to_len, to);
    debug("%i", rc);
    check(rc > 0, "Failed to safercopy.");
    check(to[to_len - 1] == '\0', "String not terminated.");

    debug("Result is: '%s':%d", to, to_len);

    // Now we try to break it:
    rc = safercopy(from_len * -1, from, to_len, to);
    debug("%i", rc);
    check(rc == -1, "safercopy should fail here");
    check(to[to_len - 1] == '\0', "String not terminated.");

    rc = safercopy(from_len, from, 0, to);
    debug("%i", rc);
    check(rc == -1, "safercopy should fail here, too");
    check(to[to_len - 1] == '\0', "String not terminated.");

    return 0;

error:
    return 1;
}

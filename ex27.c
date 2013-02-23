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

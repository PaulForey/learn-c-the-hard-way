#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_a_message(char* msg, int length)
{
	check(msg != NULL, "msg is NULL.");
	check(length > 0, "length needs to be greater than 0.");

	// Assume the string is bad, and fix it:
	if(msg[length-1] != '\0') {
		log_warn("String is not null terminated.");
		msg[length-1] = '\0';
	}

    printf("A STRING: %s\n", msg);
    return 0;

error:
	return 1;
}

int uppercase(const char* msg, int length)
{
    int i = 0;

	check(msg != NULL, "msg is NULL.");
	check(length > 0, "Length needs to be greater than 0.");

	for(i = 0; i < length; i++) {
		printf("%c", toupper(msg[i]));
	}

    printf("\n");

    return 0;

error:
	return 1;
}

int lowercase(const char* msg, int length)
{
    int i = 0;

	check(msg != NULL, "msg is NULL.");
	check(length > 0, "Length needs to be greater than 0.");

	if(msg[length-1] != '\0') {
		log_warn("String is not null terminated.");
	}

	for(i = 0; i < length; i++) {
		printf("%c", tolower(msg[i]));
	}
    printf("\n");
	
    return 0;

error:
	debug("Error reached!");
	return 1;
}

int fail_on_purpose(const char* msg, int length)
{
	check(msg != NULL, "msg is NULL.");
	check(length > 0, "length needs to be greater than 0.");

	printf("First: \"%s\", then fail!\n", msg);

    return 1;
	
error:
	return 1;
}

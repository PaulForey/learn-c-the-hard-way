#include "libex30.h"

int print_a_message(const bstring msg)
{
	check(msg != NULL, "msg is NULL.");

    printf("message: %s\n", bdata(msg));

    return 0;
error:
	return 1;
}

int uppercase(const bstring msg)
{
	check(msg != NULL, "msg is NULL.");

    int i = 0;
	for(i = 0; i < msg->slen; i++) {
		printf("%c", toupper(msg->data[i]));
	}
    printf("\n");

    return 0;
error:
	return 1;
}

int lowercase(const bstring msg)
{
	check(msg != NULL, "msg is NULL.");

    int i = 0;
	for(i = 0; i < msg->slen; i++) {
		printf("%c", tolower(msg->data[i]));
	}
    printf("\n");
	
    return 0;
error:
	return 1;
}

int fail_on_purpose(void)
{
    return 1;
}

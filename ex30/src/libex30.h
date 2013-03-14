#ifndef _libex30_h
#define _libex30_h

#include <stdio.h>
#include <ctype.h>
#include "dbg.h"
#include <bstrlib.h>

/* 
 * function:	print_a_message
 * return code:	0 for success; 1 for failure
 */
int print_a_message(bstring msg);

/* 
 * function:	uppercase
 * return code:	0 for success; 1 for failure
 */
int uppercase(bstring msg);

/* 
 * function:	lowercase
 * return code:	0 for success; 1 for failure
 */
int lowercase(bstring msg);

/* 
 * function:	fail_on_purpose
 * return code:	always returns 1
 */
int fail_on_purpose(void);

#endif // _libex30_h

#include "minunit.h"
#include <bstrlib.h>
#include <time.h>

#define STRING_1 "Hello!"
#define STRING_1_LENGTH 6
#define STRING_2 "Hello! My name is Zebra. I am a horse."
#define STRING_2_LENGTH 38
#define STRING_3 "Hello,Bonjour,Guten tag,Hola"
#define STRING_3_LENGTH 28

char* test_bstring()
{
    bstring test_string = bfromcstr(STRING_1);
    mu_assert(test_string != NULL, "string creation failed");
    mu_assert(blength(test_string) == STRING_1_LENGTH,
            "string length not correct");

    bstring test_string_copy = bstrcpy(test_string);
    mu_assert(test_string_copy != NULL, "string copy failed");

    int res = bstricmp(test_string, test_string_copy);
    mu_assert(res == 0, "bstrings not equal");

    bdestroy(test_string);
    bdestroy(test_string_copy);
    return NULL;
}

char* test_blk2bstr()
{
    char* test_block = calloc((size_t)STRING_1_LENGTH+1, sizeof(char));
    test_block[0] = 'H';
    test_block[1] = 'e';
    test_block[2] = 'l';
    test_block[3] = 'l';
    test_block[4] = 'o';
    test_block[5] = '!';
    test_block[6] = '\0';
    bstring string_block = blk2bstr(test_block, STRING_1_LENGTH);
    mu_assert(string_block != NULL, "bstring block creation failed");
    printf("The bstring block is: %s\n", bdata(string_block));

    mu_assert(blength(string_block) == STRING_1_LENGTH,
            "the block has got the wrong length");

    bdestroy(string_block);
    free(test_block);

    return NULL;
}

char* test_bstring_functions()
{
    bstring test_string_1 = bfromcstr(STRING_1);
    mu_assert(test_string_1 != NULL, "string 1 creation failed");
    bstring test_string_2 = bfromcstr(STRING_1);
    mu_assert(test_string_2 != NULL, "string 2 creation failed");
    bstring test_string_3 = bfromcstr(STRING_2);
    mu_assert(test_string_3 != NULL, "string 3 creation failed");

    printf("String 1 is: %s\n", bdata(test_string_1));

    int res = biseq(test_string_1, test_string_2);
    mu_assert(res == 1, "strings aren't equal");

    res = bconcat(test_string_1, test_string_2);
    mu_assert(res == BSTR_OK, "concantenation failed (also spelling?)");
    mu_assert(blength(test_string_1) == STRING_1_LENGTH*2,
            "length is wrong");

    printf("String 1 is now: %s\n", bdata(test_string_1));

    res = biseq(test_string_1, test_string_2);
    mu_assert(res == 0, "strings are equal when they shouldn't be");

    // second argument is position to start search:
    res = binstr(test_string_3, 0, test_string_2);
    mu_assert(res != BSTR_ERR, "Target string not found!");
    printf("Target string found at pos: %i\n", res);


    bstring found_string = bmidstr(test_string_3, res,
            blength(test_string_2));
    mu_assert(found_string != NULL, "substring failed");
    printf("Target string first char: %c\n", bchar(found_string, 0)); 
    printf("Target string last char: %c\n", bchar(found_string,
                STRING_1_LENGTH-1));
    printf("Target string found: %s\n", bdata(found_string));


    bdestroy(test_string_1);
    bdestroy(test_string_2);
    bdestroy(test_string_3);
    bdestroy(found_string);

    return NULL;
}

char* test_bstring_list()
{
    bstring test_string = bfromcstr(STRING_3);
    mu_assert(test_string != NULL, "string creation failed");

    struct bstrList* split_strings = bsplit(test_string, ',');
    mu_assert(split_strings != NULL, "split strings failed");

    mu_assert(split_strings->qty == 4, "wrong quantity of split strings");

    bdestroy(test_string);
    bstrListDestroy(split_strings);

    return NULL;
}

char* test_bstring_assign()
{
    bstring test_string_1 = bfromcstr(STRING_1);
    bstring test_string_2 = bfromcstr(STRING_2);

    printf("String 1: %s\n", bdata(test_string_1));
    printf("String 2: %s\n", bdata(test_string_2));

    int res = bassign(test_string_2, test_string_1); 
    mu_assert(res == BSTR_OK, "bassign failed");
    mu_assert(bstrcmp(test_string_1, test_string_2) == 0,
            "test strings aren't equal");

    printf("String 2 now: %s\n", bdata(test_string_2));

    res = bassigncstr(test_string_1, STRING_2);
    mu_assert(res == BSTR_OK, "bassigncstr failed");
    
    printf("String 1 now: %s\n", bdata(test_string_1));

    //get seconds since the epoch for random seed
    time_t time_now = time(NULL);
    srand((unsigned int) time_now);

    char* test_block = calloc((size_t)STRING_2_LENGTH, sizeof(char));
    int i;
    for(i = 0; i < STRING_2_LENGTH; i++) {
        test_block[i] = (char) rand();
    }
    //test_block[STRING_2_LENGTH] = '\0';
    res = bassignblk(test_string_2, test_block, STRING_2_LENGTH);
    mu_assert(res == BSTR_OK, "bassignblk failed");

    mu_assert(bstrcmp(test_string_1, test_string_2) != 0,
            "test strings are equal when they shouldn't be");

    printf("String 2 now: %s\n", bdata(test_string_2));

    free(test_block);
    bdestroy(test_string_1);
    bdestroy(test_string_2);

    return NULL;
}
/*
test_bfindreplace

test_bformat

test_bchar
*/

char* all_tests() {
    mu_suite_start();

    mu_run_test(test_bstring);
    mu_run_test(test_blk2bstr);
    mu_run_test(test_bstring_functions);
    mu_run_test(test_bstring_list);
    mu_run_test(test_bstring_assign);

    return NULL;
}

RUN_TESTS(all_tests);

#include "minunit.h"
#include <assert.h>
#include <bstrlib.h>
#include <lcthw/hashmap.h>
Hashmap* map = NULL;

static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("xest data 3"); //may be typo
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(HashmapNode* node)
{
    debug("KEY: %s; VALUE: %s", bdata((bstring)node->key), bdata((bstring)node->data));
    traverse_called++;
    return 0;
}

static int traverse_fail_cb(HashmapNode* node)
{
    debug("KEY: %s; VALUE: %s", bdata((bstring)node->key), bdata((bstring)node->data));
    traverse_called++;
    if(traverse_called == 2) {
        return 1;
    } else {
        return 0;
    }
}

char* test_create()
{
    map = Hashmap_create(NULL, NULL);
    mu_assert(map != NULL, "Failed to create map.");
    return NULL;
}

char* test_get_set()
{
    int rc = Hashmap_set(map, &test1, &expect1);
    mu_assert(rc == 0, "failed to set test1");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == &expect1, "Wrong value for test1.");
    
    rc = Hashmap_set(map, &test2, &expect2);
    mu_assert(rc == 0, "failed to set test2");
    result = Hashmap_get(map, &test2);
    mu_assert(result == &expect2, "Wrong value for test2.");
    
    rc = Hashmap_set(map, &test3, &expect3);
    mu_assert(rc == 0, "failed to set test3");
    result = Hashmap_get(map, &test3);
    mu_assert(result == &expect3, "Wrong value for test3.");
    return NULL;
}

char* test_traverse()
{
    int rc = Hashmap_traverse(map, traverse_good_cb);
    mu_assert(rc == 0, "Failed to traverse.");
    mu_assert(traverse_called == 3, "Wrong count traverse.");
    traverse_called = 0;
    
    rc = Hashmap_traverse(map, traverse_fail_cb);
    mu_assert(rc == 1, "Failed to traverse");
    mu_assert(traverse_called == 2, "Wrong traverse count for fail.");
    traverse_called = 0;
    return NULL;
}

char* test_delete()
{
    bstring deleted = (bstring)Hashmap_delete(map, &test1);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect1, "Should get test1.");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == NULL, "Should delete.");

    deleted = (bstring)Hashmap_delete(map, &test2);
    mu_assert(deleted != NULL, "Got NULL on delete2.");
    mu_assert(deleted == &expect2, "Should get test2.");
    result = Hashmap_get(map, &test2);
    mu_assert(result == NULL, "Should delete2.");

    deleted = (bstring)Hashmap_delete(map, &test3);
    mu_assert(deleted != NULL, "Got NULL on delete3.");
    mu_assert(deleted == &expect3, "Should get test3.");
    result = Hashmap_get(map, &test3);
    mu_assert(result == NULL, "Should delete.");

    return NULL;
}

char* test_count()
{
    int rc = Hashmap_count(map);
    mu_assert(rc == 0, "map should be empty");
    
    rc = Hashmap_set(map, &test1, &expect1);
    mu_assert(rc == 0, "failed to set test1 here");
    
    rc = Hashmap_count(map);
    mu_assert(rc == 1, "map should contain one item");
    
    rc = Hashmap_set(map, &test2, &expect2);
    mu_assert(rc == 0, "failed to set test2 here");
    
    rc = Hashmap_count(map);
    mu_assert(rc == 2, "map should contain one item");
    
    Hashmap_delete(map, &test1);
    rc = Hashmap_count(map);
    mu_assert(rc == 1, "map should contain one item");
    
    Hashmap_delete(map, &test2);
    rc = Hashmap_count(map);
    mu_assert(rc == 0, "map should be empty");
    
    return NULL;
}

char* test_insert_duplicates()
{
    int rc = Hashmap_set(map, &test1, &expect1);
    mu_assert(rc == 0, "failed to set test1 on first try");
    bstring result = Hashmap_get(map, &test1);
    mu_assert(result == &expect1, "wrong value for test1 on first try");
    
    rc = Hashmap_count(map);
    mu_assert(rc == 1, "Wrong count after adding item.");
    
    rc = Hashmap_set(map, &test1, &expect2);
    mu_assert(rc == 1, "failed to set test1 on second try");
    
    rc = Hashmap_count(map);
    mu_assert(rc == 1, "Wrong count after adding duplicate item.");
    
    result = Hashmap_get(map, &test1);
    mu_assert(result == &expect2, "wrong value for test1 on second try");
    
    return NULL;
}

char* test_destroy()
{
    Hashmap_destroy(map);
    return NULL;
}

char* all_tests()
{
    mu_suite_start();
    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_traverse);
    mu_run_test(test_delete);
    mu_run_test(test_count);
    mu_run_test(test_insert_duplicates);
    mu_run_test(test_destroy);
    return NULL;
}

RUN_TESTS(all_tests);


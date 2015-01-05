#include "minunit.h"
#include <lcthw/bstree.h>
#include <lcthw/hashmap_algos.h>
#include <assert.h>
#include <bstrlib.h>
#include <stdlib.h>
#include <time.h>

BSTree* map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("xest data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(BSTreeNode* node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;
    return 0;
}

static int traverse_fail_cb(BSTreeNode* node)
{
    debug("KEY: %s", bdata((bstring)node->key));
    traverse_called++;
    
    if(traverse_called == 2) {
        return 1;
    }
    
    return 0;
}

char* test_create()
{
    map = BSTree_create(NULL);
    mu_assert(map != NULL, "Failed to create map.");
    
    return NULL;
}

char* test_destroy()
{
    BSTree_destroy(map);
    
    return NULL;
}

char* test_get_set()
{
    int rc = BSTree_set(map, &test1, &expect1);
    mu_assert(rc == 0, "Failed to set &test1");
    bstring result = BSTree_get(map, &test1);
    mu_assert(result == &expect1, "Wrong value for test1.");
    
    rc = BSTree_set(map, &test2, &expect2);
    mu_assert(rc == 0, "Failed to set &test2");
    result = BSTree_get(map, &test2);
    mu_assert(result == &expect2, "Wrong value for test2.");
    
    rc = BSTree_set(map, &test3, &expect3);
    mu_assert(rc == 0, "Failed to set &test3");
    result = BSTree_get(map, &test3);
    mu_assert(result == &expect3, "Wrong value for test3.");
    
    return NULL;
}

char* test_traverse()
{
    int rc = BSTree_traverse(map, traverse_good_cb);
    mu_assert(rc == 0, "Failed to traverse.");
    mu_assert(traverse_called == 3, "Wrong count traverse.");
    
    traverse_called = 0;
    rc = BSTree_traverse(map, traverse_fail_cb);
    mu_assert(rc == 1, "Failed to traverse.");
    mu_assert(traverse_called == 2, "Wrong count traverse for fail.");
    
    return NULL;
}

char* test_delete()
{
    bstring deleted = (bstring)BSTree_delete(map, &test1);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect1, "Should get test1");
    bstring result = BSTree_get(map, &test1);
    mu_assert(result == NULL, "Should delete.");
    
    deleted = (bstring)BSTree_delete(map, &test1);
    mu_assert(deleted == NULL, "Should get NULL on delete.");
    
    deleted = (bstring)BSTree_delete(map, &test2);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect2, "Should get test2");
    result = BSTree_get(map, &test2);
    mu_assert(result == NULL, "Should delete.");
    
    deleted = (bstring)BSTree_delete(map, &test3);
    mu_assert(deleted != NULL, "Got NULL on delete.");
    mu_assert(deleted == &expect3, "Should get test3");
    result = BSTree_get(map, &test3);
    mu_assert(result == NULL, "Should delete.");
    
    // test deleting non-existent stuff
    deleted = (bstring)BSTree_delete(map, &test3);
    mu_assert(deleted == NULL, "Should get NULL.");
    
    return NULL;
}

#define FUZZ_SIZE 100

char* test_fuzzing()
{
    BSTree* store = BSTree_create(NULL);
    int i = 0;
    int j = 0;
    bstring numbers[FUZZ_SIZE] = {NULL};
    bstring data[FUZZ_SIZE] = {NULL};

    srand((unsigned int)time(NULL));
    
    int num;

    for (i = 0; i < FUZZ_SIZE; i++) {
        num = rand();
        numbers[i] = bformat("%d", num);
        data[i] = bformat("data %d", num);
        BSTree_set(store, numbers[i], data[i]);
    }

    for (i = 0; i < FUZZ_SIZE; i++) {
        bstring value = BSTree_delete(store, numbers[i]);
        mu_assert(value == data[i], "Failed to delete the right number.");
        
        mu_assert(BSTree_delete(store, numbers[i]) == NULL,
                                        "Should get nothing");
        
        for (j = i+1; j < FUZZ_SIZE-1-i; j++) {
            bstring value = BSTree_get(store, numbers[j]);
            mu_assert(value == data[j], "Failed to get the right number.");
        }
        
        bdestroy(value);
        bdestroy(numbers[i]);
    }

    BSTree_destroy(store);
    
    return NULL;
}


static int hash_compare(void* a, void* b)
{
    return (a < b) ? -1 : (a > b);
}

char* test_christhash_tree() // Christmas tree! Geddit?
{
    BSTree* hash_tree = BSTree_create(hash_compare);
    mu_assert(hash_tree != NULL, "Failed to create hash_tree.");

    uint32_t hash1 = Hashmap_default_hash(&test1);
    int res = BSTree_set(hash_tree, &hash1, &expect1);
    mu_assert(res == 0, "Failed to set hash1 in tree.");

    bstring result = BSTree_get(hash_tree, &hash1);
    mu_assert(result == &expect1, "hash1 data not proper.");

    result = BSTree_delete(hash_tree, &hash1);
    mu_assert(result == &expect1, "hash1 data not proper.");

    BSTree_destroy(hash_tree);

    return NULL;
}

char* test_hash_fuzzing()
{
    BSTree* store = BSTree_create(hash_compare);
    int i = 0;
    int j = 0;
    uint32_t numbers[FUZZ_SIZE] = {0};
    bstring data[FUZZ_SIZE] = {NULL};

    srand((unsigned int)time(NULL));
    
    int num;
    
    for(i = 0; i < FUZZ_SIZE; i++) {
        num = rand();
        numbers[i] = Hashmap_default_hash(bformat("%d", num));
        data[i] = bformat("data %d", num);
        BSTree_set(store, &numbers[i], data[i]);
    }
    
    for (i = 0; i < FUZZ_SIZE; i++) {
        bstring value = BSTree_delete(store, &numbers[i]);
        mu_assert(value == data[i], "Failed to delete the right number.");
        
        mu_assert(BSTree_delete(store, &numbers[i]) == NULL,
                                      "Should get nothing");
        
        for(j = i+1; j < FUZZ_SIZE-1-i; j++) {
            bstring value = BSTree_get(store, &numbers[j]);
            mu_assert(value == data[j], "Failed to get the right number.");
        }
        
        bdestroy(value);
    }
    
    BSTree_destroy(store);
    
    return NULL;
}

#define TEST_TIME 1
#define ARRAY_SIZE 80000

char* test_string_key_speed()
{
    BSTree* tree = BSTree_create(NULL);

    bstring keys[ARRAY_SIZE] = {NULL};
    bstring values[ARRAY_SIZE] = {NULL};

    time_t start_time;
    time_t elapsed_time;
    // Use the time as the random seed and store it as start time.
    srand((unsigned int)time(&start_time));

    int num;
    int i = 0;

    do {
        mu_assert(i < ARRAY_SIZE, "i is too big for test array.")
        num = rand();
        keys[i] = bformat("%d", num);
        values[i] = bformat("data %d", num);
        BSTree_set(tree, keys[i], values[i]);
        
        i++;
        elapsed_time = time(NULL) - start_time;
    } while (elapsed_time < TEST_TIME);

    int data_size = i;

    printf("string key:\tDATA SIZE: %d\tTIME: %d\t OPS: %f\n",
            data_size,
            (int)elapsed_time,
            (double)data_size/elapsed_time);

    for (i = 0; i < data_size; i++) {
        bdestroy(keys[i]);
        bdestroy(values[i]);
    }

    BSTree_destroy(tree);

    return NULL;
}

char* test_hash_key_speed()
{
    BSTree* tree = BSTree_create(hash_compare);

    uint32_t keys[ARRAY_SIZE] = {0};
    bstring values[ARRAY_SIZE] = {NULL};

    time_t start_time;
    time_t elapsed_time;
    // Use the time as the random seed and store it as start time.
    srand((unsigned int)time(&start_time));

    int num;
    int i = 0;

    do {
        mu_assert(i < ARRAY_SIZE, "i is too big for test array.")
        num = rand();
        //bformat("%d", num);
        keys[i] = Hashmap_default_hash(bformat("%d", num));
        values[i] = bformat("data %d", num);
        BSTree_set(tree, &keys[i], values[i]);
        
        i++;
        elapsed_time = time(NULL) - start_time;
    } while (elapsed_time < TEST_TIME);

    int data_size = i;

    printf("hash key:\tDATA SIZE: %d\tTIME: %d\t OPS: %f\n",
            data_size,
            (int)elapsed_time,
            (double)data_size/elapsed_time);

    for (i = 0; i < data_size; i++) {
        bdestroy(values[i]);
    }

    BSTree_destroy(tree);

    return NULL;
}
char* all_tests()
{
    mu_suite_start();
    
//#ifdef whatever
    mu_run_test(test_create);
    mu_run_test(test_get_set);
    mu_run_test(test_traverse);
    mu_run_test(test_delete);
    mu_run_test(test_destroy);
    mu_run_test(test_christhash_tree);
    mu_run_test(test_string_key_speed);
    mu_run_test(test_hash_key_speed);
//#endif
    mu_run_test(test_fuzzing);
    mu_run_test(test_hash_fuzzing);
    
    return NULL;
}

RUN_TESTS(all_tests);

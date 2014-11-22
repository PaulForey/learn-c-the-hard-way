#include <stdint.h>
#include <dbg.h>
#include <bstrlib.h>
#include <lcthw/hashmap.h>

static int default_compare(void* a, void* b)
{
    return bstrcmp((bstring)a, (bstring)b);
}

/**
 * Simple Bob Jenkin's hash algorithm taken
 * from the Wikipedia description.
 */
static uint32_t default_hash(void* a)
{
    size_t len = blength((bstring)a);
    char* key = bdata((bstring)a);
    uint32_t hash = 0;
    uint32_t i = 0;
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

Hashmap* Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
    Hashmap* map = calloc(1, sizeof(Hashmap));
    check_mem(map);
    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash == NULL ? default_hash : hash;
    map->buckets = DArray_create(sizeof(DArray*), DEFAULT_NUMBER_OF_BUCKETS);
    map->buckets->end = map->buckets->max; // Fake out expanding it.
    check_mem(map->buckets);
    return map;
error:
    if(map) {
        Hashmap_destroy(map);
    }
    return NULL;
}

void Hashmap_destroy(Hashmap* map)
{
    int i = 0;
    int j = 0;
    if(map) {
        if(map->buckets) {
            for(i = 0; i < DArray_count(map->buckets); i++) {
                DArray* bucket = DArray_get(map->buckets, i);
                if(bucket) {
                    for(j = 0; j < DArray_count(bucket); j++) {
                        free(DArray_get(bucket, j));
                    }
                    DArray_destroy(bucket);
                }
            }
            DArray_destroy(map->buckets);
        }
        free(map);
    }
}

static inline HashmapNode* Hashmap_node_create(int hash, void* key, void* data)
{
    HashmapNode* node = calloc(1, sizeof(HashmapNode));
    check_mem(node);
    node->key = key;
    node->data = data;
    node->hash = hash;
    return node;
error:
    return NULL;
}

static inline DArray* Hashmap_find_bucket(Hashmap* map, void* key,
        int create, uint32_t* hash_out)
{
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
    *hash_out = hash; // Store it for the return so the caller can use it.
    DArray* bucket = DArray_get(map->buckets, bucket_n);
    if(!bucket && create) {
        // New bucket, set it up:
        bucket = DArray_create(sizeof(void*), DEFAULT_NUMBER_OF_BUCKETS);
        check_mem(bucket);
        DArray_set(map->buckets, bucket_n, bucket);
    }
    return bucket;
error:
    return NULL;
}

static inline int Hashmap_get_node(Hashmap* map, uint32_t hash,
        DArray* bucket, void* key)
{
    int i = 0;
    for(i = 0; i < DArray_end(bucket); i++) {
        debug("TRY: %d", i);
        HashmapNode* node = DArray_get(bucket, i);
        if(node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int Hashmap_set(Hashmap* map, void* key, void* data)
{
    uint32_t hash = 0;
    DArray* bucket = Hashmap_find_bucket(map, key, 1, &hash);
    check(bucket, "Error: can't create bucket.");
    // Check if data already exists first:
    int existing_node_pos = Hashmap_get_node(map, hash, bucket, key);
    if (existing_node_pos == -1) {
        HashmapNode* node = Hashmap_node_create(hash, key, data);
        check_mem(node);
        DArray_push(bucket, node);
        return 0;
    } else {
        HashmapNode* node = DArray_get(bucket, existing_node_pos);
        node->data = data;
        return 1;
    }
error:
    return -1;
}

void* Hashmap_get(Hashmap* map, void* key)
{
    uint32_t hash = 0;
    DArray* bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if(!bucket)
        return NULL;
    int i = Hashmap_get_node(map, hash, bucket, key);
    if(i == -1)
        return NULL;
    HashmapNode* node = DArray_get(bucket, i);
    check(node != NULL, "Failed to get node from its bucket.");
    return node->data;
error:
    return NULL;
}

int Hashmap_traverse(Hashmap* map, Hashmap_traverse_cb traverse_cb)
{
    int i = 0;
    int j = 0;
    int rc = 0;
    for(i = 0; i < DArray_count(map->buckets); i++) {
        DArray* bucket = DArray_get(map->buckets, i);
        if(bucket) {
            for(j = 0; j < DArray_count(bucket); j++) {
                HashmapNode* node = DArray_get(bucket, j);
                rc = traverse_cb(node);
                if(rc != 0)
                    return rc;
            }
        }
    }
    return 0;
}

int Hashmap_count(Hashmap* map)
{
    check_mem(map);
    int declared_buckets = 0;
    int i = 0;
    for(i = 0; i < DArray_count(map->buckets); i++) {
        DArray* bucket = DArray_get(map->buckets, i);
        if (bucket) {
            declared_buckets += DArray_count(bucket);
        }
    }
    debug("declared_buckets: %d", declared_buckets);
    return declared_buckets;
error:
    return -1;
}

bstring Hashmap_bstring(Hashmap* map)
{
    bstring pair_separator = bfromcstr("; ");
    bstring key_value_separator = bfromcstr(": ");
    bstring ender = bfromcstr("}");
    check_mem(map);
    bstring result = bfromcstr("{");
    int i = 0;
    int j = 0;
    int rc;
    for(i = 0; i < DArray_count(map->buckets); i++) {
        DArray* bucket = DArray_get(map->buckets, i);
        if(bucket) {
            for(j = 0; j < DArray_count(bucket); j++) {
                HashmapNode* node = DArray_get(bucket, j);
                if(node) {
                    rc = bconcat(result, node->key); 
                    check(rc == 0, "Error concantenating strings");
                    rc = bconcat(result, key_value_separator); 
                    check(rc == 0, "Error concantenating strings");
                    rc = bconcat(result, node->data); 
                    check(rc == 0, "Error concantenating strings");
                    rc = bconcat(result, pair_separator); 
                    check(rc == 0, "Error concantenating strings");
                }
            }
        }
    }
    
    rc = btrunc(result, blength(result)-2);
    check(rc == 0, "Error truncating result string");
    
    rc = bconcat(result, ender); 
    check(rc == 0, "Error concantenating strings");
    
    bdestroy(pair_separator);
    bdestroy(key_value_separator);
    bdestroy(ender);
    return result;
error:
    return NULL;
}

void* Hashmap_delete(Hashmap* map, void* key)
{
    uint32_t hash = 0;
    DArray* bucket = Hashmap_find_bucket(map, key, 0, &hash);
    if(!bucket)
        return NULL;
    int i = Hashmap_get_node(map, hash, bucket, key);
    if(i == -1)
        return NULL;
    HashmapNode* node = DArray_get(bucket, i);
    void* data = node->data;
    free(node);
    HashmapNode* ending = DArray_pop(bucket);
    if(ending != node) {
        // Alright looks ike it's not the last one; swap it:
        DArray_set(bucket, i, ending);
    }
    return data;
}


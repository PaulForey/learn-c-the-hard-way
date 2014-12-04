#include <dbg.h>
#include <lcthw/bstree.h>
#include <stdlib.h>
#include <lcthw/bstrlib.h>

static int default_compare(void* a, void* b)
{
    return bstrcmp((bstring)a, (bstring)b);
}

BSTree* BSTree_create(BSTree_compare compare)
{
    BSTree* map = calloc(1, sizeof(BSTree));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare; 

    return map;

error:
    if(map)
        BSTree_destroy(map);
    return NULL;
}

static int BSTree_destroy_cb(BSTreeNode* node)
{
    free(node);
    return 0;
}

void BSTree_destroy(BSTree* map)
{
    if(map) {
        BSTree_traverse(map, BSTree_destroy_cb);
        free(map);
    }
}

static inline BSTreeNode* BSTreeNode_create(BSTreeNode* parent, void* key, void* data)

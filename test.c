#include "dbg.h"

int main ()
{
    void* point = NULL;
    printf("Size of void pointer %p:\t%d\n", point, sizeof(point));

    int x = 201;
    int* y = &x;
    printf("Size of int point %p:\t%d\n", y, sizeof(y));

    return 0;
}

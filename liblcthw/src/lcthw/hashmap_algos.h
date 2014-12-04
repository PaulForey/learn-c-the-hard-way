#ifndef _lcthw_Hashmap_algos_h
#define _lcthw_Hashmap_algos_h

#include <stdint.h>

uint32_t Hashmap_default_hash(void* data);
uint32_t Hashmap_fnv1a_hash(void* data);
uint32_t Hashmap_adler32_hash(void* data);
uint32_t Hashmap_djb_hash(void* data);
uint32_t Hashmap_pfa_hash(void* data);

#endif // _lcthw_Hashmap_algos_h

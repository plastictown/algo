#ifndef __STUPID_HASHMAP_INCLUDED__
#define __STUPID_HASHMAP_INCLUDED__

#include <stdlib.h>
#include <stdint.h>

#include <hdr/array.h>
#include <hdr/list.h>

// only for POD values
typedef struct hash_pair
{
	uint32_t h; /**< Hash */
	node_s*  l; /**< List of values */
} hash_pair_t;

typedef struct stupid_hashmap
{
	array_t idx; /** array of hash_pair structs */
} stupid_hashmap_t;

uint32_t stupid_hash ( void* key, size_t key_sz );


#endif //!__STUPID_HASHMAP_INCLUDED__

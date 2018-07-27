#ifndef __STUPID_HASHMAP_INCLUDED__
#define __STUPID_HASHMAP_INCLUDED__

#include <stdlib.h>
#include <stdint.h>

#include <hdr/array.h>
#include <hdr/list.h>

// 115249 is a prime number
#define STUPID_HASHMAP_MAX 115249

typedef enum shm_ret_state
{
  SHT_OK   = 0,
  SHT_FAIL = 1
} shm_ret_state_t;

typedef enum shm_row_state
{
  ROW_NORMAL  = 0,
  ROW_DELETED = 1
} shm_row_state_t;

typedef uint32_t shm_value_t;


#pragma pack(push ,2)

/** @struct hash_pair
 * Only for values that have a certain constant
 * size because the implementation of the list
 * requires a specific size for the values
 */
typedef struct hash_pair
{
  shm_row_state_t del; /**< deleted flag */
  node_s*  l;   /**< List of values */
} hash_pair_t;

#pragma pack (pop)

typedef struct stupid_hashmap
{
  array_t rows; /** array of hash_pair structs */
} stupid_hashmap_t;

#ifdef __GNUC__
uint32_t stupid_hash ( void* key, size_t key_sz ) __attribute__( ( nonnull( 1 ) ) );
int  sht_init (stupid_hashmap_t* table) __attribute__( ( nonnull( 1 ) ) );
void sht_clear(stupid_hashmap_t* table) __attribute__( ( nonnull( 1 ) ) );
int  sht_insert(stupid_hashmap_t* table, void* key, size_t key_sz, shm_value_t* val) __attribute__( ( nonnull( 1, 2, 4 ) ) );
#else // __GNUC__
uint32_t stupid_hash ( void* key, size_t key_sz );
int  sht_init (stupid_hashmap_t* table);
void sht_clear(stupid_hashmap_t* table);
int  sht_insert(stupid_hashmap_t* table, void* key, size_t key_sz, shm_value_t* val);
#endif// __GNUC__


#endif //!__STUPID_HASHMAP_INCLUDED__

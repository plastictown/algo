#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <zlib.h>

#include <hdr/stupid-hashmap.h>
#include <hdr/util.h>

uint32_t stupid_hash ( void* key, size_t key_sz )
{
  uint32_t crc = crc32(0L, Z_NULL, 0);
  uint8_t* p = (uint8_t*)key;
  for(size_t i = 0u; i < key_sz; i++)
  {
    crc = crc32(crc, p + i, 1u);
  }

  return (crc % STUPID_HASHMAP_MAX);
}

int sht_init(stupid_hashmap_t* table)
{
  if(table == NULL)
    return SHT_FAIL;

  int rv = array_init_n(&table->rows, sizeof(hash_pair_t), STUPID_HASHMAP_MAX);
  if(rv != ARRAY_RET_OK)
  {
#ifdef STDIO_OUT
    printf("can't init stupid hash_map\n");
#endif //! STDIO_OUT
    return SHT_FAIL;
  }
  hash_pair_t hp;
  hp.del = ROW_NORMAL;
  hp.l   = NULL; // empty list
  for(size_t i = 0u; i < STUPID_HASHMAP_MAX; i++)
  {
    int rv = array_push_back(&table->rows, &hp);
    if(rv != ARRAY_RET_OK)
    {
#ifdef STDIO_OUT
      printf("can't init stupid hash_map\n");
#endif //! STDIO_OUT
      array_clear(&table->rows);
      return SHT_FAIL;
    }
  }
  return SHT_OK;
}

void sht_clear(stupid_hashmap_t* table)
{
  if(table == NULL)
    return;

  // cleaning lists
  for(size_t i = 0u; i < table->rows.len; i++)
  {
    hash_pair_t* pp = array_get_at(&table->rows, i);
    if(pp != NULL)
    {
      list_clear(&pp->l);
    }
  }
  array_clear(&table->rows);
}

int sht_insert(stupid_hashmap_t* table, void* key, size_t key_sz, shm_value_t* val)
{
  if(table   == NULL  ||
      key    == NULL  ||
      key_sz == 0u    ||
      val    == NULL
    ) return SHT_FAIL;

  uint32_t hash  = stupid_hash(key, key_sz);
  hash_pair_t* p = array_get_at(&table->rows, hash);
  if(p == NULL)
  {
    // TODO: print error message
    return SHT_FAIL;
  }

  // looks like the row is occuped
  if(p->del != ROW_DELETED && p->l != NULL)
  {
    if(list_size((p->l)) != 0u)
    {
      // row is occuped !!! AAA-AA-A-AA!!
      // I can add a new value or return an error
      // Need to think on it
    }
    // TODO
  }

  if( p->del == ROW_DELETED )
  {
    list_clear(&(p->l));
    p->del = ROW_NORMAL;
    p->l = NULL;
  }

  list_add(&(p->l), *val);

  // TODO

  return SHT_OK;

}

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>
#include <hdr/array.h>
#include <hdr/stupid-hashmap.h>
#include <zlib.h>

int main( void )
{

	//---create hashmap---//
  stupid_hashmap_t hm;
  int res = sht_init(&hm);
  if(res != SHT_OK )
  	{
  		printf("init_sht() - failed\n");
  		return EXIT_FAILURE;
  	}
  printf("init_sht() - ok\n");
  printf("there are %u rows in table\n", hm.rows.len);
  printf("table capacity is %u items\n", hm.rows.cap);

  char* key = "bla-bla";
  uint32_t val = 42u;

  //---insert pair into hashmap---//

  int rv = sht_insert(&hm, key, strlen(key), &val);
  if( rv != SHT_OK )
  	{
  		printf("sht_insert() - failed\n");
  		return EXIT_FAILURE;
  	}

  printf("sht_insert() - ok\n");

  //---get item by key (not implemented yet)---//

  uint32_t hash = stupid_hash(key, strlen(key));
  hash_pair_t* p = array_get_at(&hm.rows, hash);
  size_t lsz = list_size(p->l);
  printf("hash: %u; list size: %u\n", hash, lsz);

  if(lsz > 0u)
  	{
  		printf("the first item of the list: %u\n", p->l->payload);
  	}

  //---cleanup---//

  sht_clear(&hm);

  return EXIT_SUCCESS;
}

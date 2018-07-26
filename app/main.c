#include <stdio.h>

#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>
#include <hdr/array.h>
#include <hdr/stupid-hashmap.h>

int main( void )
{
  // init array
  array_t a;
  int rv = array_init( &a, sizeof( size_t ) );
  if( rv != ARRAY_RET_OK )
    {
      printf( "array_init_n() - failed\n" );
      return EXIT_FAILURE;
    }

  stupid_hashmap_t hm; // not implemented yet

  array_clear( &a );
  return 0;
}

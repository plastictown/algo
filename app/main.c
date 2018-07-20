#include <stdio.h>

#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>
#include <hdr/array.h>

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

  //add items
  for( size_t i = 1; i <= 20u; i++ )
    {
      int rv = array_push_back( &a, &i );
      if( rv != ARRAY_RET_OK )
        {
          printf( "array_push_back() - failed\n" );
          return EXIT_FAILURE;
        }
    }
  // set values
  for( size_t i = 0; i < 20u; i++ )
    {
      int rv = array_set_at( &a, &i, i );
      if( rv != ARRAY_RET_OK )
        {
          printf( "array_set_at() - failed\n" );
          return EXIT_FAILURE;
        }
    }

    array_reverse(&a);

  for(size_t i = 0u; i < a.len; i++){
    size_t* el = array_get_at(&a, i);
    printf(" %u", *el);
  }
  printf("\n");

  array_clear( &a );
  return 0;
}

#include <stdio.h>

#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>
#include <hdr/array.h>

/*
void print_list(node_s* l)
{
  node_s* ptr = l;
  do
  {
    printf(" %d", ptr->payload);
  }
  while( (ptr = ptr->next) != NULL );
  printf("\n");
}
*/

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

  size_t len = a.len;

  for( size_t k = 0u; k < len; k++ )
    {
      array_pop_back( &a );
      // get & print values
      for( size_t i = 0; i < a.cap; i++ )
        {
          size_t* el = array_get_at( &a, i );
          if( el != NULL )
            printf( " %u", *el );
        }
      printf( "\n[%u, %u]\n", a.len, a.cap );
    }

  array_clear( &a );
  return 0;
}

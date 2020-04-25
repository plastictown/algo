#include <stdio.h>

#include <hdr/search.h>

int* bsrc( int* src, size_t l, size_t r, int val )
{
  if( src == NULL )
    return NULL;

  if( r < l )
    return NULL;

  size_t m = l + ( r - l ) / 2;

  if( val < src[m] )
    return bsrc( src, l, m - 1, val );
  else if( val > src[m] )
    return bsrc( src, m + 1, r, val );
  else
    return ( src + m );

  return NULL;
}

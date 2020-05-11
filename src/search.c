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

int* bsrc_nr(int* src, size_t size, int val)
{
  if (src == NULL)
    return NULL;

  if (size < 1)
    return NULL;

  size_t l = 0;
  size_t r = size - 1;

  while (r >= l)
  {
    size_t m = l + (r - l) / 2;
    if (src[m] == val)
      return src + m;
    else if (src[m] > val)
    {
      r = m - 1;
      continue;
    }
    else if (src[m] < val)
    {
      l = m + 1;
      continue;
    }
  }
  return NULL;
}
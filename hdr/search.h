#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <stdlib.h>

// recursive
int* bsrc( int* src, size_t l, size_t r, int val );

// non-recursive
int* bsrc_nr(int* src, size_t size, int val);

#endif //!__SEARCH_H__

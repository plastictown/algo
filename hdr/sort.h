#ifndef __SORT_H__
#define __SORT_H__

#include <stdlib.h>

void bubble_sort( int* src, size_t sz );

void quicksort( int* src, size_t l, size_t r );
size_t quicksort_partition( int* src, size_t l, size_t r );

#endif //__SORT_H__

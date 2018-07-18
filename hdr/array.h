#ifndef __ARRAY_H_INCLUDED__
#define __ARRAY_H_INCLUDED__

#include <stdlib.h>

#define ARRAY_ALLOC_POINT 2
#define ARRAY_RET_OK      0
#define ARRAY_RET_FAIL   -1

typedef struct Array
{
  void*  data;
  size_t len;
  size_t cap;
  size_t item_sz;
} array_t;

int  array_init   (array_t* src, size_t item_size);
int  array_init_n (array_t* src, size_t item_size, size_t capacity);
void array_clear  (array_t* src);
int  array_resize (array_t* src, size_t new_cap);
int  array_set_at (array_t* src, void* item, size_t pos);
void* array_get_at(array_t* src, size_t pos);
int  array_push_back(array_t* src, void* item);

#endif // __ARRAY_H_INCLUDED__

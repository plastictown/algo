#include <stdlib.h>
#include <string.h>

#include <hdr/array.h>

int array_init(array_t* src, size_t item_size)
{
  if(src == NULL  ) return ARRAY_RET_FAIL;
  if(item_size < 1) return ARRAY_RET_FAIL;

  src->cap     = 0u;
  src->data    = NULL;
  src->item_sz = item_size;
  src->len     = 0u;

  return ARRAY_RET_OK;
}

int  array_init_n(array_t* src, size_t item_size, size_t capacity)
{
  if(src == NULL  ) return ARRAY_RET_FAIL;
  if(item_size < 1) return ARRAY_RET_FAIL;

  if(capacity == 0u)
  {
    src->cap     = 0u;
    src->data    = NULL;
    src->item_sz = item_size;
    src->len     = 0u;
    return ARRAY_RET_OK;
  }

  src->item_sz = item_size;
  src->data    = calloc(item_size, capacity);
  if(src->data == NULL) return ARRAY_RET_FAIL;
  src->cap = capacity;
  src->len = 0u;
  return ARRAY_RET_OK;
}

void array_clear(array_t* src)
{
  if(src == NULL) return;
  if(src->data == NULL)
  {
    src->cap = 0u;
    src->len = 0u;
    return;
  }
  free(src->data);
  src->cap = 0u;
  src->data = NULL;
  src->len = 0u;
}

int array_resize(array_t* src, size_t new_cap)
{
  if(src  == NULL) return ARRAY_RET_FAIL;
  if(new_cap == src->cap) return ARRAY_RET_OK;
  if(new_cap == 0u)
  {
    array_clear(src);
    return ARRAY_RET_OK;
  }

void* mem = realloc(src->data, new_cap*src->item_sz);
   if(mem == NULL)
    return ARRAY_RET_FAIL;
  src->data = mem;
  src->cap  = new_cap;
  if(new_cap < src->len)
    src->len = new_cap;
  else // TODO: make function array_fill(array_t* src, size_t l, size_t r, void* val);
  {
    unsigned char* p = (unsigned char*)src->data;
    size_t offset = src->len*src->item_sz;
    size_t sz = (new_cap - src->len)*src->item_sz;
    memset(p + offset, 0, sz);
  }
  return ARRAY_RET_OK;
}

int array_push_back(array_t* src, void* item)
{
  if(src  == NULL) return ARRAY_RET_FAIL;
  if(item == NULL) return ARRAY_RET_FAIL;
  if(src->item_sz < 1u) return ARRAY_RET_FAIL;

  if(src->data == NULL)
  {
    size_t new_len = ARRAY_ALLOC_POINT;
    int ret = array_init_n(src, src->item_sz, new_len);
    if( ret != ARRAY_RET_OK) return ARRAY_RET_FAIL;
    memcpy(src->data, item, src->item_sz);
    src->len = 1u;
    return ARRAY_RET_OK;
  }

  size_t new_len = src->len + 1u;
  if(new_len > src->cap)
  {
    int rv = array_resize(src, new_len*ARRAY_ALLOC_POINT);
    if(rv != ARRAY_RET_OK) return rv;
  }
  unsigned char* p = (unsigned char*)src->data;
  size_t offset = src->item_sz*src->len;
  memcpy(p+offset, item, src->item_sz);
  src->len++;
  return ARRAY_RET_OK;
}

void* array_get_at(array_t* src, size_t pos)
{
  if( src == NULL )
    return NULL;
  if(src->data == NULL ||
     src->cap  == 0u   ||
     pos >= src->len
     )
      return NULL;
  size_t offset = src->item_sz*pos;
  char* p = (char*)(src->data );
  return    (void*)(p + offset);
}

int array_set_at(array_t* src, void* item, size_t pos)
{
  if( src == NULL || item == NULL)
    return ARRAY_RET_FAIL;
  if(src->data == NULL ||
     src->cap  == 0u   ||
     pos >= src->len
     )
      return ARRAY_RET_FAIL;

  size_t offset = src->item_sz*pos;
  char* p = (char*)(src->data);
  memcpy(p + offset, item, src->item_sz);

  return ARRAY_RET_OK;
}

size_t array_find_first (array_t* src, void* item)
{
  // bad pointer(s)
  if( src  == NULL || item == NULL)
    return ARRAY_NPOS;

  if( src->data    == NULL ||
      src->cap     == 0u   ||
      src->len     == 0u   ||
      src->item_sz < 1u)
    return ARRAY_NPOS;

  unsigned char* p = (unsigned char*) src->data;
  size_t idx = 0u;

  while(idx < src->len)
  {
    size_t* ptr = p;
    int rv = memcmp(p, item, src->item_sz);
    if( rv == 0 )
      return idx;
    p += src->item_sz;
    idx++;
  }
  return ARRAY_NPOS;
}

#ifndef __ARRAY_H_INCLUDED__
#define __ARRAY_H_INCLUDED__

#include <stdlib.h>

#define ARRAY_ALLOC_POINT 2
#define ARRAY_RET_OK      0
#define ARRAY_RET_FAIL   -1

#define ARRAY_NPOS (size_t)(-1)

typedef struct Array
{
  void*  data;
  size_t len;
  size_t cap;
  size_t item_sz;
} array_t;

#ifdef __GNUC__
int  array_init     ( array_t* src, size_t item_size ) __attribute__( ( nonnull( 1 ) ) );
int  array_init_n   ( array_t* src, size_t item_size, size_t capacity ) __attribute__( ( nonnull( 1 ) ) );
void array_clear    ( array_t* src ) __attribute__( ( nonnull( 1 ) ) );
int  array_resize   ( array_t* src, size_t new_cap ) __attribute__( ( nonnull( 1 ) ) );
int  array_set_at   ( array_t* src, void* item, size_t pos ) __attribute__( ( nonnull( 1, 2 ) ) );
void*  array_get_at ( array_t* src, size_t pos ) __attribute__( ( nonnull( 1 ) ) );
int  array_push_back( array_t* src, void* item ) __attribute__( ( nonnull( 1, 2 ) ) );
void array_pop_back ( array_t* src ) __attribute__( ( nonnull( 1 ) ) );
void array_reverse  ( array_t* src ) __attribute__( ( nonnull( 1 ) ) );
int  array_erase    ( array_t* src, size_t pos ) __attribute__( ( nonnull( 1 ) ) );
int  array_swap     ( array_t* src, size_t first, size_t second ) __attribute__( ( nonnull( 1 ) ) );
size_t array_find_first ( array_t* src, void* item ) __attribute__( ( nonnull( 1, 2 ) ) );
#else  // __GNUC__
int  array_init     (array_t* src, size_t item_size);
int  array_init_n   ( array_t* src, size_t item_size, size_t capacity );
void array_clear    ( array_t* src );
int  array_resize   ( array_t* src, size_t new_cap );
int  array_set_at   ( array_t* src, void* item, size_t pos );
void*  array_get_at ( array_t* src, size_t pos );
int  array_push_back( array_t* src, void* item );
void array_pop_back ( array_t* src );
void array_reverse  ( array_t* src );
int  array_erase    ( array_t* src, size_t pos ); //TODO
size_t array_find_first ( array_t* src, void* item );
int  array_swap     ( array_t* src, size_t first, size_t second );
#endif // __GNUC__

#endif // __ARRAY_H_INCLUDED__

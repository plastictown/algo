#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

#ifdef STDIO_OUT
#include <stdio.h>
#define PRINT(v,f) printf(#v" = %"#f"\n",v)
#else
#define PRINT(v,f)
#endif //STDIO_OUT

void swap( void* a, void* b, size_t sz );
int is_palindrome( const char* str );

#endif //__UTIL_H__

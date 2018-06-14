#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

#ifdef STDIO_OUT
  #include <stdio.h>
  #define print(x) printf((x))
#else
  #define print(x)
#endif //STDIO_OUT

void swap(void* a, void* b, size_t sz);

#endif //__UTIL_H__

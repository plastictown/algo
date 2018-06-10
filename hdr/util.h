#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef STDIO_OUT
  #include <stdio.h>
  #define print(x) printf((x))
#else
  #define print(x) 
#endif //STDIO_OUT

#endif //__UTIL_H__
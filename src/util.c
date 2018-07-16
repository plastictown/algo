#include <string.h>
#include <hdr/util.h>
#include <ctype.h>

void swap(void* a, void* b, size_t sz)
{
    char* c = calloc(1, sz);
    memcpy(c, a, sz);
    memcpy(a, b, sz);
    memcpy(b, c, sz);

    free(c);
}

int is_palindrome( const char* str)
{
  int res = 1;
  size_t sz = strlen(str);
  size_t half = sz/2;
  size_t l =0u, r = sz - 1u;
  for(int i=0; i<= half; i++)
  {
    while(!isalnum(str[l]))l++;
    while(!isalnum(str[r]))r--;

    if(l>r) break;

    if(toupper(str[l])!=toupper(str[r]))
    {
      res = 0;
      break;
    }
  }
  return res;
}
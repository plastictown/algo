#include <stdio.h>
#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>

void print_list(node_s* l)
{
  node_s* ptr = l;
  do
  {
    printf(" %d", ptr->payload);
  }
  while( (ptr = ptr->next) != NULL );
  printf("\n");
}

int* bsrc(int* src, size_t l, size_t r, int val)
{
    if(src == NULL)
      return NULL;
    
    if( r < l )
      return NULL;
    
    size_t m = l + (r - l)/2;
    
    if(val < src[m])
      return bsrc(src, l, m-1, val);
    else if(val > src[m])
      return bsrc(src, l+1, r, val);
    else
      return (src + m);
    
  return NULL;
}

int main(int argc, char** argv)
{
  int a[] = {1,2};
  size_t len = 7;
  int* e  = bsrc(a, 0, len - 1 , 3);
  if(e != NULL)
   printf("%d\n", *e);
 else
   printf("not found\n");
	return 0;
}

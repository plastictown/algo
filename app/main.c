#include <stdio.h>
#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>

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

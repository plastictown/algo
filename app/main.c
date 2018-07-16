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



int main(int argc, char** argv)
{
  node_s* l = NULL;
  for(uint32_t i = 1u; i <= 20u; i++)
    list_add(&l, i);

  print_list(l);

  list_reverse(&l);

  print_list(l);

	return 0;
}

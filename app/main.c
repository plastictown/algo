#include <stdio.h>
#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>

int main(int argc, char** argv)
{
  int data = 42;
    listd l;
    listd_init(&l, sizeof(int));
    for(int i=0; i< 10; i++)
      listd_add_new(&l, &i);
    printf("tail: %d\n", *(int*)l.tail->data );
    printf("count: %d\n", listd_count(&l));
    listd_clear(&l);

	return 0;
}

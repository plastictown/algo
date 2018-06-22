#include <stdio.h>
#include <hdr/util.h>
#include <hdr/sort.h>

int main(int argc, char** argv)
{
	print("--algo--\n");

	int buf[]={3,1,2,5,6,7,-1};
    quicksort(buf,0,6);

    for(size_t i=0; i<7; i++)
    {
        printf("%d ", buf[i]);
    }

	return 0;
}

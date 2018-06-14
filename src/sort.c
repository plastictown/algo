#include <stdlib.h>
#include <hdr/util.h>

#include <hdr/sort.h>

void bubble_sort(int* src, size_t sz)
{
    if(src == NULL)
        return;
    if(sz < 1)
        return;
    for(size_t i=0; i<sz; i++)
        for(size_t j=0; j<sz-1; j++)
            if(src[j]>src[j+1])
                swap(&src[j], &src[j+1], sizeof(int));
}

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

void quicksort(int* src, size_t l, size_t r)
{
    if(src == NULL)
        return;

    if(l < r)
    {
        int q = quicksort_partition(src, l, r);
        quicksort(src, l, q);
        quicksort(src, q+1, r);
    }
}
size_t quicksort_partition(int* src, size_t l, size_t r)
{
    int o = src[(l+r)/2];
    size_t i = l;
    size_t j = r;

    while(i<=j)
    {
        while(src[i] < o)
            i++;
        while(src[j] > o)
            j--;
        if(i>=j)
            break;
        swap(&src[i++], &src[j--],sizeof(int));
    }
    return j;
}

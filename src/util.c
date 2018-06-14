#include <string.h>
#include <hdr/util.h>

void swap(void* a, void* b, size_t sz)
{
    char* c = calloc(1, sz);
    memcpy(c, a, sz);
    memcpy(a, b, sz);
    memcpy(b, c, sz);

    free(c);
}

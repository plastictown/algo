#ifndef __LIST_H_INCLUDED__
#define __LIST_H_INCLUDED__

#include <stdint.h>
#include <stdlib.h>

typedef struct noded_s
{
  struct noded_s* next;
  uint32_t payload;
} node_s;

node_s* list_get_last( node_s* head );
node_s* list_add( node_s** phead, uint32_t value );
int list_remove_last( node_s** phead );
size_t list_size( node_s* head );
void list_clear( node_s** phead );
const node_s* list_find( node_s* head, uint32_t value );
void list_reverse( node_s** phead );

#endif // __LIST_H_INCLUDED__

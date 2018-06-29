#include <string.h>
#include <hdr/listd.h>

void listd_init( listd* src,  size_t sz)
{
  if( src == NULL) return;
  src->sz = sz;
  src->count = 0u;

  src->head = NULL;
  src->tail = NULL;
}

node_d* listd_alloc_node(size_t sz)
{
  if(sz == 0)
    return NULL;

  node_d* n = (node_d*) calloc(1, sizeof(node_d));
  if( n == NULL )
    return NULL;
  n->data = calloc(1, sz);
  if( n->data == NULL )
  {
    free(n);
    return NULL;
  }
  return n;
}

size_t listd_count( listd* src )
{
  if( src != NULL )
    return src->count;
  return 0;
}

int listd_add_empty( listd* src )
{
  if( src == NULL )
    return -1;
  // list is empty
  if( src->count == 0u)
    {
      src->head = listd_alloc_node(src->sz);
      if( src->head == NULL )
        return -1;
      src->head->prev = NULL;
      src->head->next = NULL;
      src->tail = src->head;
      (src->count)++;
      return 0;
    }
  node_d* n = listd_alloc_node(src->sz);
  if( n == NULL )
    return -1;
  n->prev = src->tail;
  n->next = NULL;
  src->tail->next = n;
  src->tail = n;
  (src->count)++;
  return 0;
}

void _listd_free_node( node_d* n)
{
  if( n == NULL)
    return;
  if(n->data != NULL)
    free (n->data);
  free(n);
}

void listd_remove_last( listd* src )
{
  if( src == NULL )
    return;
  if( src->count == 0u )
    return;
  node_d* _tail = src->tail;
  if(src->count > 1u)
    ((node_d*)(src->tail->prev))->next = NULL;
  src->tail = src->tail->prev;
  (src->count)--;
  _listd_free_node(_tail);
  _tail = NULL;
}

void listd_clear( listd* src )
{
  if(src == NULL)
    return;
  if( src->count == 0u )
    return;
  while( src->count > 0)
    listd_remove_last( src );
}

int listd_add_new( listd* src, void* data )
{
  if( src == NULL || data == NULL)
    return -1;
  int res = listd_add_empty( src );
  if( res != 0)
    return -1;
  memcpy( src->tail->data, data, src->sz);
  return 0;
}

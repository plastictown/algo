#ifndef __LISTD_H_INCLUDED__
#define __LISTD_H_INCLUDED__

#include <stdlib.h>

typedef struct noded_t
{
  struct node_d* prev;
  struct node_d* next;
  void*   data;
} node_d;

typedef struct listd_t
{
  size_t sz;
  size_t count;
  node_d* head;
  node_d* tail;
} listd;

void listd_init( listd* src, size_t sz );
void listd_init_n( listd* src, size_t sz, size_t n );//TODO
int listd_add_empty( listd* src );
node_d* listd_alloc_node( size_t sz );
void _listd_free_node( node_d* n );
void listd_remove_last( listd* src );
void listd_clear( listd* src );
int listd_add_new( listd* src, void* data );
size_t listd_count( listd* src );


#endif // __LISTD_H_INCLUDED__

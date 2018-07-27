#include <stdlib.h>
#include <stdint.h>

#include <hdr/list.h>

node_s* list_add( node_s** phead, uint32_t value )
{
  if ( phead == NULL )
    return NULL;
  if ( *phead == NULL )
  {
    *phead = ( node_s* )calloc( 1, sizeof( node_s ) );
    ( *phead )->next = NULL;
    ( *phead )->payload = value;
    return ( *phead );
  }
  else
  {
    node_s* last = list_get_last( *phead );
    if ( last == NULL )
      return NULL;
    last->next = ( node_s* )calloc( 1, sizeof( node_s ) );
    last->next->next = NULL;
    last->next->payload = value;
    return last->next;
  }
  return NULL;
}

node_s* list_get_last( node_s* head )
{
  if ( head == NULL )
    return NULL;
  node_s* ptr = head;
  while ( ptr->next != NULL )
    ptr = ptr->next;
  return ptr;
}

int list_remove_last( node_s** phead )
{
  if ( phead == NULL )
    return 0;
  node_s* head = *phead;
  if ( head == NULL )
    return 0;
  if ( head->next == NULL )
  {
    free( head );
    *phead = NULL;
    return 0;
  }
  node_s* ptr = head;
  node_s* prev = NULL;
  while ( ptr->next != NULL )
  {
    prev = ptr;
    ptr = ptr->next;
  }
  free( ptr );
  prev->next = NULL;
  return 1;
}

void list_clear( node_s** phead )
{
  if ( phead == NULL )
    return;
  while ( list_remove_last( phead ) );
}

size_t list_size( node_s* head )
{
  if ( head == NULL )
    return 0u;
  size_t sz = 0u;
  node_s* ptr = head;
  do
  {
    sz++;
  }
  while ( ( ptr = ptr->next ) != NULL );
  return sz;
}

const node_s* list_find( node_s* head, uint32_t value )
{
  if ( head == NULL )
    return NULL;
  node_s* ptr = head;
  do
  {
    if ( ptr->payload == value )
      return ptr;
  }
  while ( ( ptr = ptr->next ) != NULL );
  return NULL;
}

void list_reverse( node_s** phead )
{
  if( phead == NULL )
    return;
  // list is empty
  node_s* head = *phead;
  if( head == NULL )
    return;
  // one element in the list
  if( head->next == NULL )
    return;

  node_s* next = NULL;
  node_s* prev = NULL;
  node_s* cur  = head;

  while( cur != NULL )
  {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  *phead = prev;
}

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>

#include <hdr/util.h>
#include <hdr/sort.h>
#include <hdr/listd.h>
#include <hdr/list.h>
#include <hdr/search.h>
#include <hdr/array.h>
#include <hdr/stupid-hashmap.h>
#include <zlib.h>

void sigsegv_handler( int signum );
void sigterm_handler( int signum );
void sigint_handler( int signum );
void print_error( const char* pstr, size_t len );

int main( void )
{
  signal( SIGSEGV, sigsegv_handler );
  signal( SIGTERM, sigterm_handler );
  signal( SIGINT,  sigint_handler );

  // SIGSEGV test
  //int* arr = calloc(1, sizeof(int));
  //arr[50000] = 10;

  //---create hashmap---//
  stupid_hashmap_t hm;
  int res = sht_init( &hm );
  if( res != SHT_OK )
    {
      printf( "init_sht() - failed\n" );
      return EXIT_FAILURE;
    }
  printf( "init_sht() - ok\n" );
  printf( "there are %u rows in table\n", hm.rows.len );
  printf( "table capacity is %u items\n", hm.rows.cap );

  char* key = "bla-bla";
  uint32_t val = 42u;

  //---insert pair into hashmap---//

  int rv = sht_insert( &hm, key, strlen( key ), &val );
  if( rv != SHT_OK )
    {
      printf( "sht_insert() - failed\n" );
      return EXIT_FAILURE;
    }

  printf( "sht_insert() - ok\n" );

  //---get item by key (not implemented yet)---//

  uint32_t hash = stupid_hash( key, strlen( key ) );
  hash_pair_t* p = array_get_at( &hm.rows, hash );
  size_t lsz = list_size( p->l );
  printf( "hash: %u; list size: %u\n", hash, lsz );

  if( lsz > 0u )
    {
      printf( "the first item of the list: %u\n", p->l->payload );
    }

  //---cleanup---//

  sht_clear( &hm );

  // Ctrl+C here to handle SIGINT
  while( 1 );

  return EXIT_SUCCESS;
}

void print_error( const char* pstr, size_t len )
{
  write( STDERR_FILENO, pstr, len );
}

void sigsegv_handler( int signum )
{
  char* err_str = "SIGSEGV received\n";
  size_t len = strlen( err_str );
  write( STDERR_FILENO, err_str, len );
  signal( signum, SIG_DFL );
  exit( signum );
}

void sigterm_handler( int signum )
{
  char* err_str = "SIGTERM received\n";
  size_t len = strlen( err_str );
  write( STDERR_FILENO, err_str, len );
  signal( signum, SIG_DFL );
  exit( signum );
}

void sigint_handler( int signum )
{
  char* err_str = "SIGINT received\n";
  size_t len = strlen( err_str );
  write( STDERR_FILENO, err_str, len );
  signal( signum, SIG_DFL );
  exit( signum );
}

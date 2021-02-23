#include "macros.h"
#include <stdio.h>
#include <stdlib.h>

/*Purpose: to malloc the given number of bytes and verify that
  malloc succeeded.
  Input: bytes - IN the number of bytes to malloc
  Return: a void pointer to malloced data.
 */
void *my_malloc(int bytes)
{
  void *ptr = malloc(bytes);
  
  if(ptr == NULL)
  {
    printf("Unable to allocate %d bytes of memory\n", bytes);
    exit(0);
  }

  return ptr;
}

/*Purpose: to realloc the given number of bytes and verify that
  realloc succeeded.
  Input:  ptr   - IN/OUT: the pointer to realloc
          bytes - IN the number of bytes to malloc
  Return: a void pointer to malloced data.
 */
void *my_realloc(void *ptr, int bytes)
{
  ptr = realloc(ptr, bytes);
  
  if(ptr == NULL)
  {
    printf("Unable to reallocate %d bytes of memory\n", bytes);
    exit(0);
  }

  return ptr;
}
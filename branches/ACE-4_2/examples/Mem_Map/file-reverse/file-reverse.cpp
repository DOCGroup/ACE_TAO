
// $Id$

#include "ace/Mem_Map.h"

static void
putline (const char *s)
{
  while (putchar (*s++) != '\n')
    continue;
}

static void
print_array_in_reverse (char *array, int size)
{
  size--;

  if (array[size] == '\0')
    array[size] = '\n';

  while (--size >= 0)
    if (array[size] == '\n')
      putline (array + size + 1);

  putline (array);
}

int
main (int argc, char **argv)
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc != 2)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %n file\n%a"), -1);

  ACE_Mem_Map mmap;
  
  if (mmap.map (argv[1]) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%n: %p\n%a", "mmap"), -1);

  print_array_in_reverse ((char *) mmap.addr (), mmap.size ());

  return 0;
}

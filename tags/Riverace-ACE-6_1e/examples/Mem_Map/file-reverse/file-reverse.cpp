// $Id$

// Print a file in reverse by using the ACE memory mapped file
// wrapper.  It is SO easy to do compared with alternatives!

#include "ace/OS_main.h"
#include "ace/Mem_Map.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"



static void
putline (const char *s)
{
  while (putchar (*s++) != '\n')
    continue;
}

static void
print_array_in_reverse (char *array,
                        int size)
{
  if (size <= 0)
    return;

  size--;

  if (array[size] == '\0')
    array[size] = '\n';

  while (--size >= 0)
    if (array[size] == '\n')
      putline (array + size + 1);

  putline (array);
}

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_LOG_MSG->open (argv[0]);

  if (argc != 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %n file\n"),
                      -1);

  ACE_Mem_Map mmap;

  if (mmap.map (argv[1], static_cast<size_t> (-1), O_RDWR) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%n: %p\n",
                       "mmap"),
                      -1);

  print_array_in_reverse ((char *) mmap.addr (),
                          ACE_Utils::truncate_cast<int> (mmap.size ()));
  return 0;
}

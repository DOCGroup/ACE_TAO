// $Id$

#include "ace/Service_Config.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Misc, test_read_buffer, "$Id$")

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_HANDLE handle = argc > 1 ? ACE_OS::open (argv[1], O_RDONLY) : 0;
  int term = argc > 2 ? ACE_OS::atoi (argv[2]) : EOF;
  int search = argc > 3 ? ACE_OS::atoi (argv[3]) : '\n';
  int replace = argc > 4 ? ACE_OS::atoi (argv[4]) : '\0';

  ACE_Read_Buffer rb (handle);

  char *buf;

  while ((buf = rb.read (term, search, replace)) != 0)
    {
      ACE_OS::write (ACE_STDOUT, buf, rb.size ());
      ACE_Allocator::instance ()->free (buf);
    }
  return 0;
}

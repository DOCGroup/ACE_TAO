// $Id$

#include "ace/Service_Config.h"
#include "ace/Read_Buffer.h"

int 
main (int argc, char *argv[])
{
  int handle = argc > 1 ? ACE_OS::open (argv[1], O_RDONLY) : 0;
  int term = argc > 2 ? ACE_OS::atoi (argv[2]) : EOF;
  int search = argc > 3 ? ACE_OS::atoi (argv[3]) : '\n';
  int replace = argc > 4 ? ACE_OS::atoi (argv[4]) : '\0';

  ACE_Read_Buffer rb (handle);
  
  char *buf;

  while ((buf = rb.read (term, search, replace)) != 0)
    {
      ACE_OS::write (1, buf, rb.size ());
      ACE_Service_Config::alloc ()->free (buf);
    }
  return 0;
}

#include "ace/Service_Config.h"
// $Id$

#include "ace/Read_Buffer.h"

int 
main (int argc, char *argv[])
{
  int handle = argc > 1 ? ::open (argv[1], O_RDONLY) : 0;
  int term = argc > 2 ? ::atoi (argv[2]) : EOF;
  int search = argc > 3 ? ::atoi (argv[3]) : '\n';
  int replace = argc > 4 ? ::atoi (argv[4]) : '\0';

  ACE_Read_Buffer rb (handle);
  
  char *buf;

  while ((buf = rb.read (term, search, replace)) != 0)
    {
      ::write (1, buf, rb.size ());
      ACE_Service_Config::alloc ()->free (buf);
    }
  return 0;
}

#if 0
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Stream<ACE_Null_Mutex, ACE_Null_Condition_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
#endif

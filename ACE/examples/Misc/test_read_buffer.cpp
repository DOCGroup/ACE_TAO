#include "ace/OS_main.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Service_Config.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdlib.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_HANDLE const handle = argc > 1 ? ACE_OS::open (argv[1], O_RDONLY) : ACE_STDIN;
  int const term = argc > 2 ? ACE_OS::atoi (argv[2]) : EOF;
  int const search = argc > 3 ? ACE_OS::atoi (argv[3]) : '\n';
  int const replace = argc > 4 ? ACE_OS::atoi (argv[4]) : '\0';

  ACE_Read_Buffer rb (handle, argc > 1);
  ACE_Read_Buffer_Unique_Ptr buffer (
    rb.read (term, search, replace), {rb.alloc ()});
  while (buffer != nullptr)
    {
      ACE_OS::write (ACE_STDOUT, buffer.get (), rb.size ());
      buffer.reset (rb.read (term, search, replace));
    }
  return 0;
}

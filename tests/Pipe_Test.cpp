#include "ace/Pipe.h"
#include "test_config.h"

void
open (ACE_Pipe &pipe, const char *name)
{
  if (pipe.open () == 0)
    ACE_DEBUG ((LM_DEBUG, "Pipe %s: successful open\n", name));
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Pipe %s: UNSUCCESSFUL open\n", name));
      if (pipe.read_handle () == ACE_INVALID_HANDLE)
	ACE_DEBUG ((LM_DEBUG, "Reader is invalid\n"));
      else
	ACE_DEBUG ((LM_DEBUG, "Reader is valid\n"));
      if (pipe.write_handle () == ACE_INVALID_HANDLE)
	ACE_DEBUG ((LM_DEBUG, "Writer is invalid\n"));
      else
	ACE_DEBUG ((LM_DEBUG, "Writer is valid\n"));

      ACE_OS::exit (-1);
    }     
}

int 
main (int, char *argv[])
{
  ACE_START_TEST ("Pipe_Test.cpp");

  ACE_Pipe a, b, c, d, e;

  ::open (a, "a");
  ::open (b, "b");
  ::open (c, "c");
  ::open (d, "d");
  ::open (e, "e");

  ACE_END_TEST;
  return 0;
}

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Pipe_Test.cpp
//
// = DESCRIPTION
//    Tests the construction of multiple pipes in a process.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Pipe.h"
#include "test_config.h"

void
open (ACE_Pipe &pipe, 
      const char *name, 
      int close_pipe = 1)
{
  if (pipe.open () == 0)
    ACE_DEBUG ((LM_DEBUG, "Pipe %s: successful open\n", name));
  else
    {
      ACE_DEBUG ((LM_DEBUG, "%p: UNSUCCESSFUL open for pipe %s\n", "pipe.open", name));
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
  if (close_pipe)
    pipe.close ();
}

int 
main (int, char *argv[])
{
  ACE_START_TEST ("Pipe_Test.cpp");

  ACE_Pipe a, b, c, d, e, f, g, h, i, j;

  ::open (a, "a");
  ::open (b, "b");
  ::open (c, "c");
  ::open (d, "d");
  ::open (e, "e");
  ::open (f, "f");
  ::open (g, "g");
  ::open (h, "h");
  ::open (i, "i");
  ::open (j, "j");

  ACE_END_TEST;
  return 0;
}

// $Id$

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
#include "ace/Process.h"
#include "test_config.h"

static void
open (ACE_Pipe &pipe, 
      const char *name, 
      int close_pipe = 1)
{
  ACE_DEBUG ((LM_DEBUG, "opening %s\n", name));
  ACE_ASSERT (pipe.open () != -1);
  ACE_ASSERT (pipe.read_handle () != ACE_INVALID_HANDLE
	      && pipe.write_handle () != ACE_INVALID_HANDLE);

  if (close_pipe)
    pipe.close ();
}

int 
main (int argc, char *argv[])
{
  ACE_START_TEST ("Pipe_Test.cpp");

  if (argc > 1)
    {
      ACE_Pipe a, b, c, d, e;

      open (a, "a");
      open (b, "b");
      open (c, "c");
      open (d, "d");
      open (e, "e");
    }
  else
    {
      char *s_argv[3];
      s_argv[0] = "Pipe_Test" ACE_PLATFORM_EXE_SUFFIX;
      s_argv[1] = "-r"; // This is just a dummy.
      s_argv[2] = 0;

      for (int i = 0; i < ACE_MAX_ITERATIONS; i++)
	{
	  ACE_Process server;

	  ACE_ASSERT (server.start (s_argv) != -1);

	  ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", server.getpid ()));

	  // Wait for the process we just created to exit.
	  server.wait ();
	  ACE_DEBUG ((LM_DEBUG, "Server %d finished\n", server.getpid ()));
	}
    }

  ACE_END_TEST;
  return 0;
}

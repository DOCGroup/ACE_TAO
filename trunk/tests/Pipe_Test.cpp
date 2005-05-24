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
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Pipe.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(tests, Pipe_Test, "$Id$")

static int close_pipe = 1;
static int child_process = 0;
static int iterations = ACE_MAX_ITERATIONS;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "usage: %n [-d (don't close pipes)] [-c (child process)] [-i (iterations)] \n"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("dci:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'd':
      close_pipe = 0;
      break;
    case 'c':
      child_process = 1;
      break;
    case 'i':
      iterations = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

static void
open (ACE_Pipe &pipe,
      const char *name)
{
  ACE_DEBUG ((LM_DEBUG, "opening %s\n", name));
  ACE_ASSERT (pipe.open () != -1);
  ACE_ASSERT (pipe.read_handle () != ACE_INVALID_HANDLE
              && pipe.write_handle () != ACE_INVALID_HANDLE);

  if (close_pipe)
    pipe.close ();
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  parse_args (argc, argv);

  if (child_process)
    {
      ACE_APPEND_LOG (ACE_TEXT("Pipe_Test-children"));
      ACE_Pipe a, b, c, d, e;

      open (a, "a");
      open (b, "b");
      open (c, "c");
      open (d, "d");
      open (e, "e");

      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT("Pipe_Test"));
      ACE_INIT_LOG (ACE_TEXT("Pipe_Test-children"));

      ACE_Process_Options options;
      options.command_line (ACE_TEXT ("%s -c%s"),
                            argv[0],
                            close_pipe == 0 ? ACE_TEXT (" -d") : ACE_TEXT (""));

      ACE_exitcode status = 0;

      for (int i = 0; i < ::iterations; i++)
        {
          ACE_Process server;

          ACE_ASSERT (server.spawn (options) != -1);

          ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n",
                      server.getpid ()));

          // Wait for the process we just created to exit.
          server.wait (&status);

          // Check if child exited without error.
          if (WIFEXITED (status) != 0
              && WEXITSTATUS (status) != 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Child of server %d finished with error "
                          "exit status %d\n",
                          server.getpid (),
                          WEXITSTATUS (status)));

              ACE_END_TEST;

              exit (WEXITSTATUS (status));
            }

          ACE_DEBUG ((LM_DEBUG, "Server %d finished\n", server.getpid ()));
        }
      ACE_END_TEST;
    }

  return 0;
}

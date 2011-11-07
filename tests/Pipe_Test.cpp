
//=============================================================================
/**
 *  @file    Pipe_Test.cpp
 *
 *  $Id$
 *
 *  Tests the construction of multiple pipes in a process.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Pipe.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

// Indicates whether we should close the pipe or not.
static int close_pipe = 1;

// Indicates whether we're running as the child or the parent.
static int child_process = 0;

// Number of iterations to run the test.
static int iterations = ACE_MAX_ITERATIONS;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-d (don't close pipes)] ")
              ACE_TEXT ("[-c (child process)] [-i (iterations)]\n")));
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

// Consolidate the ACE_Pipe initializations.

static void
open_pipe (ACE_Pipe &pipe,
      const char *name)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("opening %C\n"), name));
  int result = pipe.open ();

  ACE_TEST_ASSERT (result != -1);
  result = pipe.read_handle () != ACE_INVALID_HANDLE
    && pipe.write_handle () != ACE_INVALID_HANDLE;
  ACE_TEST_ASSERT (result == 1);

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

      open_pipe (a, "a");
      open_pipe (b, "b");
      open_pipe (c, "c");
      open_pipe (d, "d");
      open_pipe (e, "e");

      ACE_END_LOG;
    }
  else
    {
      ACE_START_TEST (ACE_TEXT("Pipe_Test"));
      ACE_INIT_LOG (ACE_TEXT("Pipe_Test-children"));

#  if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
      const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%s -c%s");
#  else
      const ACE_TCHAR *cmdline_fmt = ACE_TEXT ("%ls -c%ls");
#  endif /* ACE_WIN32 || !ACE_USES_WCHAR */
      ACE_Process_Options options;
      options.command_line (cmdline_fmt,
                            argc > 0 ? argv[0] : ACE_TEXT ("Pipe_Test"),
                            close_pipe == 0 ? ACE_TEXT (" -d") : ACE_TEXT (""));

      ACE_exitcode status = 0;

      for (int i = 0; i < ::iterations; i++)
        {
          ACE_Process server;

          if (server.spawn (options) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("%p\n"),
                                 ACE_TEXT ("spawn failed")),
                                -1);
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Server forked with pid = %d.\n"),
                          server.getpid ()));
            }

          // Wait for the process we just created to exit.
          server.wait (&status);

          // Check if child exited without error.
          if (WIFEXITED (status) != 0
              && WEXITSTATUS (status) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Child of server %d finished with error ")
                          ACE_TEXT ("exit status %d\n"),
                          server.getpid (),
                          WEXITSTATUS (status)));

              ACE_END_TEST;

              ACE_OS::exit (WEXITSTATUS (status));
            }

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server %d finished\n"),
                      server.getpid ()));
        }
      ACE_END_TEST;
    }

  return 0;
}

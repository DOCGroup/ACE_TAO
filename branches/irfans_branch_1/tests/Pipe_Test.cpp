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

#include "test_config.h"
#include "ace/Pipe.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"

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
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dci:");

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
      iterations = atoi (get_opt.optarg);
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
main (int argc, char *argv[])
{
  parse_args (argc, argv);

  if (child_process)
    {      
      ACE_APPEND_LOG ("Pipe_Test-children");      
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
      ACE_START_TEST ("Pipe_Test");
      ACE_INIT_LOG ("Pipe_Test-children");      

      ACE_Process_Options options;
      if (close_pipe == 0)
	options.command_line (__TEXT ("Pipe_Test") ACE_PLATFORM_EXE_SUFFIX __TEXT (" -c -d"));
      else
	options.command_line (__TEXT ("Pipe_Test") ACE_PLATFORM_EXE_SUFFIX __TEXT (" -c"));
      
      for (int i = 0; i < ::iterations; i++)
	{
	  ACE_Process server;

	  ACE_ASSERT (server.spawn (options) != -1);

	  ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", server.getpid ()));

	  // Wait for the process we just created to exit.
	  server.wait ();
	  ACE_DEBUG ((LM_DEBUG, "Server %d finished\n", server.getpid ()));
	}
      ACE_END_TEST;      
    }

  return 0;
}

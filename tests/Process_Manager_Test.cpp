// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Process_Manager_Test.cpp
//
// = DESCRIPTION
//      This program tests the various methods provided by
//      <ACE_Process_Manager>.  It illustrates both the explicit <wait>
//      functions and the Reactor-style auto-reaping.  There's an
//      Exit_Handler class that can print out (in Debug mode) when a
//      child has been reaped.
//
//      The child processes spawned are simply this program itself, with
//      an integer argument specifying how long to "process" (actually,
//      the child just sleeps for the specified length of time).
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu> and
//    Dave Madden <dhm@mersenne.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Thread.h"

ACE_RCSID(tests, Process_Manager_Test, "Process_Manager_Test.cpp,v 4.11 1999/09/02 04:36:30 schmidt Exp")

#if (!defined (ACE_LACKS_FORK) || defined (ACE_WIN32))

static u_int debug_test = 0;

class Exit_Handler : public ACE_Event_Handler
{
public:
  Exit_Handler (const char *msg): msg_ (msg) { }

  virtual ~Exit_Handler (void) { }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
  {
    delete this;
    return 0;
  }

  virtual int handle_exit (ACE_Process *proc)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Exit_Handler(%s) got %d: %d\n"),
                msg_,
                int (proc->getpid ()),
                int (proc->exit_code ()) ));
    return 0;
  }
private:
  const char *msg_;
};

static void
usage (const ACE_TCHAR *argv0)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("usage: %s [-d] [sleep-seconds]\n"),
              argv0));
  ACE_OS::exit (0);
}

static pid_t
spawn_child (const ACE_TCHAR *argv0,
             ACE_Process_Manager &mgr,
             int sleep_time = 0)
{
  ACE_Process_Options opts;

  opts.command_line (ACE_TEXT("%s %s %d"),
                     argv0,
                     debug_test ? ACE_TEXT ("-d") : ACE_TEXT (""),
                     sleep_time);

  pid_t  result = mgr.spawn (opts);

  if (result != ACE_INVALID_PID)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) spawned child: pid %d time %d\n"),
                int (result), sleep_time));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn failed")));

  return result;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt args (argc, argv, ACE_TEXT ("d"));

  for (int arg = args (); arg != EOF; arg = args ())
    switch (arg)
      {
      case 'd':
        debug_test = 1u;
        break;
      default:
        usage (argv[0]);
        break;
      }

  if (args.opt_ind () == argc - 1)
    {     // child process: sleep & exit
      int secs = ACE_OS::atoi (argv[args.opt_ind ()]);
      ACE_OS::sleep (secs ? secs : 1);
      if (debug_test)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%T: pid %P about to exit with code %d\n"),
                    secs));
      return secs;
    }

  if (args.opt_ind () != argc)      // incorrect usage
    usage (argv[0]);

  ACE_START_TEST (ACE_TEXT ("Process_Manager_Test"));

  // Try the explicit <ACE_Process_Manager::wait> functions

  int result = 0, test_status = 0;
  ACE_Process_Manager mgr;

  mgr.register_handler (new Exit_Handler ("default"));

  ACE_exitcode exitcode;

  // --------------------------------------------------
  // wait for a specific PID
  pid_t child1 = spawn_child (argv[0],
                              mgr,
                              1);
  result = mgr.wait (child1,
                     &exitcode);

  if (result != child1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected to reap child1 (%d); got %d\n"),
                  child1,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child1, pid %d: %d\n"),
                child1,
                exitcode));

  // --------------------------------------------------
  // wait for a specific PID; another should finish first
  pid_t child2 = spawn_child (argv[0],
                              mgr,
                              1);
  pid_t child3 = spawn_child (argv[0],
                              mgr,
                              4);
  result = mgr.wait (child3,
                     &exitcode);

  if (result != child3)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected to reap child3 (%d); got %d\n"),
                  child3,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 3, pid %d: %d\n"),
                child3,
                exitcode));

  // Now wait for any...should get the one that finished earlier.

  result = mgr.wait (0, &exitcode);

  if (result != child2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected to reap child2 (%d); got %d\n"),
                  child2,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 2, pid %d: %d\n"),
                result,
                exitcode));

  // --------------------------------------------------
  // Try the timed wait functions

  // This one shouldn't timeout:
  pid_t child4 = spawn_child (argv[0],
                              mgr,
                              1);
  result = mgr.wait (0, ACE_Time_Value (4), &exitcode);

  if (result != child4)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected to reap child4 (%d); got %d\n"),
                  child4,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 4 pid %d: %d\n"),
                result,
                exitcode));

  // This one should timeout:
  pid_t child5 = spawn_child (argv[0],
                              mgr,
                              4);
  result = mgr.wait (0, ACE_Time_Value (1), &exitcode);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected wait to time out; got %d\n"),
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) Correctly timed out wait at child 5\n")));

  // Now wait indefinitely to clean up...
  result = mgr.wait (0, &exitcode);

  if (result != child5)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: expected to reap child5 pid %d; got %d\n"),
                  child5,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 5, pid %d: %d\n"),
                result,
                exitcode));

  // --------------------------------------------------
  // Finally, try the reactor stuff...
  mgr.open (ACE_Process_Manager::DEFAULT_SIZE,
            ACE_Reactor::instance ());

  pid_t child6 = spawn_child (argv[0],
                              mgr,
                              5);
  /* pid_t child7 = */ spawn_child (argv[0],
                                    mgr,
                                    6);

  mgr.register_handler (new Exit_Handler ("specific"),
                        child6);

  ACE_Time_Value how_long (10);

  ACE_Reactor::instance ()->run_reactor_event_loop (how_long);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Reactor loop done!\n") ));

  size_t nr_procs = mgr.managed ();
  if (nr_procs != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P) %d processes left in manager\n"),
                nr_procs));

  ACE_END_TEST;
  return test_status;
}

#else /* (!ACE_LACKS_FORK || ACE_WIN32) */
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Process_Manager_Test"));
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("The ACE Process Manager is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* (!ACE_LACKS_FORK || ACE_WIN32) */

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

ACE_RCSID(tests, Process_Manager_Test, "Process_Manager_Test.cpp,v 4.11 1999/09/02 04:36:30 schmidt Exp")

#if (!defined (ACE_LACKS_FORK) || defined (ACE_WIN32)) && defined (ACE_HAS_THREADS)

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
    if (debug_test)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Exit_Handler(%s) got %d: %d\n"),
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

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) spawned child: %d\n"),
                int (result)));
  return result;
}

int
main (int argc, ACE_TCHAR *argv[])
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
                    ACE_TEXT ("(%P|%t) about to exit with code %d\n"),
                    secs));
      return secs;
    }

  if (args.opt_ind () != argc)      // incorrect usage
    usage (argv[0]);

  ACE_START_TEST (ACE_TEXT ("Process_Manager_Test"));

  // Try the explicit <ACE_Process_Manager::wait> functions

  int result = 0;
  ACE_Process_Manager mgr;

  mgr.register_handler (new Exit_Handler ("default"));

  ACE_exitcode exitcode;

  // --------------------------------------------------
  // wait for a specific PID
  pid_t child1 = spawn_child (argv[0],
                              mgr,
                              1);
  pid_t result1 = mgr.wait (child1,
                            &exitcode);

  ACE_ASSERT (result1 == child1);
  // ACE_ASSERT (exitcode == 1);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for child %d: %d\n"),
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
  pid_t result2 = mgr.wait (child3,
                            &exitcode);

  ACE_ASSERT (result2 == child3);
  // ACE_ASSERT (exitcode == 4);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for child %d: %d\n"),
                child3,
                exitcode));

  // Now wait for any...should get the one that finished earlier.

  pid_t result3 = mgr.wait (0,
                            &exitcode);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) expected to reap %d, got %d\n"),
              child2,
              result3));
  ACE_ASSERT (result3 == child2);
  // ACE_ASSERT (exitcode == 1);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for any child: %d\n"),
                exitcode));

  // --------------------------------------------------
  // Try the timed wait functions

  // This one shouldn't timeout:
  pid_t child4 = spawn_child (argv[0],
                              mgr,
                              1);
  pid_t result4 = mgr.wait (0,
                            ACE_Time_Value (4),
                            &exitcode);

  ACE_ASSERT (result4 == child4);
  // ACE_ASSERT (exitcode == 1);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for any child: %d\n"),
                exitcode));

  // This one should timeout:
  pid_t child5 = spawn_child (argv[0],
                              mgr,
                              4);
  pid_t result5 = mgr.wait (0,
                            ACE_Time_Value (1),
                            &exitcode);

  ACE_ASSERT (result5 == 0);    // should have timed out

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for any child: %d\n"),
                exitcode));

  // Now wait indefinitely to clean up...
  result5 = mgr.wait (0,
                      &exitcode);

  ACE_ASSERT (result5 == child5);
  // ACE_ASSERT (exitcode == 4);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) waited for child 5 again: %d\n"),
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

  ACE_Reactor::instance ()->run_event_loop (how_long);

  if (debug_test)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) done!\n") ));

  ACE_END_TEST;
  return result;
}

#else /* (!ACE_LACKS_FORK || ACE_WIN32) && ACE_HAS_THREADS */
int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Process_Manager_Test"));
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("The ACE Process Manager is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}
#endif /* (!ACE_LACKS_FORK || ACE_WIN32) && ACE_HAS_THREADS */

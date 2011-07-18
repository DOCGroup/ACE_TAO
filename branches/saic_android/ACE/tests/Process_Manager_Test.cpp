
//=============================================================================
/**
 *  @file    Process_Manager_Test.cpp
 *
 *  $Id$
 *
 *    This program tests the various methods provided by
 *    <ACE_Process_Manager>.  It illustrates both the explicit <wait>
 *    functions and the Reactor-style auto-reaping.  There's an
 *    Exit_Handler class that can print out (in Debug mode) when a
 *    child has been reaped.
 *
 *    The child processes spawned are simply this program itself, with
 *    an integer argument specifying how long to "process" (actually,
 *    the child just sleeps for the specified length of time).
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu> and Dave Madden <dhm@mersenne.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/SString.h"
#include "ace/Atomic_Op.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/Process_Manager.h"
#include "ace/Synch_Traits.h"
#include "ace/Get_Opt.h"
#include "ace/Thread.h"
#include "ace/Reactor.h"



static u_int debug_test = 0;
#if defined (ACE_HAS_WIN32_PRIORITY_CLASS)
static u_int process_id = 0;
#endif

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
                ACE_TEXT ("(%P) Exit_Handler(%C) got %d: %d\n"),
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
             int sleep_time,
             int my_process_id)
{

#if defined (ACE_HAS_WINCE)
const ACE_TCHAR *cmdline_format = ACE_TEXT("%s %d");
#elif defined (ACE_WIN32)
const ACE_TCHAR *cmdline_format = ACE_TEXT("\"%s\" %s %d");
#elif !defined (ACE_USES_WCHAR)
const ACE_TCHAR *cmdline_format = ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR ACE_TEXT("%s %s %d");
#else
const ACE_TCHAR *cmdline_format = ACE_TEXT (".") ACE_DIRECTORY_SEPARATOR_STR ACE_TEXT("%ls %ls %d");
#endif
  ACE_Process_Options opts;

  ACE_TCHAR prio[64];
  ACE_TCHAR cmd[16];

  if (debug_test)
    ACE_OS::strcpy (cmd, ACE_TEXT ("-d"));
  else
    cmd[0] = ACE_TEXT ('\0');

#if defined (ACE_HAS_WIN32_PRIORITY_CLASS)
  if (my_process_id == 1)
    {
      opts.creation_flags (ABOVE_NORMAL_PRIORITY_CLASS);
      ACE_OS::sprintf (prio, ACE_TEXT("and priority 'above normal'"));
    }
  else if (my_process_id == 2)
    {
      opts.creation_flags (BELOW_NORMAL_PRIORITY_CLASS);
      ACE_OS::sprintf (prio, ACE_TEXT("and priority 'below normal'"));
    }
  else if (my_process_id == 3)
    {
      opts.creation_flags (IDLE_PRIORITY_CLASS);
      ACE_OS::sprintf (prio, ACE_TEXT("and priority 'idle'"));
    }
  else if (my_process_id == 4)
    {
      opts.creation_flags (HIGH_PRIORITY_CLASS);
      ACE_OS::sprintf (prio, ACE_TEXT("and priority 'high'"));
    }
  else if (my_process_id == 5)
    {
      opts.creation_flags (NORMAL_PRIORITY_CLASS);
      ACE_OS::sprintf (prio, ACE_TEXT("and priority 'normal'"));
    }
  else
    prio[0] = ACE_TEXT ('\0');

  ACE_TCHAR pd [16];
  ACE_OS::sprintf (pd, ACE_TEXT (" -p %d"), my_process_id);
  ACE_OS::strcat (cmd, pd);
#else
  ACE_UNUSED_ARG (my_process_id);
  prio[0] = ACE_TEXT ('\0');
#endif

  opts.process_name (argv0);
  opts.command_line (cmdline_format,
#if !defined (ACE_HAS_WINCE)
                     argv0,
#endif /* !ACE_HAS_WINCE */
                     cmd,
                     sleep_time);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Spawning <%s> <%s>\n"),
                        opts.process_name(),
                        opts.command_line_buf ()));

  pid_t result = mgr.spawn (opts);

  if (result != ACE_INVALID_PID)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) spawned child: pid %d time %d %s\n"),
                int (result), sleep_time, prio));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("spawn failed")));

  return result;
}

ACE_CString order;

ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> running_tasks = 0;

class Process_Task : public ACE_Task<ACE_SYNCH>
{
public:
  Process_Task (const ACE_TCHAR *argv0,
                ACE_Process_Manager &mgr,
                int sleep_time)
    : argv0_ (argv0),
      mgr_ (mgr),
      sleep_time_ (sleep_time) { }

      // FUZZ: disable check_for_lack_ACE_OS
      /// FUZZ: enable check_for_lack_ACE_OS
  int open (void*)
  {
    char tmp[10];
    order += ACE_OS::itoa (sleep_time_, tmp, 10);
    ++running_tasks;
    activate ();
    return 0;
  }

  int svc (void)
  {
    int result = 0;
    ACE_exitcode exitcode;
    pid_t my_child = spawn_child (argv0_,
                                  mgr_,
                                  sleep_time_,
                                  0);
    result = mgr_.wait (my_child,
                        &exitcode);
    if (result != my_child)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P) Error: expected to reap child (%d); got %d\n"),
                    my_child,
                    result));
        if (result == ACE_INVALID_PID)
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
        //test_status = 1;
      }
    else
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P) reaped child, pid %d: %d\n"),
                  my_child,
                  exitcode));
    char tmp[10];
    order += ACE_OS::itoa (sleep_time_, tmp, 10);
    return 0;
  }

      // FUZZ: disable check_for_lack_ACE_OS
      /// FUZZ: enable check_for_lack_ACE_OS
  int close (u_long)
  {
    --running_tasks;
    return 0;
  }

private:
  const ACE_TCHAR *argv0_;
  ACE_Process_Manager &mgr_;
  int sleep_time_;
};

static int
command_line_test (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Testing for last character of command line\n")));
  int result = 0;
  const ACE_TCHAR *command = ACE_TEXT ("test Hello");
  size_t command_len = ACE_OS::strlen (command);
  ACE_Process_Options options (1, command_len + 1);
  options.command_line (command);
  ACE_TCHAR * const *procargv = options.command_line_argv ();
  if (ACE_OS::strcmp (procargv [1], ACE_TEXT ("Hello")) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("command_line_test failed: expected \"%s\"; got \"%s\"\n"),
                  ACE_TEXT ("Hello"),
                  procargv [1]));
      result = 1;
    }
  return result;
}

#if defined (ACE_HAS_WIN32_PRIORITY_CLASS)
void
check_process_priority (DWORD priority)
{
  if ((process_id == 0) ||
      (process_id == 1 && priority == ABOVE_NORMAL_PRIORITY_CLASS) ||
      (process_id == 2 && priority == BELOW_NORMAL_PRIORITY_CLASS) ||
      (process_id == 3 && priority == IDLE_PRIORITY_CLASS) ||
      (process_id == 4 && priority == HIGH_PRIORITY_CLASS) ||
      (process_id == 5 && priority == NORMAL_PRIORITY_CLASS) ||
      (process_id == 7 && priority == NORMAL_PRIORITY_CLASS))
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Process ID (%d) and priority (%d) match\n"),
            process_id, priority));
  else
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Given process priority (%d) and real priority (%d) differ.\n"),
                  process_id, priority));
}
#endif

int
run_main (int argc, ACE_TCHAR *argv[])
{
#if defined (ACE_HAS_WIN32_PRIORITY_CLASS)
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("dp:"));
#else
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("d"));
#endif
  int opt;
  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
          case 'd':
            debug_test = 1u;
            break;
#if defined (ACE_HAS_WIN32_PRIORITY_CLASS)
          case 'p':
            process_id = ACE_OS::atoi (get_opt.opt_arg ());
            break;
#endif
        }
    }

  if (get_opt.opt_ind () == argc - 1)
    {
      // child process: sleep & exit
      ACE_TCHAR lognm[MAXPATHLEN];
      int const mypid (ACE_OS::getpid ());
      ACE_OS::sprintf(lognm, ACE_TEXT ("Process_Manager_Test-child-%d"), mypid);

      ACE_START_TEST (lognm);
      int const secs = ACE_OS::atoi (argv[get_opt.opt_ind ()]);
      ACE_OS::sleep (secs ? secs : 1);

      ACE_TCHAR prio[64];
#if defined (ACE_WIN32_HAS_PRIORITY_CLASS)
      DWORD priority = ::GetPriorityClass (::GetCurrentProcess());

      check_process_priority(priority);

      if (priority == ABOVE_NORMAL_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'above normal'"));
      else if (priority == BELOW_NORMAL_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'below normal'"));
      else if (priority == HIGH_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'high'"));
      else if (priority == IDLE_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'idle'"));
      else if (priority == NORMAL_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'normal'"));
      else if (priority == REALTIME_PRIORITY_CLASS)
        ACE_OS::sprintf (prio, ACE_TEXT("and priority 'realtime'"));
#else
      prio[0] = ACE_TEXT ('\0');
#endif
      if (debug_test)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%T: pid %P about to exit with code %d %s\n"),
                    secs,
                    prio));
      ACE_END_LOG;

      return secs;
    }

  if (get_opt.opt_ind () != argc)      // incorrect usage
    usage (argv[0]);

  ACE_START_TEST (ACE_TEXT ("Process_Manager_Test"));

  int result = 0;
  int test_status = 0;

  if ((result = command_line_test ()) != 0)
    test_status = result;

  // Try the explicit <ACE_Process_Manager::wait> functions

  ACE_Process_Manager mgr;

  mgr.register_handler (new Exit_Handler ("default"));

  ACE_exitcode exitcode;

  // --------------------------------------------------
  // wait for a specific PID
  pid_t child1 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              1,
                              1);
  result = mgr.wait (child1,
                     &exitcode);

  if (result != child1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P) Error: expected to reap child1 (%d); got %d\n"),
                  child1,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child1, pid %d: %d\n"),
                child1,
                exitcode));

  // --------------------------------------------------
  // wait for a specific PID; another should finish first
  pid_t child2 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              1,
                              2);
  pid_t child3 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              4,
                              3);
  result = mgr.wait (child3,
                     &exitcode);

  if (result != child3)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P) Error: expected to reap child3 (%d); got %d\n"),
                  child3,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
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
                  ACE_TEXT ("(%P) Error: expected to reap child2 (%d); got %d\n"),
                  child2,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
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
  pid_t child4 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              1,
                              4);
  result = mgr.wait (0, ACE_Time_Value (4), &exitcode);

  if (result != child4)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P) Error: expected to reap child4 (%d); got %d\n"),
                  child4,
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 4 pid %d: %d\n"),
                result,
                exitcode));

  // This one should timeout:
  pid_t child5 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              4,
                              5);
  result = mgr.wait (0, ACE_Time_Value (1), &exitcode);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P) Error: expected wait to time out; got %d\n"),
                  result));
      if (result == ACE_INVALID_PID)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
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
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("error")));
      test_status = 1;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P) reaped child 5, pid %d: %d\n"),
                result,
                exitcode));

  // Terminate a child process and make sure we can wait for it.
  pid_t child6 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              5,
                              6);
  ACE_exitcode status6;
  if (-1 == mgr.terminate (child6))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P) %p\n"), ACE_TEXT ("terminate child6")));
      test_status = 1;
      mgr.wait (child6, &status6);  // Wait for child to exit just to clean up
    }
  else
    {
      if (-1 == mgr.wait (child6, &status6))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P) wait on child6 reported ACE_INVALID_PID\n")));
          test_status = 1;
        }
      else
        {
          // Get the results of the termination.
#if !defined(ACE_WIN32)
          if (WIFSIGNALED (status6) != 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P) child6 died on signal %d - correct\n"),
                        WTERMSIG (status6)));
          else
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P) child6 should have died on signal, ")
                        ACE_TEXT ("but didn't; exit status %d\n"),
                        WEXITSTATUS (status6)));
#else
          ACE_DEBUG
            ((LM_DEBUG,
              ACE_TEXT ("(%P) The process terminated with exit code %d\n"),
              status6));
#endif /*ACE_WIN32*/
        }
    }

#ifdef ACE_HAS_THREADS
  Process_Task task1 (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"), mgr, 3);
  Process_Task task2 (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"), mgr, 2);
  Process_Task task3 (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"), mgr, 1);
  task1.open (0);
  task2.open (0);
  task3.open (0);

  while (running_tasks!=0)
    {
      ACE_OS::sleep (1);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P) still running tasks\n")));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) result: '%C'\n"),
              order.c_str ()));

  if (order != "321123")
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P) wrong order of spawns ('%C', should be '321123')\n"),
                  order.c_str ()));
      test_status = 1;
    }
#endif /* ACE_HAS_THREADS */

#if !defined (ACE_OPENVMS)
  // --------------------------------------------------
  // Finally, try the reactor stuff...
  mgr.open (ACE_Process_Manager::DEFAULT_SIZE,
            ACE_Reactor::instance ());

  pid_t child7 = spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                              mgr,
                              5,
                              7);
  /* pid_t child8 = */ spawn_child (argc > 0 ? argv[0] : ACE_TEXT ("Process_Manager_Test"),
                                    mgr,
                                    6,
                                    0);

  mgr.register_handler (new Exit_Handler ("specific"),
                        child7);

  ACE_Time_Value how_long (10);

  ACE_Reactor::instance ()->run_reactor_event_loop (how_long);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P) Reactor loop done!\n") ));

  size_t const nr_procs = mgr.managed ();
  if (nr_procs != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("(%P) %d processes left in manager\n"),
                nr_procs));
#endif /* !defined (ACE_OPENVMS) */
  ACE_END_TEST;
  return test_status;
}


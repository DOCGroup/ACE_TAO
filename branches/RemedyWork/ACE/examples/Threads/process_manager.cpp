
//=============================================================================
/**
 *  @file    process_manager.cpp
 *
 *  $Id$
 *
 * Test out the mechanisms provided by the ACE_Process_Manager.
 * Using the global ACE_Process_Manager::instance(), we first spawn
 * some processes (re-invoke this program, and plain-old-fork on
 * systems that support it), and try the wait() functions.
 *
 * Then, we register the Process_Manager with
 * ACE_Reactor::instance() and spawn more processes, counting on the
 * autoreap to clean up.
 *
 * Specific-pid and generic exit-handler functions are also tested.
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu> and Dave Madden <dhm@mersenne.com>
 */
//=============================================================================


#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Service_Config.h"
#include "ace/Thread_Manager.h"
#include "ace/Process_Manager.h"
#include "ace/Get_Opt.h"

class ExitHandler : public ACE_Event_Handler
{
public:
  ExitHandler (const char *name);

  /// Called when object is removed from the <ACE_Reactor>.
  virtual ~ExitHandler (void);
  virtual int handle_exit (ACE_Process *proc);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);
private:
  const char *name_;
};

ExitHandler::ExitHandler (const char *name)
  : ACE_Event_Handler (),
    name_ (name)
{
}

ExitHandler::~ExitHandler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) ExitHandler \"%s\" destroyed\n",
              name_));
}

int
ExitHandler::handle_exit (ACE_Process *proc)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) ExitHandler \"%s\" handle_exit for pid %d status %d\n",
              name_,
              proc->getpid (),
              proc->exit_code ()));
  return 0;
}

int
ExitHandler::handle_timeout(const ACE_Time_Value &,
                            const void *)
{
  static int tick_tock = 0;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) \"%s\" %s\n",
              name_,
              ACE_ODD (tick_tock) ? "Tock" : "Tick"));
  tick_tock++;
  return 0;
}

int
ExitHandler::handle_close (ACE_HANDLE,
                           ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) ExitHandler \"%s\" handle_close\n",
              name_));
  delete this;
  return 0;
}

// Spin furiously <iterations> times, pausing every 100 cycles to
// print a message and sleep for a few seconds.

static void
worker (size_t iterations)
{
  for (size_t i = 0;
       i <= iterations;
       i++)
    if (i && (i % 100) == 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t@%T) worker spinning furiously... (%u)\n",
                    i));
        ACE_OS::sleep (1);
      }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) worker finished\n"));
}

static int n_iterations = 500;
static int child = 0;
static int exit_code = 0;

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("i:e:cu"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
        n_iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'e':
        exit_code = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'c':
        child = 1;
        break;
      case 'u':
      default:
        ACE_DEBUG ((LM_DEBUG, "usage:\n"
                    "-p <processes>\n"
                    "-i <iterations>\n"));
        break;
      }
}

// Use ACE_Process_Manager::instance() to spawn another copy of this
// process.

static pid_t
respawn_self (const ACE_TCHAR *myname,
              int iter,
              int exit_code)
{
  ACE_Process_Options options;
  options.command_line ("%s -c -i %d -e %d",
                        myname,
                        iter,
                        exit_code);
  return ACE_Process_Manager::instance ()->spawn (options);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config daemon;

  daemon.open (argv[0]);

  parse_args (argc, argv);

  if (child)
    {
      worker (n_iterations);

      ACE_OS::exit (exit_code);
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Process_Manager test.  Expect output from"
              "2 or 3 processes...\n"));

  ACE_Process_Manager::instance ()->register_handler
    (new ExitHandler ("default"));

  pid_t pid1 = respawn_self (argv[0],
                             n_iterations,
                             111);
  pid_t pid2 = respawn_self (argv[0],
                             n_iterations + 500,
                             222);

#if !defined (ACE_WIN32)
  pid_t pid3 = ACE_OS::fork ();

  if (!pid3)
    {
      worker (n_iterations);
      return 999;
    }
#endif /* ACE_WIN32 */

  ACE_Process_Manager::instance ()->register_handler (new ExitHandler ("specific"),
                                                      pid2);

  if (pid1 == ACE_INVALID_PID || pid2 == ACE_INVALID_PID)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) %p\n",
                       "start_n"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Test parent waiting (synchronously, "
              "up to 6 seconds) for children...\n"));

  int result =
    ACE_Process_Manager::instance ()->wait (ACE_Time_Value (6));

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Test parent: %d processes left\n",
              result));

  if (result > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t@%T) Test parent waiting (synchronously, "
                  "indefinitely) for remaining children...\n"));
      result =
        ACE_Process_Manager::instance ()->wait ();
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t@%T) Test parent finished waiting: %d\n",
                  result));
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Test parent: try auto-reap functions\n"));

  ACE_Process_Manager::instance ()->open (ACE_Process_Manager::DEFAULT_SIZE,
                                          ACE_Reactor::instance ());

  pid1 = respawn_self (argv[0],
                       n_iterations + 200,
                       333 );
  pid2 = respawn_self (argv[0],
                       n_iterations + 500,
                       444);

#if !defined (ACE_WIN32)
  pid3 = ACE_OS::fork ();

  if (!pid3)
    {
      worker (n_iterations);
      return 888;
    }
#endif /* ACE_WIN32 */

  ExitHandler *main_thread_work = 0;
  ACE_NEW_RETURN (main_thread_work,
                  ExitHandler ("main thread worker"),
                  1);

  ACE_Reactor::instance ()->schedule_timer (main_thread_work,
                                            0,
                                            ACE_Time_Value (2),
                                            ACE_Time_Value (1, 500000));
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Test parent: expect several Processes "
              "to be auto-detected over the next 30 seconds.\n"
              "The main thread will do some other work, too.\n" ));

  ACE_Time_Value briefly (30);

  result = ACE_Reactor::run_event_loop (briefly);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t@%T) Test parent: finished (%d) %d.%d.  Close"
              "Process_Manager...\n",
              result,
              briefly.sec (),
              briefly.usec ()));

  ACE_Process_Manager::instance ()->close ();

  return 0;
}

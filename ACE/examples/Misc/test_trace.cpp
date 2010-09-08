// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Misc
//
// = FILENAME
//    test_trace.cpp
//
// = DESCRIPTION
//    This example illustrates how to use the ACE tracing feature and
//    the ACE_TRACE macro.  It also shows the use of the ACE_Task_Base
//    class running as an "active object".
//
//    When adding ACE tracing to an application one option is to add
//
//    #define ACE_NTRACE 0
//
//    in the line above #include "ace/Log_Msg.h".  That's the approach shown below.
//
//    Another option is to add this line in $ACE_ROOT/ace/config.h.
//    Note, however, that if you add the line in config.h, you need to
//    add it *after* you've built ACE, i.e., don't build ACE with it
//    set to 0 as then you will get all the internal ACE_TRACE calls
//    showing up as well!  In a nutshell, in this second option you
//    do this:
//
//    1. Build ACE without tracing (i.e., don't #define ACE_NTRACE 0 in config.h)
//    2. Add #define ACE_NTRACE 0 in config.h
//    3. Build your app with tracing.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu> and
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

// Enable tracing
#define ACE_NTRACE 0

#include "ace/OS_main.h"
#include "ace/Signal.h"
#include "ace/Task.h"



class My_Task : public ACE_Task_Base
{
public:
  My_Task (size_t depth)
    : depth_ (depth)
  {
  }

  int recursive (size_t depth);

  virtual int svc (void)
  {
    return this->recursive (this->depth_);
  }

private:
  size_t depth_;
  // Depth of the recursion.
};

int
My_Task::recursive (size_t depth)
{
  ACE_TRACE ("My_Task::recursive");

  if (depth > 0)
    return recursive (depth - 1);
  else
    return 0;
  // Destructor of <ACE_Trace> automatically called.
}

extern "C"
void
exithook (void)
{
  ACE_TRACE ("void exithook (void)");

  ACE_DEBUG ((LM_DEBUG,
              "we're outta here!\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const size_t MAX_DEPTH = argc == 1 ? 10 : ACE_OS::atoi (argv[1]);

  ACE_OS::atexit (exithook);

  if (argc > 2)
    ACE_Trace::set_nesting_indent (ACE_OS::atoi (argv[2]));

  ACE_TRACE ("int ACE_TMAIN (int argc, ACE_TCHAR *argv[])");

  ACE_Sig_Action sig1 ((ACE_SignalHandler) ACE_Trace::start_tracing,
                       SIGUSR1);
  ACE_UNUSED_ARG (sig1);
  ACE_Sig_Action sig2 ((ACE_SignalHandler) ACE_Trace::stop_tracing,
                       SIGUSR2);
  ACE_UNUSED_ARG (sig2);

  My_Task task (MAX_DEPTH);

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
  int n_threads = argc > 3 ? ACE_OS::atoi (argv[3]) : 4;

  if (task.activate (THR_BOUND | THR_DETACHED, n_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "activate"),
                      -1);

  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();
#else
  const int MAX_ITERATIONS = argc > 3 ? ACE_OS::atoi (argv[3]) : 10;

  for (int i = 0; i < MAX_ITERATIONS; i++)
    task.svc ();
#endif /* ACE_MT_SAFE */

  // Destructor automatically called.
  return 0;
}

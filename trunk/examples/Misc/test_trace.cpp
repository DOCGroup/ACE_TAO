// Enable tracing
// @(#)test_trace.cpp	1.1	10/18/96

#define ACE_NTRACE 0
#include "ace/Log_Msg.h"

#if defined (ACE_MT_SAFE)
#include "ace/Thread.h"
#endif /* ACE_MT_SAFE */

#include "ace/Signal.h"

static void
foo (int max_depth)
{
  ACE_Trace _ ("void foo (void)", __LINE__, __FILE__);

  if (max_depth > 0)
    foo (max_depth - 1);
  /* Destructor automatically called */
}

int 
main (int argc, char *argv[])
{
  const int MAX_DEPTH = argc == 1 ? 10 : atoi (argv[1]);

  if (argc > 2)
    ACE_Trace::set_nesting_indent (ACE_OS::atoi (argv[2]));

  ACE_Trace _ ("int main (int argc, char *argv[])", __LINE__, __FILE__);

  ACE_Sig_Action sig1 ((ACE_SignalHandler) ACE_Trace::start_tracing, SIGUSR1);
  ACE_Sig_Action sig2 ((ACE_SignalHandler) ACE_Trace::stop_tracing, SIGUSR2);

#if defined (ACE_MT_SAFE)
  int n_threads = argc > 3 ? ACE_OS::atoi (argv[3]) : 4;

  if (ACE_Thread::spawn_n (n_threads, ACE_THR_FUNC (foo), 
			   (void *) MAX_DEPTH, 
			   THR_BOUND | THR_DETACHED) != n_threads)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), -1);
  ACE_OS::thr_exit (0);
#else
  for (;;)
    foo (MAX_DEPTH);
#endif /* ACE_MT_SAFE */

  /* Destructor automatically called */
  return 0;
}


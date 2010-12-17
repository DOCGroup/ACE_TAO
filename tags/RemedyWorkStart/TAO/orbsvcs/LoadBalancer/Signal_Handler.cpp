// $Id$

#include "Signal_Handler.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

TAO_LB_Signal_Handler::TAO_LB_Signal_Handler (CORBA::ORB_ptr orb,
                                              PortableServer::POA_ptr poa)
  :
#ifdef ACE_HAS_THREADS
    signal_guard_ (),
#endif  /* ACE_HAS_THREADS */
    sigset_ (),
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  // Register signal handlers.
  this->sigset_.sig_add (SIGINT);
  this->sigset_.sig_add (SIGTERM);
}

int
TAO_LB_Signal_Handler::svc (void)
{
  // This method is only invoked when performing synchronous signal
  // handling.

  int signum = -1;

  // Block waiting for the registered signals.
  if (ACE_OS::sigwait (this->sigset_, &signum) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p\n",
                         "ERROR waiting on signal"),
                        -1);
    }

  ACE_ASSERT (signum == SIGINT || signum == SIGTERM);

//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("(%P|%t) synchronous signal handler done\n")));

  return this->perform_cleanup (signum);
}

int
TAO_LB_Signal_Handler::activate (long flags,
                                 int n_threads,
                                 int force_active,
                                 long priority,
                                 int grp_id,
                                 ACE_Task_Base *task,
                                 ACE_hthread_t thread_handles[],
                                 void *stack[],
                                 size_t stack_size[],
                                 ACE_thread_t thread_ids[],
                                 const char* thr_name[])
{
  // sigwait() is not implemented on MS Windows.  Handle signals
  // asynchronously through the ORB's reactor in that case instead.
  // Otherwise, handle signals synchronously in another thread.

#if defined (ACE_HAS_THREADS) && !defined (ACE_WIN32)
  return this->ACE_Task_Base::activate (flags,
                                        n_threads,
                                        force_active,
                                        priority,
                                        grp_id,
                                        task,
                                        thread_handles,
                                        stack,
                                        stack_size,
                                        thread_ids,
                                        thr_name);
#else
  ACE_UNUSED_ARG (flags);
  ACE_UNUSED_ARG (n_threads);
  ACE_UNUSED_ARG (force_active);
  ACE_UNUSED_ARG (priority);
  ACE_UNUSED_ARG (grp_id);
  ACE_UNUSED_ARG (task);
  ACE_UNUSED_ARG (thread_handles);
  ACE_UNUSED_ARG (stack);
  ACE_UNUSED_ARG (stack_size);
  ACE_UNUSED_ARG (thread_ids);
  ACE_UNUSED_ARG (thr_name);

  return
    this->orb_->orb_core ()->reactor ()->register_handler (this->sigset_,
                                                           this);
#endif  /* ACE_HAS_THREADS */
}

int
TAO_LB_Signal_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
//   ACE_DEBUG ((LM_DEBUG,
//               ACE_TEXT ("(%P|%t) ASYNCHRONOUS signal handler done\n")));

  // This method is only used in the asynchronous signal handling case
  // (i.e. when single-threaded build is used).

  // @note Is it okay to perform ORB operations from this method?  The
  //       issue here is whether or not
  //       ACE_Event_Handler::handle_signal() is called from within an
  //       OS signal handler, and if that did occur is it safe to
  //       perform ORB operations?
  return this->perform_cleanup (signum);
}

int
TAO_LB_Signal_Handler::perform_cleanup (int signum)
{
  try
    {
      // Shutdown the POA.
      //
      // Shutting down the POA will cause servants "owned" by the POA
      // to be destroyed.  Servants will then have the opportunity to
      // clean up all resources they are responsible for.
      this->poa_->destroy (1, 1);

      // Now shutdown the ORB.
      this->orb_->shutdown (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception");

      ACE_ERROR_RETURN ((LM_ERROR,
                         "Problem during cleanup initiated by signal %d.\n",
                         signum),
                        -1);
    }

  return 0;
}

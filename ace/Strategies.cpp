// Strategies.cpp
// $Id$

#if !defined (ACE_STRATEGIES_C)
#define ACE_STRATEGIES_C

#define ACE_BUILD_DLL
#include "ace/Strategies.h"

#define SH SVC_HANDLER
#define PR_AC_1 ACE_PEER_ACCEPTOR_1
#define PR_AC_2 ACE_PEER_ACCEPTOR_2
#define PR_AD ACE_PEER_ACCEPTOR_ADDR

ACE_ALLOC_HOOK_DEFINE(ACE_Singleton_Strategy)

template <class SH> void
ACE_Singleton_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Singleton_Strategy<SH>::dump");
}

template <class SH> int
ACE_Singleton_Strategy<SH>::open (SVC_HANDLER *sh,
				  ACE_Thread_Manager *)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SH>::open");
  if (this->svc_handler_ != 0)
    delete this->svc_handler_;

  this->svc_handler_ = sh;
  return 0;
}
  
template <class SH>
ACE_Singleton_Strategy<SH>::ACE_Singleton_Strategy (SVC_HANDLER *sh,
						    ACE_Thread_Manager *tm)
  : svc_handler_ (0)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SH>::ACE_Singleton_Strategy");
  this->open (sh, tm);
}
  
template <class SH>
ACE_Singleton_Strategy<SH>::~ACE_Singleton_Strategy (void)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SH>::~ACE_Singleton_Strategy");
  delete this->svc_handler_;
}

// Create a Singleton SVC_HANDLER by always returning the same
// SVC_HANDLER.

template <class SH> SVC_HANDLER *
ACE_Singleton_Strategy<SH>::make_svc_handler (void)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SH>::make_svc_handler");
  return this->svc_handler_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Creation_Strategy)

template <class SH> void
ACE_Creation_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Creation_Strategy<SH>::dump");
}

template <class SH> int
ACE_Creation_Strategy<SH>::open (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SH>::open");
  this->thr_mgr_ = thr_mgr;
  return 0;
}


template <class SH> 
ACE_Creation_Strategy<SH>::ACE_Creation_Strategy (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SH>::ACE_Creation_Strategy");
  this->open (thr_mgr);
}

// Default behavior is to make a new SVC_HANDLER, passing in the
// Thread_Manager (if any).

template <class SH> SH *
ACE_Creation_Strategy<SH>::make_svc_handler (void)
{
  ACE_TRACE ("ACE_Creation_Strategy<SH>::make_svc_handler");
  return new SH (this->thr_mgr_);
}

template <class SH> 
ACE_Creation_Strategy<SH>::~ACE_Creation_Strategy (void)
{
  ACE_TRACE ("ACE_Creation_Strategy<SH>::~ACE_Creation_Strategy");
}

ACE_ALLOC_HOOK_DEFINE(ACE_DLL_Strategy)

template <class SH> void
ACE_DLL_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_DLL_Strategy<SH>::dump");
}

template <class SH> int
ACE_DLL_Strategy<SH>::open (const char svc_dll_info[],
			    ACE_Service_Config *svc_config,
			    ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SH>::open");
  this->inherited::open (thr_mgr);
  this->svc_config_ = svc_config;
  return 0;
}

template <class SH> 
ACE_DLL_Strategy<SH>::ACE_DLL_Strategy (const char svc_dll_info[],
					ACE_Service_Config *sc,
					ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SH>::ACE_DLL_Strategy");
  if (this->open (svc_dll_info, sc, thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
}

template <class SH> 
ACE_DLL_Strategy<SH>::ACE_DLL_Strategy (void)
{
  ACE_TRACE ("ACE_DLL_Strategy<SH>::ACE_DLL_Strategy");
}

// Create a SVC_HANDLER by dynamically linking it from a DLL.

template <class SH> SH *
ACE_DLL_Strategy<SH>::make_svc_handler (void)
{
  ACE_TRACE ("ACE_DLL_Strategy<SH>::make_svc_handler");
  // Open the shared library.
  void *handle = (void *) ACE_OS::dlopen (this->shared_library_);

  // Extract the factory function.
  SH *(*factory)(void) = (SH *(*)(void)) ACE_OS::dlsym (handle, 
						  this->factory_function_);
  
  // Call the factory function to obtain the new SVC_Handler (should
  // use RTTI here when it becomes available...)
  SH *svc_handler = (*factory)();

  if (svc_handler != 0)
    {
      // Create an ACE_Service_Record containing the SVC_Handler and
      // insert into this->svc_config_->svc_rep;

      // @@ This remains to be implemented...
      // @@ Somehow, we need to deal with this->thr_mgr_...
    }

  return svc_handler;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Concurrency_Strategy)

template <class SH> void
ACE_Concurrency_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SH>::dump");
}

// Default behavior is to activate the SVC_HANDLER by calling it's
// open() method, which allows the SVC_HANDLER to determine its own
// concurrency strategy.

template <class SH> int
ACE_Concurrency_Strategy<SH>::activate_svc_handler (SH *svc_handler,
						    void *arg)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SH>::activate_svc_handler");
  // Delegate control to the application-specific service
  // handler.

  if (svc_handler->open (arg) == -1)
    {
      // Close down handler to avoid resource leaks.
      svc_handler->close (0); 
      return -1;
    }
  else
    return 0;
}

template <class SH> 
ACE_Concurrency_Strategy<SH>::~ACE_Concurrency_Strategy (void)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SH>::~ACE_Concurrency_Strategy");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Strategy)

template <class SH> void
ACE_Thread_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::dump");
}

template <class SH> int
ACE_Thread_Strategy<SH>::open (ACE_Thread_Manager *thr_mgr,
			       long thr_flags,
			       int n_threads)
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::open");
  this->thr_mgr_ = thr_mgr;
  this->n_threads_ = n_threads;
  this->thr_flags_ = thr_flags;

  // Must have a thread manager!
  if (this->thr_mgr_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "error: must have a non-NULL thread manager\n"), -1);
  else
    return 0;
}

template <class SH> 
ACE_Thread_Strategy<SH>::ACE_Thread_Strategy (ACE_Thread_Manager *thr_mgr,
					      long thr_flags,
					      int n_threads)
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::ACE_Thread_Strategy");
  this->open (thr_mgr, thr_flags, n_threads);
}

template <class SH> 
ACE_Thread_Strategy<SH>::ACE_Thread_Strategy (void)
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::ACE_Thread_Strategy");
}

template <class SH> 
ACE_Thread_Strategy<SH>::~ACE_Thread_Strategy (void)
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::~ACE_Thread_Strategy");
}

template <class SH> int
ACE_Thread_Strategy<SH>::activate_svc_handler (SH *svc_handler,
					       void *arg)
{
  ACE_TRACE ("ACE_Thread_Strategy<SH>::activate_svc_handler");
  // Call up to our parent to do the SVC_HANDLER initialization.
  if (this->inherited::activate_svc_handler (svc_handler, arg) == -1)
    return -1;
  else 
    // Turn the <svc_handler> into an active object (if it isn't
    // already one as a result of the first activation...)
    return svc_handler->activate (this->thr_flags_, this->n_threads_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Accept_Strategy)

template <class SH, PR_AC_1> void
ACE_Accept_Strategy<SH, PR_AC_2>::dump (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::dump");
}

template <class SH, PR_AC_1> int
ACE_Accept_Strategy<SH, PR_AC_2>::open (const PR_AD &local_addr, 
					int restart)
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::open");
  return this->peer_acceptor_.open (local_addr, restart);
}

template <class SH, PR_AC_1>
ACE_Accept_Strategy<SH, PR_AC_2>::ACE_Accept_Strategy (const PR_AD &local_addr, 
						       int restart)
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::ACE_Accept_Strategy");
  if (this->open (local_addr, restart) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
}

template <class SH, PR_AC_1>
ACE_Accept_Strategy<SH, PR_AC_2>::ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::ACE_Accept_Strategy");
}

template <class SH, PR_AC_1> int
ACE_Accept_Strategy<SH, PR_AC_2>::accept_svc_handler (SH *svc_handler)
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::accept_svc_handler");
  if (this->peer_acceptor_.accept (*svc_handler) == -1)
    {
      svc_handler->close (0);
      return -1;
    }
  else
    return 0;
}

template <class SH, PR_AC_1> ACE_HANDLE
ACE_Accept_Strategy<SH, PR_AC_2>::get_handle (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::get_handle");
  return this->peer_acceptor_.get_handle ();
}

template <class SH, PR_AC_1> ACE_PEER_ACCEPTOR &
ACE_Accept_Strategy<SH, PR_AC_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

template <class SH, PR_AC_1>
ACE_Accept_Strategy<SH, PR_AC_2>::~ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SH, PR_AC_2>::~ACE_Accept_Strategy");
  if (this->peer_acceptor_.close () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "close"));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Process_Strategy)

template <class SH> void
ACE_Process_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::dump");
}

template <class SH> int
ACE_Process_Strategy<SH>::open (int n_processes)
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::open");
  this->n_processes_ = n_processes;

  return 0;
}

template <class SH> 
ACE_Process_Strategy<SH>::ACE_Process_Strategy (int n_processes)
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::ACE_Process_Strategy");
  this->open (thr_mgr, thr_flags, n_threads);
}

template <class SH> 
ACE_Process_Strategy<SH>::ACE_Process_Strategy (void)
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::ACE_Process_Strategy");
}

template <class SH> 
ACE_Process_Strategy<SH>::~ACE_Process_Strategy (void)
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::~ACE_Process_Strategy");
}

template <class SH> int
ACE_Process_Strategy<SH>::activate_svc_handler (SH *svc_handler,
						void *arg)
{
  ACE_TRACE ("ACE_Process_Strategy<SH>::activate_svc_handler");
  switch (ACE_OS::fork ())
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "fork"), -1);
      /* NOTREACHED */
    case 0: // In child process.
      // Call up to our ancestor in the inheritance to do the
      // SVC_HANDLER initialization.  
      return this->inherited::activate_svc_handler (svc_handler, arg);
      /* NOTREACHED */
    default: // In parent process.
      return 0;
    }
}

template <class SH>
ACE_Scheduling_Strategy<SH>::ACE_Scheduling_Strategy (SH *scheduler)
  : scheduler_ (scheduler),
    delete_scheduler_ (0)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SH>::ACE_Scheduling_Strategy");

  if (this->scheduler_ == 0)
    {
      // Create a new SVC_HANDLER and assign the global Thread_Manager
      // and Reactor to it...
      ACE_NEW (this->scheduler_, SH);

      if (this->scheduler_->thr_mgr () == 0)
	this->scheduler_->thr_mgr (ACE_Service_Config::thr_mgr ());

      if (this->scheduler_->reactor () == 0)
	this->scheduler_->reactor (ACE_Service_Config::reactor ());

      this->delete_scheduler_ = 1;
    }
}

template <class SH>
ACE_Scheduling_Strategy<SH>::~ACE_Scheduling_Strategy (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SH>::~ACE_Scheduling_Strategy");

  if (this->delete_scheduler_)
    this->scheduler_->destroy ();
}

template <class SH> void
ACE_Scheduling_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SH>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "scheduler_ = %x", this->scheduler_));
  ACE_DEBUG ((LM_DEBUG, "\ndelete_scheduler_ = %d", this->delete_scheduler_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class SH> int 
ACE_Scheduling_Strategy<SH>::suspend (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SH>::suspend");
  return -1;
}

template <class SH> int 
ACE_Scheduling_Strategy<SH>::resume (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SH>::resume");
  return -1;
}

template <class SH>
ACE_Schedule_All_Reactive_Strategy<SH>::ACE_Schedule_All_Reactive_Strategy (SH *scheduler)
  : ACE_Scheduling_Strategy<SH> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SH>::ACE_Schedule_All_Reactive_Strategy");
}

template <class SH> int 
ACE_Schedule_All_Reactive_Strategy<SH>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SH>::suspend");
  return this->scheduler_->reactor ()->suspend_handlers ();
}

template <class SH> void
ACE_Schedule_All_Reactive_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SH>::dump");

  ACE_Scheduling_Strategy<SH>::dump ();
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Reactive_Strategy<SH>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SH>::resume");
  return this->scheduler_->reactor ()->resume_handlers ();
}

template <class SH>
ACE_Schedule_All_Threaded_Strategy<SH>::ACE_Schedule_All_Threaded_Strategy (SH *scheduler)
  : ACE_Scheduling_Strategy<SH> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SH>::ACE_Schedule_All_Threaded_Strategy");
}

template <class SH> int 
ACE_Schedule_All_Threaded_Strategy<SH>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SH>::suspend");
  return this->scheduler_->thr_mgr ()->suspend_all ();
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Threaded_Strategy<SH>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SH>::resume");
  return this->scheduler_->thr_mgr ()->resume_all ();
}

template <class SH> void
ACE_Schedule_All_Threaded_Strategy<SH>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SH>::dump");

  ACE_Scheduling_Strategy<SH>::dump ();
}

#undef SH
#undef PR_AC_1
#undef PR_AC_2
#undef PR_AD

#endif /* ACE_STRATEGIES_C */

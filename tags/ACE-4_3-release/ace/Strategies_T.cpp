// Strategies_T.cpp
// $Id$

#if !defined (ACE_STRATEGIES_T_C)
#define ACE_STRATEGIES_T_C

#define ACE_BUILD_DLL
#include "ace/Strategies_T.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Singleton_Strategy)

template <class SVC_HANDLER> void
ACE_Singleton_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_Singleton_Strategy<SVC_HANDLER>::open (SVC_HANDLER *sh,
					   ACE_Thread_Manager *)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::open");

  if (this->svc_handler_ != 0)
    delete this->svc_handler_;

  this->svc_handler_ = sh;
  return 0;
}
  
template <class SVC_HANDLER>
ACE_Singleton_Strategy<SVC_HANDLER>::ACE_Singleton_Strategy (SVC_HANDLER *sh,
							     ACE_Thread_Manager *tm)
  : svc_handler_ (0)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::ACE_Singleton_Strategy");
  this->open (sh, tm);
}
  
template <class SVC_HANDLER>
ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy (void)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy");
  delete this->svc_handler_;
}

// Create a Singleton SVC_HANDLER by always returning the same
// SVC_HANDLER.

template <class SVC_HANDLER> int
ACE_Singleton_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::make_svc_handler");
  sh = this->svc_handler_;
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Creation_Strategy)

template <class SVC_HANDLER> void
ACE_Creation_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_Creation_Strategy<SVC_HANDLER>::open (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::open");
  this->thr_mgr_ = thr_mgr;
  return 0;
}


template <class SVC_HANDLER> 
ACE_Creation_Strategy<SVC_HANDLER>::ACE_Creation_Strategy (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::ACE_Creation_Strategy");
  this->open (thr_mgr);
}

// Default behavior is to make a new SVC_HANDLER, passing in the
// Thread_Manager (if any).

template <class SVC_HANDLER> int
ACE_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::make_svc_handler");

  if (sh == 0)
    ACE_NEW_RETURN (sh, SVC_HANDLER (this->thr_mgr_), -1);
  return 0;
}

template <class SVC_HANDLER> 
ACE_Creation_Strategy<SVC_HANDLER>::~ACE_Creation_Strategy (void)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::~ACE_Creation_Strategy");
}

ACE_ALLOC_HOOK_DEFINE(ACE_DLL_Strategy)

template <class SVC_HANDLER> void
ACE_DLL_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_DLL_Strategy<SVC_HANDLER>::open (const char svc_dll_info[],
				     ACE_Service_Repository *svc_rep,
				     ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::open");
  this->inherited::open (thr_mgr);
  this->svc_rep_ = svc_rep;
  return 0;
}

template <class SVC_HANDLER> 
ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy (const char svc_dll_info[],
						 ACE_Service_Repository *svc_rep,
						 ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy");
  if (this->open (svc_dll_info, svc_rep, thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
}

template <class SVC_HANDLER> 
ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy (void)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy");
}

// Create a SVC_HANDLER by dynamically linking it from a DLL.

template <class SVC_HANDLER> int
ACE_DLL_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::make_svc_handler");
  // Open the shared library.
  ACE_SHLIB_HANDLE handle = ACE_OS::dlopen (this->shared_library_);

  // Extract the factory function.
  SVC_HANDLER *(*factory)(void) = (SVC_HANDLER *(*)(void)) ACE_OS::dlsym 
    (handle, this->factory_function_);
  
  // Call the factory function to obtain the new SVC_Handler (should
  // use RTTI here when it becomes available...)
  SVC_HANDLER *svc_handler;
  
  ACE_ALLOCATOR_RETURN (svc_handler, (*factory)(), -1);

  if (svc_handler != 0)
    {
      // Create an ACE_Service_Record containing the SVC_Handler and
      // insert into this->svc_rep_;

      // @@ This remains to be implemented...
      // @@ Somehow, we need to deal with this->thr_mgr_...
    }

  sh = svc_handler;
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Concurrency_Strategy)

template <class SVC_HANDLER> void
ACE_Concurrency_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::dump");
}

// Default behavior is to activate the SVC_HANDLER by calling it's
// open() method, which allows the SVC_HANDLER to determine its own
// concurrency strategy.

template <class SVC_HANDLER> int
ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *svc_handler,
							     void *arg)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler");
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

template <class SVC_HANDLER> 
ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy (void)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Strategy)

template <class SVC_HANDLER> void
ACE_Reactive_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_Reactive_Strategy<SVC_HANDLER>::open (ACE_Reactor *reactor,
					  ACE_Reactor_Mask mask,
					  int flags)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::open");
  this->reactor_ = reactor;
  this->mask_ = mask;
  this->flags_ = flags;

  // Must have a <Reactor>
  if (this->reactor_ == 0)
    return -1;
  else
    return 0;
}

template <class SVC_HANDLER> 
ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy (ACE_Reactor *reactor,
							   ACE_Reactor_Mask mask,
							   int flags)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy");

  if (this->open (reactor, mask, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
		"ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy"));
}

template <class SVC_HANDLER> 
ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy (void)
  : reactor_ (0),
    mask_ (ACE_Event_Handler::NULL_MASK),
    flags_ (0)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy");
}

template <class SVC_HANDLER> 
ACE_Reactive_Strategy<SVC_HANDLER>::~ACE_Reactive_Strategy (void)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::~ACE_Reactive_Strategy");
}

template <class SVC_HANDLER> int
ACE_Reactive_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *svc_handler,
							  void *arg)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::activate_svc_handler");

  if (this->reactor_ == 0)
    return -1;

  // See if we should enable non-blocking I/O on the <svc_handler>'s
  // peer.
  else if (ACE_BIT_ENABLED (this->flags_, ACE_NONBLOCK) != 0)
    {
      if (svc_handler->peer ().enable (ACE_NONBLOCK) == -1)
	return -1;
    }
  // Otherwise, make sure it's disabled by default.
  else if (svc_handler->peer ().disable (ACE_NONBLOCK) == -1)
    return -1;

  // Register with the Reactor with the appropriate <mask>.
  if (this->reactor_->register_handler (svc_handler, this->mask_) == -1)
    return -1;

    // Call up to our parent to do the SVC_HANDLER initialization.
  else if (this->inherited::activate_svc_handler (svc_handler, arg) == -1)
    {
      // Make sure to remove the <svc_handler> from the <Reactor>.
      this->reactor_->remove_handler (svc_handler, this->mask_);
      return -1;
    }
  else
    return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Thread_Strategy)

template <class SVC_HANDLER> void
ACE_Thread_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_Thread_Strategy<SVC_HANDLER>::open (ACE_Thread_Manager *thr_mgr,
					long thr_flags,
					size_t n_threads)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::open");
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

template <class SVC_HANDLER> 
ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy (ACE_Thread_Manager *thr_mgr,
						       long thr_flags,
						       size_t n_threads)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy");

  if (this->open (thr_mgr, thr_flags, n_threads) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
		"ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy"));
}

template <class SVC_HANDLER> 
ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy (void)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy");
}

template <class SVC_HANDLER> 
ACE_Thread_Strategy<SVC_HANDLER>::~ACE_Thread_Strategy (void)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::~ACE_Thread_Strategy");
}

template <class SVC_HANDLER> int
ACE_Thread_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *svc_handler,
							void *arg)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::activate_svc_handler");
  // Call up to our parent to do the SVC_HANDLER initialization.
  if (this->inherited::activate_svc_handler (svc_handler, arg) == -1)
    return -1;
  else 
    // Turn the <svc_handler> into an active object (if it isn't
    // already one as a result of the first activation...)
    return svc_handler->activate (this->thr_flags_, this->n_threads_);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Accept_Strategy)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr, int restart)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy");

  if (this->open (local_addr, restart) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "open"));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open 
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr, int restart)
{
  return this->acceptor_.open (local_addr, restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> void
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump");
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler");

  if (this->acceptor_.accept (svc_handler->peer ()) == -1)
    {
      svc_handler->close (0);
      return -1;
    }
  else
    return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle");
  return this->acceptor_.get_handle ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_PEER_ACCEPTOR &
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->acceptor_;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy");

  if (this->acceptor_.close () == -1)
    ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "close"));
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> void
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::dump (void) const
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::dump");
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> int
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connect_svc_handler
  (SVC_HANDLER *&sh,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms)
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connect_svc_handler");

  return this->connector_.connect (sh->peer (), remote_addr,
				   timeout, local_addr,
				   reuse_addr, flags, perms);
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> ACE_PEER_CONNECTOR &
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connector (void) const
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connector");
  return (ACE_PEER_CONNECTOR &) this->connector_;
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::ACE_Connect_Strategy (void)
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::ACE_Connect_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1>
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::~ACE_Connect_Strategy (void)
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::~ACE_Connect_Strategy");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Connect_Strategy)
ACE_ALLOC_HOOK_DEFINE(ACE_Process_Strategy)

template <class SVC_HANDLER> void
ACE_Process_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::dump");
}

template <class SVC_HANDLER> int
ACE_Process_Strategy<SVC_HANDLER>::open (size_t n_processes,
					 ACE_Event_Handler *acceptor,
					 ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::open");
  this->n_processes_ = n_processes;
  this->acceptor_ = acceptor;
  this->reactor_ = reactor;

  return 0;
}

template <class SVC_HANDLER> 
ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy (size_t n_processes,
							 ACE_Event_Handler *acceptor,
							 ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy");
  this->open (n_processes, acceptor, reactor);
}

template <class SVC_HANDLER> 
ACE_Process_Strategy<SVC_HANDLER>::~ACE_Process_Strategy (void)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::~ACE_Process_Strategy");
}

template <class SVC_HANDLER> int
ACE_Process_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *svc_handler,
							 void *arg)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::activate_svc_handler");

  switch (ACE_OS::fork ("child"))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "fork"), -1);
      /* NOTREACHED */
    case 0: // In child process.

      // Close down the SOCK_Acceptor's handle since we don't need to
      // keep it open.
      if (this->acceptor_ != 0)
	// Ignore the return value here...
	(void) this->reactor_->remove_handler (this->acceptor_, 
					       ACE_Event_Handler::ACCEPT_MASK);

      // Call up to our ancestor in the inheritance to do the
      // SVC_HANDLER initialization.  
      return this->inherited::activate_svc_handler (svc_handler, arg);
      /* NOTREACHED */
    default: // In parent process.
      // We need to close down the <SVC_HANDLER> here because it's
      // running in the child. 
      svc_handler->destroy ();
      return 0;
    }
}

template <class SVC_HANDLER>
ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy (SVC_HANDLER *scheduler)
  : scheduler_ (scheduler),
    delete_scheduler_ (0)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy");

  if (this->scheduler_ == 0)
    {
      // Create a new SVC_HANDLER and assign the global Thread_Manager
      // and Reactor to it...
      ACE_NEW (this->scheduler_, SVC_HANDLER);

      if (this->scheduler_->thr_mgr () == 0)
	this->scheduler_->thr_mgr (ACE_Thread_Manager::instance ());

      if (this->scheduler_->reactor () == 0)
	this->scheduler_->reactor (ACE_Reactor::instance ());

      this->delete_scheduler_ = 1;
    }
}

template <class SVC_HANDLER>
ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy");

  if (this->delete_scheduler_)
    this->scheduler_->destroy ();
}

template <class SVC_HANDLER> void
ACE_Scheduling_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "scheduler_ = %x", this->scheduler_));
  ACE_DEBUG ((LM_DEBUG, "\ndelete_scheduler_ = %d", this->delete_scheduler_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class SVC_HANDLER> int 
ACE_Scheduling_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::suspend");
  return -1;
}

template <class SVC_HANDLER> int 
ACE_Scheduling_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::resume");
  return -1;
}

template <class SVC_HANDLER>
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::ACE_Schedule_All_Reactive_Strategy 
  (SVC_HANDLER *scheduler)
  : ACE_Scheduling_Strategy<SVC_HANDLER> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::ACE_Schedule_All_Reactive_Strategy");
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend");
  return this->scheduler_->reactor ()->suspend_handlers ();
}

template <class SVC_HANDLER> void
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::dump");

  ACE_Scheduling_Strategy<SVC_HANDLER>::dump ();
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume");
  return this->scheduler_->reactor ()->resume_handlers ();
}

template <class SVC_HANDLER>
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy 
  (SVC_HANDLER *scheduler)
  : ACE_Scheduling_Strategy<SVC_HANDLER> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy");
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend");
  return this->scheduler_->thr_mgr ()->suspend_all ();
}

template <class SVC_HANDLER> int 
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume");
  return this->scheduler_->thr_mgr ()->resume_all ();
}

template <class SVC_HANDLER> void
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::dump");

  ACE_Scheduling_Strategy<SVC_HANDLER>::dump ();
}

// Return the address of <b> by default.  This should typically be
// specialized!

template<class ADDR_T, class SVC_HANDLER> size_t
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::hash_i (const ADDR_T &b) const
{
  size_t tmp;
  const void *a = (void *) &b;
  (void) ACE_OS::memcpy ((void *) &tmp, (void *) &a, sizeof tmp);
  return tmp;
}

// Default definition for type comparison operation (returns values
// corresponding to those returned by <memcmp>/<strcmp>).  This should
// typically be specialized.

template<class ADDR_T, class SVC_HANDLER> int
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::compare_i (const ADDR_T &b1, 
					       const ADDR_T &b2) const
{
  if (&b1 == &b2)
    return 0;
  else
    return ACE_OS::memcmp (&b1, &b2, sizeof b1);
}

// Automatic conversion operators
template<class ADDR_T, class SVC_HANDLER>
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::operator ADDR_T& (void)
{
  return addr_;
}

template<class ADDR_T, class SVC_HANDLER>
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::operator const ADDR_T& (void) const
{
  return addr_;
}

template<class ADDR_T, class SVC_HANDLER>
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::ACE_Hash_Addr (void)
  : hash_value_ (0),
    svc_handler_ (0)
{
}

template<class ADDR_T, class SVC_HANDLER>
ACE_Hash_Addr<ADDR_T, SVC_HANDLER>::ACE_Hash_Addr (const ADDR_T &a, SVC_HANDLER *sh)
  : hash_value_ (0),
    svc_handler_ (sh),
    addr_ (a)
{
  (void) this->hash ();
}

template<class ADDR_T, class SVC_HANDLER> u_long
ACE_Hash_Addr<ADDR_T,SVC_HANDLER>::hash (void) const
{
  // In doing the check below, we take chance of paying a performance
  // price when the hash value is zero.  But, that will (hopefully)
  // happen far less often than a non-zero value, so this caching
  // strategy should pay off, esp. if hash computation is expensive
  // relative to the simple comparison.

  if (this->hash_value_ == 0)
    ((ACE_Hash_Addr<ADDR_T, SVC_HANDLER> *) this)->hash_value_ = this->hash_i (addr_);

  return this->hash_value_;
}

template<class ADDR_T, class SVC_HANDLER> int
ACE_Hash_Addr<ADDR_T,SVC_HANDLER>::operator== (const ACE_Hash_Addr<ADDR_T, SVC_HANDLER> &rhs) const
{
  if (svc_handler_ == 0)
    return this->compare_i (addr_, rhs.addr_) == 0;
  else
    return svc_handler_->in_use () == 0 
      && this->compare_i (addr_, rhs.addr_) == 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::connect_svc_handler
  (SVC_HANDLER *&sh,
   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
   ACE_Time_Value *timeout,
   const ACE_PEER_CONNECTOR_ADDR &local_addr,
   int reuse_addr,
   int flags,
   int perms)
{
  ACE_Hash_Addr<ACE_PEER_CONNECTOR_ADDR, SVC_HANDLER> search_addr (remote_addr);

  // Try to find the addres in the cache.  Only if we don't find it do
  // we create a new <SVC_HANDLER> and connect it with the server.

  if (this->connection_cache_.find (search_addr, sh) == -1)
    {
      ACE_NEW_RETURN(sh, SVC_HANDLER, -1);

      // Actively establish the connection
      if (ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connect_svc_handler
	  (sh, remote_addr, timeout, local_addr, reuse_addr, flags, perms) == -1)
	return -1;
      // Insert the new SVC_HANDLER instance into the cache
      else
	{
	  ACE_Hash_Addr<ACE_PEER_CONNECTOR_ADDR, SVC_HANDLER> server_addr (remote_addr,sh);

	  if (this->connection_cache_.bind (server_addr, sh) == -1)
	    return -1;
	}
    }

  sh->in_use (1);
  return 0;
}

#endif /* ACE_STRATEGIES_T_C */

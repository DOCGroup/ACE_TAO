// $Id$

#if !defined (ACE_STRATEGIES_T_C)
#define ACE_STRATEGIES_T_C

#define ACE_BUILD_DLL
#include "ace/Strategies_T.h"
#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "ace/Service_Types.h"
#include "ace/Thread_Manager.h"
#include "ace/WFMO_Reactor.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Strategies_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_ALLOC_HOOK_DEFINE(ACE_Singleton_Strategy)

template<class SVC_HANDLER>
ACE_Recycling_Strategy<SVC_HANDLER>::~ACE_Recycling_Strategy (void)
{
}

template<class SVC_HANDLER> int
ACE_Recycling_Strategy<SVC_HANDLER>::assign_recycler (SVC_HANDLER *svc_handler,
                                                      ACE_Connection_Recycling_Strategy *recycler,
                                                      const void *recycling_act)
{
  svc_handler->recycler (recycler, recycling_act);
  return 0;
}

template<class SVC_HANDLER> int
ACE_Recycling_Strategy<SVC_HANDLER>::prepare_for_recycling (SVC_HANDLER *svc_handler)
{
  return svc_handler->recycle ();
}

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
ACE_DLL_Strategy<SVC_HANDLER>::open (const char dll_name[],
                                     const char factory_function[],
                                     const char svc_name[],
                                     ACE_Service_Repository *svc_rep,
                                     ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::open");
  this->inherited::open (thr_mgr);
  ACE_OS::strncpy (this->dll_name_, dll_name);
  ACE_OS::strncpy (this->factory_function_, factory_function);
  ACE_OS::strncpy (this->svc_name_, svc_name);
  this->svc_rep_ = svc_rep;
  return 0;
}

template <class SVC_HANDLER>
ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy (const char dll_name[],
                                                 const char factory_function[],
                                                 const char svc_name[],
                                                 ACE_Service_Repository *svc_rep,
                                                 ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy");
  if (this->open (dll_name,
                  factory_function,
                  svc_name,
                  svc_rep, thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("open")));
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
  SVC_HANDLER *(*factory)(void) =
    (SVC_HANDLER *(*)(void)) ACE_OS::dlsym (handle,
                                            this->factory_function_);

  // Call the factory function to obtain the new SVC_Handler (should
  // use RTTI here when it becomes available...)
  SVC_HANDLER *svc_handler;

  ACE_ALLOCATOR_RETURN (svc_handler, (*factory)(), -1);

  if (svc_handler != 0)
    {
      // Create an ACE_Service_Type containing the SVC_Handler and
      // insert into this->svc_rep_;

      ACE_Service_Type_Impl *stp =
        new ACE_Service_Object_Type (svc_handler, this->svc_name_);

      if (stp == 0)
        {
          errno = ENOMEM;
          return -1;
        }

      ACE_Service_Type *srp =
        new ACE_Service_Type (this->svc_name_, stp, handle, 1);

      if (srp == 0)
        {
          delete stp;
          errno = ENOMEM;
          return -1;
        }

      if (this->svc_rep_->insert (srp) == -1)
        return -1;
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

  int result = 0;

  // See if we should enable non-blocking I/O on the <svc_handler>'s
  // peer.
  if (ACE_BIT_ENABLED (this->flags_, ACE_NONBLOCK) != 0)
    {
      if (svc_handler->peer ().enable (ACE_NONBLOCK) == -1)
        result = -1;
    }
  // Otherwise, make sure it's disabled by default.
  else if (svc_handler->peer ().disable (ACE_NONBLOCK) == -1)
    result = -1;

  if (result == 0 && svc_handler->open (arg) == -1)
    result = -1;

  if (result == -1)
    svc_handler->close (0);

  return result;
}

template <class SVC_HANDLER>
ACE_Concurrency_Strategy<SVC_HANDLER>::ACE_Concurrency_Strategy (int flags)
  : flags_ (flags)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy");
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
ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy (int flags)
  : ACE_Concurrency_Strategy<SVC_HANDLER> (flags),
    reactor_ (0),
    mask_ (ACE_Event_Handler::NULL_MASK)
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

  int result = 0;

  if (this->reactor_ == 0)
    result = -1;

  // Register with the Reactor with the appropriate <mask>.
  else if (this->reactor_->register_handler (svc_handler, this->mask_) == -1)
    result = -1;

  // If the implementation of the reactor uses event associations
  else if (this->reactor_->uses_event_associations ())
    {
      // If we don't have non-block on, it won't work with
      // WFMO_Reactor
      // This maybe too harsh
      // if (!ACE_BIT_ENABLED (this->flags_, ACE_NONBLOCK))
      // goto failure;
      if (svc_handler->open (arg) != -1)
        return 0;
      else
        result = -1;
    }
  else
    // Call up to our parent to do the SVC_HANDLER initialization.
    return this->inherited::activate_svc_handler (svc_handler, arg);

  if (result == -1)
    svc_handler->close (0);

  return result;
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
                                        size_t n_threads,
                                        int flags)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::open");
  this->thr_mgr_ = thr_mgr;
  this->n_threads_ = n_threads;
  this->thr_flags_ = thr_flags;
  this->flags_ = flags;

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
                                                       size_t n_threads,
                                                       int flags)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy");

  if (this->open (thr_mgr, thr_flags, n_threads, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
                "ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy"));
}

template <class SVC_HANDLER>
ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy (int flags)
  : ACE_Concurrency_Strategy<SVC_HANDLER> (flags),
    thr_mgr_ (0),
    thr_flags_ (0),
    n_threads_ (1)
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
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
   int restart,
   ACE_Reactor *reactor)
  : reactor_ (reactor)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy");

  if (this->open (local_addr, restart) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("open")));
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
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy (ACE_Reactor *reactor)
  : reactor_ (reactor)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler");

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->reactor_->uses_event_associations ();

  if (this->acceptor_.accept (svc_handler->peer (), // stream
                              0, // remote address
                              0, // timeout
                              1, // restart
                              reset_new_handle  // reset new handler
                              ) == -1)
    {
      // Close down handler to avoid memory leaks.
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
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("(%P|%t) %p\n"),  ASYS_TEXT ("close")));
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

  return this->connector_.connect (sh->peer (),
                                   remote_addr,
                                   timeout,
                                   local_addr,
                                   reuse_addr,
                                   flags,
                                   perms);
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
                                         ACE_Reactor *reactor,
                                         int flags)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::open");
  this->n_processes_ = n_processes;
  this->acceptor_ = acceptor;
  this->reactor_ = reactor;
  this->flags_ = flags;

  return 0;
}

template <class SVC_HANDLER>
ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy (size_t n_processes,
                                                         ACE_Event_Handler *acceptor,
                                                         ACE_Reactor *reactor,
                                                         int flags)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy");
  this->open (n_processes, acceptor, reactor, flags);
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
      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("fork")), -1);
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
ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy (SVC_HANDLER *)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy");
}

template <class SVC_HANDLER>
ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy");
}

template <class SVC_HANDLER> void
ACE_Scheduling_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
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

  if (scheduler == 0 || scheduler->reactor () == 0)
    this->reactor_ = ACE_Reactor::instance ();
  else
    this->reactor_ = this->scheduler_->reactor ();
}

template <class SVC_HANDLER> int
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend");
  return this->reactor_->suspend_handlers ();
}

template <class SVC_HANDLER> int
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume");
  return this->reactor_->resume_handlers ();
}

template <class SVC_HANDLER> void
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::dump");

  ACE_Scheduling_Strategy<SVC_HANDLER>::dump ();
}

template <class SVC_HANDLER>
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy
  (SVC_HANDLER *scheduler)
  : ACE_Scheduling_Strategy<SVC_HANDLER> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy");

  if (scheduler == 0 || scheduler->thr_mgr () == 0)
    this->thr_mgr_ = ACE_Thread_Manager::instance ();
  else
    this->thr_mgr_ = this->scheduler_->thr_mgr ();
}

template <class SVC_HANDLER> int
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend");
  return this->thr_mgr_->suspend_all ();
}

template <class SVC_HANDLER> int
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume");
  return this->thr_mgr_->resume_all ();
}

template <class SVC_HANDLER> void
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::dump (void) const
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::dump");

  ACE_Scheduling_Strategy<SVC_HANDLER>::dump ();
}

template <class T>
ACE_Recyclable<T>::ACE_Recyclable (void)
  : recyclable_ (0),
    t_ (T ())
{
}

template <class T>
ACE_Recyclable<T>::ACE_Recyclable (const T &t, int recyclable)
  : recyclable_ (recyclable),
    t_ (t)
{
}

template <class T>
ACE_Recyclable<T>::~ACE_Recyclable (void)
{
}

template <class T> int
ACE_Recyclable<T>::operator== (const ACE_Recyclable<T> &rhs) const
{
  if (!this->recyclable ())
    return 0;
  else
    return this->t_ == rhs.t_;
}

template <class T> int
ACE_Recyclable<T>::recyclable (void) const
{
  return this->recyclable_;
}

template <class T> void
ACE_Recyclable<T>::recyclable (int new_value)
{
  this->recyclable_ = new_value;
}

template <class T>
ACE_Hash_Recyclable<T>::ACE_Hash_Recyclable (void)
  : ACE_Recyclable<T> ()
{
}

template <class T>
ACE_Hash_Recyclable<T>::ACE_Hash_Recyclable (const T &t, int recyclable)
  : ACE_Recyclable<T> (t, recyclable)
{
}

template <class T>
ACE_Hash_Recyclable<T>::~ACE_Hash_Recyclable (void)
{
}

template <class T> u_long
ACE_Hash_Recyclable<T>::hash (void) const
{
  return this->t_.hash ();
}


template<class ADDR_T>
ACE_Hash_Addr<ADDR_T>::ACE_Hash_Addr (void)
  : hash_value_ (0)
{
}

template<class ADDR_T>
ACE_Hash_Addr<ADDR_T>::ACE_Hash_Addr (const ADDR_T &a)
  : hash_value_ (0),
    addr_ (a)
{
  this->hash ();
}

template<class ADDR_T> u_long
ACE_Hash_Addr<ADDR_T>::hash (void) const
{
  // In doing the check below, we take chance of paying a performance
  // price when the hash value is zero.  But, that will (hopefully)
  // happen far less often than a non-zero value, so this caching
  // strategy should pay off, esp. if hash computation is expensive
  // relative to the simple comparison.

  if (this->hash_value_ == 0)
    ((ACE_Hash_Addr<ADDR_T> *) this)->hash_value_ = this->hash_i (addr_);

  return this->hash_value_;
}

template<class ADDR_T> size_t
ACE_Hash_Addr<ADDR_T>::hash_i (const ADDR_T &b) const
{
  ACE_UNUSED_ARG (b);
  return 0;
}

template<class ADDR_T> int
ACE_Hash_Addr<ADDR_T>::operator== (const ACE_Hash_Addr<ADDR_T> &rhs) const
{
  return this->addr_ == rhs.addr_;
}

template <class SVC_HANDLER> int
ACE_NOOP_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&)
{
  ACE_TRACE ("ACE_NOOP_Creation_Strategy<SVC_HANDLER>::make_svc_handler");
  return 0;
}

template <class SVC_HANDLER> int
ACE_NOOP_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *,
                                                                  void *)
{
  ACE_TRACE ("ACE_NOOP_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler");
  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX>
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::ACE_Cached_Connect_Strategy
  (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Recycling_Strategy<SVC_HANDLER> *rec_s)
    : creation_strategy_ (0),
      delete_creation_strategy_ (0),
      concurrency_strategy_ (0),
      delete_concurrency_strategy_ (0),
      recycling_strategy_ (0),
      delete_recycling_strategy_ (0)
{
  if (this->open (cre_s, con_s, rec_s) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Cached_Connect_Strategy::ACE_Cached_Connect_Strategy")));
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX>
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::~ACE_Cached_Connect_Strategy (void)
{
  if (this->delete_creation_strategy_)
    delete this->creation_strategy_;
  this->delete_creation_strategy_ = 0;
  this->creation_strategy_ = 0;

  if (this->delete_concurrency_strategy_)
    delete this->concurrency_strategy_;
  this->delete_concurrency_strategy_ = 0;
  this->concurrency_strategy_ = 0;

  if (this->delete_recycling_strategy_)
    delete this->recycling_strategy_;
  this->delete_recycling_strategy_ = 0;
  this->recycling_strategy_ = 0;

  // Close down all cached service handlers.
  CONNECTION_MAP_ENTRY *entry;
  for (CONNECTION_MAP_ITERATOR iterator (connection_cache_);
       iterator.next (entry);
       iterator.advance ())
    {
      entry->int_id_->recycler (0, 0);
      entry->int_id_->close ();
    }
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::open
  (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Recycling_Strategy<SVC_HANDLER> *rec_s)
{
  // Initialize the creation strategy.

  // First we decide if we need to clean up.
  if (this->creation_strategy_ != 0 &&
      this->delete_creation_strategy_ != 0 &&
      cre_s != 0)
    {
      delete this->creation_strategy_;
      this->creation_strategy_ = 0;
      this->delete_creation_strategy_ = 0;
    }

  if (cre_s != 0)
    this->creation_strategy_ = cre_s;
  else if (this->creation_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->creation_strategy_,
                      CREATION_STRATEGY, -1);
      this->delete_creation_strategy_ = 1;
    }

  // Initialize the concurrency strategy.

  if (this->concurrency_strategy_ != 0 &&
      this->delete_concurrency_strategy_ != 0 &&
      con_s != 0)
    {
      delete this->concurrency_strategy_;
      this->concurrency_strategy_ = 0;
      this->delete_concurrency_strategy_ = 0;
    }

  if (con_s != 0)
    this->concurrency_strategy_ = con_s;
  else if (this->concurrency_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->concurrency_strategy_,
                      CONCURRENCY_STRATEGY, -1);
      this->delete_concurrency_strategy_ = 1;
    }

  // Initialize the recycling strategy.

  if (this->recycling_strategy_ != 0 &&
      this->delete_recycling_strategy_ != 0 &&
      rec_s != 0)
    {
      delete this->recycling_strategy_;
      this->recycling_strategy_ = 0;
      this->delete_recycling_strategy_ = 0;
    }

  if (rec_s != 0)
    this->recycling_strategy_ = rec_s;
  else if (this->recycling_strategy_ == 0)
    {
      ACE_NEW_RETURN (this->recycling_strategy_,
                      RECYCLING_STRATEGY, -1);
      this->delete_recycling_strategy_ = 1;
    }

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::make_svc_handler
  (SVC_HANDLER *&sh)
{
  return this->creation_strategy_->make_svc_handler (sh);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::activate_svc_handler
  (SVC_HANDLER *svc_handler)
{
  return this->concurrency_strategy_->activate_svc_handler (svc_handler);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::assign_recycler
  (SVC_HANDLER *svc_handler,
   ACE_Connection_Recycling_Strategy *recycler,
   const void *recycling_act)
{
  return this->recycling_strategy_->assign_recycler (svc_handler,
                                                     recycler,
                                                     recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::prepare_for_recycling
  (SVC_HANDLER *svc_handler)
{
  return this->recycling_strategy_->prepare_for_recycling (svc_handler);
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
  // User must not pass us svc_handlers. We will either find them a
  // non-busy one, or will create them a new one.
  if (sh != 0)
    {
      errno = EINVAL;
      return -1;
    }

  // Create the search key
  ADDRESS search_addr (remote_addr);
  CONNECTION_MAP_ENTRY *entry = 0;

  // Synchronization is required here as the setting of the recyclable
  // bit must be done atomically with the finding and binding of the
  // service handler in the cache.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  // Try to find the address in the cache.  Only if we don't find it
  // do we create a new <SVC_HANDLER> and connect it with the server.
  if (this->connection_cache_.find (search_addr, entry) == -1)
    {
      // Create a new svc_handler
      if (this->make_svc_handler (sh) == -1)
        return -1;

      // Actively establish the connection.  This is a timed blocking
      // connect.
      if (CONNECT_STRATEGY::connect_svc_handler (sh,
                                                 remote_addr,
                                                 timeout,
                                                 local_addr,
                                                 reuse_addr,
                                                 flags,
                                                 perms) == -1)
        {
          // If connect() failed because of timeouts, we have to
          // reject the connection entirely. This is necessary since
          // currently there is no way for the non-blocking connects
          // to complete and for the <Connector> to notify the cache
          // of the completion of connect().
          if (errno == EWOULDBLOCK)
            errno = ENOTSUP;
          return -1;
        }
      // Insert the new SVC_HANDLER instance into the cache.
      else
        {
          // Activate immediately if we are connected.
          if (this->activate_svc_handler (sh))
            return -1;

          // Create the key
          ADDRESS server_addr (remote_addr);
          if (this->connection_cache_.bind (server_addr,
                                            sh,
                                            entry) == -1)
            return -1;

          // Set the recycler and the recycling act
          this->assign_recycler (sh, this, entry);
        }
    }
  else
    // We found a cached svc_handler.
    {
      // Get the cached <svc_handler>
      sh = entry->int_id_;

      // Tell the <svc_handler> that it should prepare itself for being
      // recycled.
      this->prepare_for_recycling (sh);
    }

  // Mark the <svc_handler> in the cache as being <in_use>.
  // Therefore recyclable is 0.
  entry->ext_id_.recyclable (0);

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::cache (const void *recycling_act)
{
  // Synchronization is required here as the setting of the recyclable
  // bit must be done atomically with respect to other threads that
  // are querying the cache.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  // The wonders and perils of ACT
  CONNECTION_MAP_ENTRY *entry = (CONNECTION_MAP_ENTRY *) recycling_act;

  // Mark the <svc_handler> in the cache as not being <in_use>.
  // Therefore recyclable is 1.
  entry->ext_id_.recyclable (1);

  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::purge (const void *recycling_act)
{
  // Excluded other threads from changing cache while we take this
  // entry out.
  ACE_GUARD_RETURN (MUTEX, ace_mon, this->lock_, -1);

  // The wonders and perils of ACT
  CONNECTION_MAP_ENTRY *entry = (CONNECTION_MAP_ENTRY *) recycling_act;

  return this->connection_cache_.unbind (entry);
}

#endif /* ACE_STRATEGIES_T_C */

/* -*- C++ -*- */
// $Id$

template<class SVC_HANDLER> ASYS_INLINE
ACE_Recycling_Strategy<SVC_HANDLER>::~ACE_Recycling_Strategy (void)
{
}

template<class SVC_HANDLER> ASYS_INLINE int
ACE_Recycling_Strategy<SVC_HANDLER>::assign_recycler (SVC_HANDLER *svc_handler,
                                                      ACE_Connection_Recycling_Strategy *recycler,
                                                      const void *recycling_act)
{
  svc_handler->recycler (recycler, recycling_act);
  return 0;
}

template<class SVC_HANDLER> ASYS_INLINE int
ACE_Recycling_Strategy<SVC_HANDLER>::prepare_for_recycling (SVC_HANDLER *svc_handler)
{
  return svc_handler->recycle ();
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Singleton_Strategy<SVC_HANDLER>::ACE_Singleton_Strategy (SVC_HANDLER *sh,
                                                             ACE_Thread_Manager *tm)
  : svc_handler_ (0),
    delete_svc_handler_ (1)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::ACE_Singleton_Strategy");
  if (this->open (sh, tm) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Singleton_Strategy")));
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy (void)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy");
  if (this->delete_svc_handler_ != 0)
    delete this->svc_handler_;
}

// Create a Singleton SVC_HANDLER by always returning the same
// SVC_HANDLER.

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Singleton_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::make_svc_handler");
  sh = this->svc_handler_;
  return 0;
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Creation_Strategy<SVC_HANDLER>::open (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::open");
  this->thr_mgr_ = thr_mgr;
  return 0;
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Creation_Strategy<SVC_HANDLER>::ACE_Creation_Strategy (ACE_Thread_Manager *thr_mgr)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::ACE_Creation_Strategy");
  if (this->open (thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Creation_Strategy")));
}

// Default behavior is to make a new SVC_HANDLER, passing in the
// Thread_Manager (if any).

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::make_svc_handler");

  if (sh == 0)
    ACE_NEW_RETURN (sh, SVC_HANDLER (this->thr_mgr_), -1);
  return 0;
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Creation_Strategy<SVC_HANDLER>::~ACE_Creation_Strategy (void)
{
  ACE_TRACE ("ACE_Creation_Strategy<SVC_HANDLER>::~ACE_Creation_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
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
                  svc_rep,
                  thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("open")));
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy (void)
{
  ACE_TRACE ("ACE_DLL_Strategy<SVC_HANDLER>::ACE_DLL_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Concurrency_Strategy<SVC_HANDLER>::ACE_Concurrency_Strategy (int flags)
  : flags_ (flags)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy (void)
{
  ACE_TRACE ("ACE_Concurrency_Strategy<SVC_HANDLER>::~ACE_Concurrency_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy (ACE_Reactor *reactor,
                                                           ACE_Reactor_Mask mask,
                                                           int flags)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy");

  if (this->open (reactor,
                  mask,
                  flags) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy")));
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy (int flags)
  : ACE_Concurrency_Strategy<SVC_HANDLER> (flags),
    reactor_ (0),
    mask_ (ACE_Event_Handler::NULL_MASK)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Reactive_Strategy<SVC_HANDLER>::~ACE_Reactive_Strategy (void)
{
  ACE_TRACE ("ACE_Reactive_Strategy<SVC_HANDLER>::~ACE_Reactive_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy (ACE_Thread_Manager *thr_mgr,
                                                       long thr_flags,
                                                       size_t n_threads,
                                                       int flags)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy");

  if (this->open (thr_mgr,
                  thr_flags,
                  n_threads,
                  flags) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy")));
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy (int flags)
  : ACE_Concurrency_Strategy<SVC_HANDLER> (flags),
    thr_mgr_ (0),
    thr_flags_ (0),
    n_threads_ (1)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Thread_Strategy<SVC_HANDLER>::~ACE_Thread_Strategy (void)
{
  ACE_TRACE ("ACE_Thread_Strategy<SVC_HANDLER>::~ACE_Thread_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy (ACE_Reactor *reactor)
  : reactor_ (reactor)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Accept_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE ACE_HANDLE
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle");
  return this->peer_acceptor_.get_handle ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE ACE_PEER_ACCEPTOR &
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy");

  // Close the underlying acceptor.
  this->peer_acceptor_.close ();
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> ASYS_INLINE ACE_PEER_CONNECTOR &
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connector (void) const
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::connector");
  return (ACE_PEER_CONNECTOR &) this->connector_;
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> ASYS_INLINE
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::ACE_Connect_Strategy (void)
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::ACE_Connect_Strategy");
}

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1> ASYS_INLINE
ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::~ACE_Connect_Strategy (void)
{
  ACE_TRACE ("ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>::~ACE_Connect_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy (size_t n_processes,
                                                         ACE_Event_Handler *acceptor,
                                                         ACE_Reactor *reactor,
                                                         int avoid_zombies)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy");
  if (this->open (n_processes,
                  acceptor,
                  reactor,
                  avoid_zombies) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Process_Strategy")));
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Process_Strategy<SVC_HANDLER>::~ACE_Process_Strategy (void)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::~ACE_Process_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy (SVC_HANDLER *)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::ACE_Scheduling_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::~ACE_Scheduling_Strategy");
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Scheduling_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::suspend");
  return -1;
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Scheduling_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Scheduling_Strategy<SVC_HANDLER>::resume");
  return -1;
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::ACE_Schedule_All_Reactive_Strategy
  (SVC_HANDLER *scheduler)
  : ACE_Scheduling_Strategy<SVC_HANDLER> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::ACE_Schedule_All_Reactive_Strategy");

  if (scheduler == 0 || scheduler->reactor () == 0)
    this->reactor_ = ACE_Reactor::instance ();
  else
    this->reactor_ = scheduler->reactor ();
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::suspend");
  return this->reactor_->suspend_handlers ();
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Reactive_Strategy<SVC_HANDLER>::resume");
  return this->reactor_->resume_handlers ();
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy
  (SVC_HANDLER *scheduler)
  : ACE_Scheduling_Strategy<SVC_HANDLER> (scheduler)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::ACE_Schedule_All_Threaded_Strategy");

  if (scheduler == 0 || scheduler->thr_mgr () == 0)
    this->thr_mgr_ = ACE_Thread_Manager::instance ();
  else
    this->thr_mgr_ = scheduler->thr_mgr ();
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::suspend");
  return this->thr_mgr_->suspend_all ();
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume (void)
{
  ACE_TRACE ("ACE_Schedule_All_Threaded_Strategy<SVC_HANDLER>::resume");
  return this->thr_mgr_->resume_all ();
}

template <class T> ASYS_INLINE
ACE_Refcounted_Hash_Recyclable<T>::ACE_Refcounted_Hash_Recyclable (void)
  : ACE_Refcountable (0),
    ACE_Hashable (),
    ACE_Recyclable (ACE_RECYCLABLE_UNKNOWN),
    t_ ()
{
}

template <class T> ASYS_INLINE
ACE_Refcounted_Hash_Recyclable<T>::ACE_Refcounted_Hash_Recyclable (const T &t,
                                                                   int refcount,
                                                                   ACE_Recyclable_State state)
  : ACE_Refcountable (refcount),
    ACE_Hashable (),
    ACE_Recyclable (state),
    t_ (t)
{
}

template <class T> ASYS_INLINE
ACE_Refcounted_Hash_Recyclable<T>::~ACE_Refcounted_Hash_Recyclable (void)
{
}

template <class T> ASYS_INLINE u_long
ACE_Refcounted_Hash_Recyclable<T>::hash_i (void) const
{
  return this->t_.hash ();
}

template <class T> ASYS_INLINE T &
ACE_Refcounted_Hash_Recyclable<T>::subject (void)
{
  return this->t_;
}

template <class T> ASYS_INLINE int
ACE_Refcounted_Hash_Recyclable<T>::operator== (const ACE_Refcounted_Hash_Recyclable<T> &rhs) const
{
  return this->recycle_state () == rhs.recycle_state () &&
         this->t_ == rhs.t_;
}

template <class T> ASYS_INLINE int
ACE_Refcounted_Hash_Recyclable<T>::operator!= (const ACE_Refcounted_Hash_Recyclable<T> &rhs) const
{
  return !this->operator== (rhs);
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_NOOP_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&)
{
  ACE_TRACE ("ACE_NOOP_Creation_Strategy<SVC_HANDLER>::make_svc_handler");
  return 0;
}

template <class SVC_HANDLER> ASYS_INLINE int
ACE_NOOP_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (SVC_HANDLER *,
                                                                  void *)
{
  ACE_TRACE ("ACE_NOOP_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler");
  return 0;
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> ASYS_INLINE int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::make_svc_handler
  (SVC_HANDLER *&sh)
{
  return this->creation_strategy_->make_svc_handler (sh);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> ASYS_INLINE int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::activate_svc_handler
  (SVC_HANDLER *svc_handler)
{
  return this->concurrency_strategy_->activate_svc_handler (svc_handler);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> ASYS_INLINE int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::assign_recycler
  (SVC_HANDLER *svc_handler,
   ACE_Connection_Recycling_Strategy *recycler,
   const void *recycling_act)
{
  return this->recycling_strategy_->assign_recycler (svc_handler,
                                                     recycler,
                                                     recycling_act);
}

template<class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class MUTEX> ASYS_INLINE int
ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>::prepare_for_recycling
  (SVC_HANDLER *svc_handler)
{
  return this->recycling_strategy_->prepare_for_recycling (svc_handler);
}

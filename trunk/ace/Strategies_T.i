/* -*- C++ -*- */
// $Id$

template<class ADDR_T> ASYS_INLINE
ACE_Hash_Addr<ADDR_T>::~ACE_Hash_Addr (void)
{
}

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
  : svc_handler_ (0)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::ACE_Singleton_Strategy");
  this->open (sh, tm);
}

template <class SVC_HANDLER> ASYS_INLINE
ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy (void)
{
  ACE_TRACE ("ACE_Singleton_Strategy<SVC_HANDLER>::~ACE_Singleton_Strategy");
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
  this->open (thr_mgr);
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
                  svc_rep, thr_mgr) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("open")));
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

  if (this->open (reactor, mask, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
                "ACE_Reactive_Strategy<SVC_HANDLER>::ACE_Reactive_Strategy"));
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

  if (this->open (thr_mgr, thr_flags, n_threads, flags) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
                "ACE_Thread_Strategy<SVC_HANDLER>::ACE_Thread_Strategy"));
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

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE int
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr, int restart)
{
  return this->acceptor_.open (local_addr, restart);
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
  return this->acceptor_.get_handle ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE ACE_PEER_ACCEPTOR &
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->acceptor_;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ASYS_INLINE
ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy (void)
{
  ACE_TRACE ("ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Accept_Strategy");

  if (this->acceptor_.close () == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("(%P|%t) %p\n"),  ASYS_TEXT ("close")));
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
                                                         int flags)
{
  ACE_TRACE ("ACE_Process_Strategy<SVC_HANDLER>::ACE_Process_Strategy");
  this->open (n_processes, acceptor, reactor, flags);
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
ACE_Recyclable<T>::ACE_Recyclable (void)
  : recyclable_ (0),
    t_ (T ())
{
}

template <class T> ASYS_INLINE
ACE_Recyclable<T>::ACE_Recyclable (const T &t, int recyclable)
  : recyclable_ (recyclable),
    t_ (t)
{
}

template <class T> ASYS_INLINE
ACE_Recyclable<T>::~ACE_Recyclable (void)
{
}

template <class T> ASYS_INLINE int
ACE_Recyclable<T>::operator== (const ACE_Recyclable<T> &rhs) const
{
  if (!this->recyclable ())
    return 0;
  else
    return this->t_ == rhs.t_;
}

template <class T> ASYS_INLINE int
ACE_Recyclable<T>::recyclable (void) const
{
  return this->recyclable_;
}

template <class T> ASYS_INLINE void
ACE_Recyclable<T>::recyclable (int new_value)
{
  this->recyclable_ = new_value;
}

template <class T> ASYS_INLINE
ACE_Hash_Recyclable<T>::ACE_Hash_Recyclable (void)
  : ACE_Recyclable<T> ()
{
}

template <class T> ASYS_INLINE
ACE_Hash_Recyclable<T>::ACE_Hash_Recyclable (const T &t, int recyclable)
  : ACE_Recyclable<T> (t, recyclable)
{
}

template <class T> ASYS_INLINE
ACE_Hash_Recyclable<T>::~ACE_Hash_Recyclable (void)
{
}

template <class T> ASYS_INLINE u_long
ACE_Hash_Recyclable<T>::hash (void) const
{
  return this->t_.hash ();
}

template <class T> ASYS_INLINE int
ACE_Hash_Recyclable<T>::operator== (const ACE_Recyclable<T> &rhs) const
{
  return ACE_Recyclable<T>::operator== (rhs);
}

template<class ADDR_T> ASYS_INLINE
ACE_Hash_Addr<ADDR_T>::ACE_Hash_Addr (void)
  : hash_value_ (0)
{
}

template<class ADDR_T> ASYS_INLINE
ACE_Hash_Addr<ADDR_T>::ACE_Hash_Addr (const ADDR_T &a)
  : hash_value_ (0),
    addr_ (a)
{
  this->hash ();
}

template<class ADDR_T> ASYS_INLINE u_long
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

template<class ADDR_T> ASYS_INLINE size_t
ACE_Hash_Addr<ADDR_T>::hash_i (const ADDR_T &b) const
{
  ACE_UNUSED_ARG (b);
  return 0;
}

template<class ADDR_T> ASYS_INLINE int
ACE_Hash_Addr<ADDR_T>::operator== (const ACE_Hash_Addr<ADDR_T> &rhs) const
{
  return this->addr_ == rhs.addr_;
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

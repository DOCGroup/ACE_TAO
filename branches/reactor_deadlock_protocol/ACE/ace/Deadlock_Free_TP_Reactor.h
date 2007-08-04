// -*- C++ -*-

//=============================================================================
/**
 *  @file    TP_Reactor.h
 *
 *  Deadlock_Free_TP_Reactor.h,v 4.42 2004/08/20 15:21:02 bala Exp
 *
 *  The <ACE_TP_Reactor> (aka, Thread Pool Reactor) uses the
 *  Leader/Followers pattern to demultiplex events among a pool of
 *  threads.  When using a thread pool reactor, an application
 *  pre-spawns a _fixed_ number of threads.  When these threads
 *  invoke the <ACE_TP_Reactor>'s <handle_events> method, one thread
 *  will become the leader and wait for an event.  The other
 *  follower threads will queue up waiting for their turn to become
 *  the leader.  When an event occurs, the leader will pick a
 *  follower to become the leader and go on to handle the event.
 *  The consequence of using <ACE_TP_Reactor> is the amortization of
 *  the costs used to creating threads.  The context switching cost
 *  will also reduce.  More over, the total resources used by
 *  threads are bounded because there are a fixed number of threads.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Paul Oberlin <pauloberlin@gmail.com>
 */
//=============================================================================

#ifndef ACE_DEADLOCK_FREE_TP_REACTOR_H
#define ACE_DEADLOCK_FREE_TP_REACTOR_H

#include /**/ "ace/pre.h"

#include "ace/TP_Reactor.h"
#include "ace/DA_Strategy_Base.h"
#include "ace/Event_Handler.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS.h"
#include "ace/OS_NS_Thread.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

typedef ACE_Hash_Map_Entry<ACE_Event_Handler *, int> HASH_EH_ENTRY;


typedef ACE_Hash_Map_Manager_Ex<void *,
                                int,
                                ACE_Hash<void *>,
                                ACE_Equal_To<void *>,
                                ACE_Thread_Mutex> HASH_ANNOTATIONS_MAP;

typedef ACE_Hash_Map_Iterator_Ex<void *,
                                 int,
                                 ACE_Hash<void *>,
                                 ACE_Equal_To<void *>,
                                 ACE_Thread_Mutex> HASH_ANNOTATIONS_ITER;

typedef ACE_Hash_Map_Const_Iterator_Ex<void *,
                                       int,
                                       ACE_Hash<void *>,
                                       ACE_Equal_To<void *>,
                                       ACE_Thread_Mutex> HASH_ANNOTATIONS_CONST_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<void *,
                                         int,
                                         ACE_Hash<void *>,
                                         ACE_Equal_To<void *>,
                                         ACE_Thread_Mutex> HASH_ANNOTATIONS_REVERSE_ITER;

typedef HASH_ANNOTATIONS_MAP Deadlock_Free_TPR_Annotations_Table;

class ACE_Deadlock_Free_TP_Reactor_Impl: public ACE_TP_Reactor
{
friend class ACE_Singleton<ACE_Deadlock_Free_TP_Reactor_Impl, ACE_Recursive_Thread_Mutex>;
public:
  /*
  ACE_Deadlock_Free_TP_Reactor (ACE_Sig_Handler * = 0,
			     ACE_Timer_Queue * = 0,
			     int mask_signals = 1,
			     int s_queue = ACE_Select_Reactor_Token::FIFO);
*/
  //this static method is use to "prime the pump" before the first time
  //the singleton instance is called.  
  //da_strategy cannot be null.  The reactor will delete it during
  //its destructor
  static void initialize(size_t max_number_of_handles,
        DA_Strategy_Base<ACE_HANDLE>* da_strategy,
				int restart = 0,
				ACE_Sig_Handler * = 0,
				ACE_Timer_Queue * = 0,
				int mask_signals = 1,
				int s_queue = ACE_Select_Reactor_Token::FIFO);

  
  virtual int filter_deadlock_potential_handles (ACE_HANDLE upcall_handle);
  virtual void pre_upcall_hook (ACE_HANDLE upcall_handle);
  virtual void post_upcall_hook (ACE_HANDLE upcall_handle);
  virtual int  get_annotation (ACE_Event_Handler* eh);
  virtual int  get_annotation (ACE_HANDLE h);
  virtual int add_annotation (ACE_Event_Handler* eh, int annotation);
  virtual int  remove_annotation (ACE_Event_Handler* eh);

private:
  ACE_Deadlock_Free_TP_Reactor_Impl();
  virtual ~ACE_Deadlock_Free_TP_Reactor_Impl();
  ACE_RW_Thread_Mutex lock_;
  DA_Strategy_Base<ACE_HANDLE>* da_strategy_;
 
};

typedef ACE_Singleton<ACE_Deadlock_Free_TP_Reactor_Impl, ACE_Recursive_Thread_Mutex> ACE_Deadlock_Free_TP_Reactor;

#if defined (__ACE_INLINE__)
#include "ace/Deadlock_Free_TP_Reactor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_DEADLOCK_FREE_TP_REACTOR_H */

/* -*- C++ -*- */
/**
 *  @file   ESF_Delayed_Changes.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_DELAYED_CHANGES_H
#define TAO_ESF_DELAYED_CHANGES_H

#include "ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ESF_Busy_Lock.h"
#include "ace/Containers.h"

class ACE_Command_Base;

template<class Target,class Object> class TAO_ESF_Connected_Command;
template<class Target,class Object> class TAO_ESF_Disconnected_Command;
template<class Target,class Object> class TAO_ESF_Reconnected_Command;
template<class Target> class TAO_ESF_Shutdown_Command;

class ACE_Command_Base;

/**
 * @class TAO_ESF_Delayed_Changes
 *
 * @brief TAO_ESF_Delayed_Operations
 *
 * This class implements the Delayed Operations protocol to solve
 * the concurrency challenges outlined in the documentation of
 * TAO_ESF_Proxy_Collection.
 * In short the class delays changes by putting them on an
 * "operation queue", the operations are stored as command objects
 * in this queue and executed once the system is quiescent
 * (i.e. no threads are iterating over the collection).
 * The algorithm implemented so far is:
 * - If a thread is using the set then it increases the busy
 * count, this is done by calling the busy() method. Once the
 * thread has stopped using the collection the idle() method is
 * invoked and the busy count is decreased.
 * A helper class (Busy_Lock) is used to hide this protocol
 * behind the familiar GUARD idiom.
 * - If the busy count reaches the busy_hwm then the thread must
 * wait until the count reaches 0 again.
 * This can be used to control the maximum concurrency in the
 * EC, matching it (for example) with the number of
 * processors. Setting the concurrency to a high value (say one
 * million) allows for an arbitrary number of threads to execute
 * concurrently.
 * - If a modification is posted to the collection we need to
 * execute it at some point.
 * Just using the busy_hwm would not work, the HWM may not be
 * reached ever, so another form of control is needed.
 * Instead we use another counter, that keeps track of how many
 * threads have used the set since the modification was
 * posted. If this number of threads reaches max_write_delay then
 * we don't allow any more threads to go in, eventually the
 * thread count reaches 0 and we can proceed with the operations.
 * - There is one aspect of concurrency that can be problematic: if
 * thread pushes events as part of an upcall then the same thread
 * could be counted twice, we need to keep track of the threads
 * that are dispatching events and not increase (or decrease) the
 * reference count when a thread iterates twice over the same
 * set.
 * This solves the major problems, but there are other issues to
 * be addressed:
 * + How do we ensure that the operations are eventually executed?
 * + How do we simplify the execution of the locking protocol for
 * clients of this class?
 * + How do we minimize overhead for single threaded execution?
 * + How do we minimize the overhead for the cases where the
 * threads dispatching events don't post changes to the
 * collection?
 */
template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_ESF_Delayed_Changes : public TAO_ESF_Proxy_Collection<PROXY>
{
public:
  TAO_ESF_Delayed_Changes (void);
  TAO_ESF_Delayed_Changes (const COLLECTION &collection);

  int busy (void);
  int idle (void);
  int execute_delayed_operations (void);

  void connected_i (PROXY *proxy
                    ACE_ENV_ARG_DECL);
  void reconnected_i (PROXY *proxy
                    ACE_ENV_ARG_DECL);
  void disconnected_i (PROXY *proxy
                     ACE_ENV_ARG_DECL);
  void shutdown_i (ACE_ENV_SINGLE_ARG_DECL);

  typedef TAO_ESF_Connected_Command<TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Connected_Command;
  typedef TAO_ESF_Reconnected_Command<TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Reconnected_Command;
  typedef TAO_ESF_Disconnected_Command<TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Disconnected_Command;
  typedef TAO_ESF_Shutdown_Command<TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE> > Shutdown_Command;

  // = The TAO_ESF_Proxy methods
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker
                         ACE_ENV_ARG_DECL);
  virtual void connected (PROXY *proxy
                          ACE_ENV_ARG_DECL);
  virtual void reconnected (PROXY *proxy
                            ACE_ENV_ARG_DECL);
  virtual void disconnected (PROXY *proxy
                             ACE_ENV_ARG_DECL);
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

private:
  COLLECTION collection_;

  typedef TAO_ESF_Busy_Lock_Adapter<TAO_ESF_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE> > Busy_Lock;

  Busy_Lock lock_;

  ACE_SYNCH_MUTEX_T busy_lock_;

  ACE_SYNCH_CONDITION_T busy_cond_;

  CORBA::ULong busy_count_;

  CORBA::ULong write_delay_count_;

  /// Control variables for the concurrency policies.
  CORBA::ULong busy_hwm_;
  CORBA::ULong max_write_delay_;

  ACE_Unbounded_Queue<ACE_Command_Base*> command_queue_;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Delayed_Changes.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Delayed_Changes.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Delayed_Changes.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_DELAYED_CHANGES_H */

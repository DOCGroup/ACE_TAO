/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Proxy_Collection
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_EC_PROXY_COLLECTION_H
#define TAO_EC_PROXY_COLLECTION_H
#include "ace/pre.h"

#include "EC_Busy_Lock.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "ace/Containers.h"

template<class Target> class TAO_EC_Worker;

template<class PROXY>
class TAO_EC_Proxy_Collection
{
  // = TITLE
  //   EC_Proxy_Collection
  //
  // = DESCRIPTION
  //   Many components in the Event Channel need to keep a collection
  //   of proxies; these collections must be able to cope with several
  //   concurrency issues:
  //   + Some threads may need to iterate over the collection and
  //     invoke a method on each element.  Locking the collection
  //     while this is done is not feasible in all cases: under some
  //     configurations the same thread that is iterating over the
  //     collection may need to make changes to the set.
  //   + A recursive lock does not solve the concurrency problems
  //     because recursive changes to the collection still invalidate
  //     the iterators.
  //
  //   There are several solutions to this problem (see the VARIANTS)
  //   section, and there is no single one that works bests in all
  //   cases.  As usual, we wish the strategize the protocol used to
  //   serialize iterations and changes to the collection.  This class
  //   encapsulates that protocol.
  //
  //   The reasons are too long to enumerate here, but the Iterator
  //   pattern is not appropriate in this case.  We use Function
  //   Objects (aka Worker) to perform a client-defined task on each
  //   member.
  //
  //   Regular member functions are used to insert, remove and update
  //   members of the collection and to shutdown (i.e. perform final
  //   cleanup operations).
  //
  //   The class must also collaborate with other components of the
  //   EC to efficiently and safely perform memory managment of the
  //   members in the collection.
  //
  // = VARIANTS
  //
  //   We identify several sources of variation:
  //
  //   + Immediate_Changes: in this variant the iteration in performed
  //   while holding some kind of synchronization primitive, such as a
  //   thread mutex, a recursive mutex, a RW lock, etc.
  //   This is only useful in configurations where a separate thread
  //   dispatches the events, and thus, can only be used with real
  //   locks.
  //
  //   + Copy_On_Read: before performing the iteration the collection
  //   is duplicated into a temporary array.  Thus no locks are held
  //   during the iteration.  This is a very expensive approach, but
  //   useful in many cases.
  //   The kind of lock is also strategized in this case.
  //
  //   + Copy_On_Write: this is very similar to the previous approach,
  //   but the collection is only duplicated when a change is required
  //   while some thread is performing an iteration.  The iteration
  //   continues over the original copy, while the changes are
  //   performed in the duplicate.  The new copy of the collection is
  //   used for any subsequent operations, the original is discarded
  //   when the last thread using it completes its work.
  //   This approach optimizes for the case where no changes are
  //   is duplicated into a temporary array.  Thus no locks are held
  //   during the iteration.  This is a very expensive approach, but
  //   useful in many cases.
  //   The kind of lock is also strategized in this case.
  //
  //   + Delayed_Changes: before starting the iteration a counter is
  //   incremented, this counter is used to keep track of the number
  //   of threads concurrently using the collection.
  //   If a thread wants to perform a change to the collection it must
  //   first verify that there are no threads iterating over it.  If
  //   there are any threads then the thread queues the modification
  //   for later execution, using the Command pattern.
  //   The kind of lock is strategized, as this approach is used in
  //   single threaded configurations.
  //   There are two main variations:
  //     - An upcall can result in new dispatches: in this case we
  //       have to keep track of a the list of current threads using
  //       a Set, to avoid dead-locks.
  //       IMPLEMENTATION: the design is not complete, probably
  //       similar to the next one.
  //     - Otherwise we just need to control the concurrency using the
  //       algorithm described below.
  //
  //
  // = MEMORY MANAGMENT
  //   It assumes ownership of the proxies added to the collection,
  //   it increases the reference count.
  //
  // = LOCKING
  //   Locking is provided by derived classes.
  //
  // = TODO
  //
public:
  virtual ~TAO_EC_Proxy_Collection (void);
  // destructor

  virtual void for_each (TAO_EC_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV) = 0;
  // Iterate over the collection and invoke worker->work() for each
  // member of the collection.
  // This encapsulates

  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV) = 0;
  // Insert a new element into the collection.  The collection assumes
  // ownership of the element.

  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV) = 0;
  // Insert an element into the collection.  No errors can be raised
  // if the element is already present.
  // The collection assumes ownership, i.e. must invoke
  // <proxy->_decr_refcnt()> if the element is already present in the
  // collection.

  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV) = 0;
  // Remove an element from the collection.

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV) = 0;
  // The EC is shutting down, must release all the elements.
};

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_EC_Immediate_Changes : public TAO_EC_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_EC_Immediate_Changes
  //
  // = DESCRIPTION
  //   Implement the Immediate_
public:
  TAO_EC_Immediate_Changes (void);
  TAO_EC_Immediate_Changes (const COLLECTION &collection);

  // = The TAO_EC_Proxy methods
  virtual void for_each (TAO_EC_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV);
  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV);
  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);

private:
  COLLECTION collection_;

  ACE_LOCK lock_;
};

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_EC_Copy_On_Read : public TAO_EC_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_EC_Copy_On_Read
  //
  // = DESCRIPTION
  //   Implement the Copy_On_Read protocol
  //   The class is parametric on the kind of collection and locking
  //   mechanism used.
  //
  // = TODO
  //
public:
  TAO_EC_Copy_On_Read (void);
  TAO_EC_Copy_On_Read (const COLLECTION &collection);
  // Constructors

  // = The TAO_EC_Proxy methods
  virtual void for_each (TAO_EC_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV);
  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV);
  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);

private:
  COLLECTION collection_;

  ACE_LOCK lock_;
};

// ****************************************************************

template<class Target,class Object> class TAO_EC_Connected_Command;
template<class Target,class Object> class TAO_EC_Disconnected_Command;
template<class Target,class Object> class TAO_EC_Reconnected_Command;
template<class Target> class TAO_EC_Shutdown_Command;

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_EC_Delayed_Changes : public TAO_EC_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_EC_Delayed_Operations
  //
  // = DESCRIPTION
  //   This class implements the Delayed Operations protocol to solve
  //   the concurrency challenges outlined in the documentation of
  //   TAO_EC_Proxy_Collection.
  //   In short the class delays changes by putting them on an
  //   "operation queue", the operations are stored as command objects
  //   in this queue and executed once the system is quiescent
  //   (i.e. no threads are iterating over the collection).
  //
  //   The algorithm implemented so far is:
  //   - If a thread is using the set then it increases the busy
  //     count, this is done by calling the busy() method. Once the
  //     thread has stopped using the collection the idle() method is
  //     invoked and the busy count is decreased.
  //     A helper class (Busy_Lock) is used to hide this protocol
  //     behind the familiar GUARD idiom.
  //   - If the busy count reaches the busy_hwm then the thread must
  //     wait until the count reaches 0 again.
  //     This can be used to control the maximum concurrency in the
  //     EC, matching it (for example) with the number of
  //     processors. Setting the concurrency to a high value (say one
  //     million) allows for an arbitrary number of threads to execute
  //     concurrently.
  //  - If a modification is posted to the collection we need to
  //    execute it at some point.
  //    Just using the busy_hwm would not work, the HWM may not be
  //    reached ever, so another form of control is needed.
  //    Instead we use another counter, that keeps track of how many
  //    threads have used the set since the modification was
  //    posted. If this number of threads reaches max_write_delay then
  //    we don't allow any more threads to go in, eventually the
  //    thread count reaches 0 and we can proceed with the operations.
  //
  //  - There is one aspect of concurrency that can be problematic: if
  //    thread pushes events as part of an upcall then the same thread
  //    could be counted twice, we need to keep track of the threads
  //    that are dispatching events and not increase (or decrease) the
  //    reference count when a thread iterates twice over the same
  //    set.
  //
  //   This solves the major problems, but there are other issues to
  //   be addressed:
  //   + How do we ensure that the operations are eventually executed?
  //   + How do we simplify the execution of the locking protocol for
  //   clients of this class?
  //   + How do we minimize overhead for single threaded execution?
  //   + How do we minimize the overhead for the cases where the
  //     threads dispatching events don't post changes to the
  //     collection?
  //
public:
  TAO_EC_Delayed_Changes (void);
  TAO_EC_Delayed_Changes (const COLLECTION &collection);

  int busy (void);
  int idle (void);
  int execute_delayed_operations (void);

  void connected_i (PROXY *proxy,
                    CORBA::Environment &ACE_TRY_ENV);
  void reconnected_i (PROXY *proxy,
                    CORBA::Environment &ACE_TRY_ENV);
  void disconnected_i (PROXY *proxy,
                     CORBA::Environment &ACE_TRY_ENV);
  void shutdown_i (CORBA::Environment &ACE_TRY_ENV);

  typedef TAO_EC_Connected_Command<TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Connected_Command;
  typedef TAO_EC_Reconnected_Command<TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Reconnected_Command;
  typedef TAO_EC_Disconnected_Command<TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE>,PROXY> Disconnected_Command;
  typedef TAO_EC_Shutdown_Command<TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE> > Shutdown_Command;

  // = The TAO_EC_Proxy methods
  virtual void for_each (TAO_EC_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV);
  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV);
  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);

private:
  COLLECTION collection_;

  typedef TAO_EC_Busy_Lock_Adapter<TAO_EC_Delayed_Changes<PROXY,COLLECTION,ITERATOR,ACE_SYNCH_USE> > Busy_Lock;

  Busy_Lock lock_;

  ACE_SYNCH_MUTEX_T busy_lock_;

  ACE_SYNCH_CONDITION_T busy_cond_;

  CORBA::ULong busy_count_;

  CORBA::ULong write_delay_count_;

  CORBA::ULong busy_hwm_;
  CORBA::ULong max_write_delay_;
  // Control variables for the concurrency policies.

  ACE_Unbounded_Queue<ACE_Command_Base*> command_queue_;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Proxy_Collection.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "EC_Proxy_Collection.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("EC_Proxy_Collection.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_EC_PROXY_COLLECTION_H */

/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ProxyPushSupplier_Set
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Defines the interface for a collection of ProxyPushSuppliers that
//   can handle the concurrency requirements of the Event Channel.
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
// ============================================================================

#ifndef TAO_EC_PROXYPUSHSUPPLIER_SET_H
#define TAO_EC_PROXYPUSHSUPPLIER_SET_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"
#include "tao/corba.h"
#include "EC_Busy_Lock.h"

class TAO_EC_ProxyPushSupplier;
template<class Target,class Object> class TAO_EC_Connected_Command;
template<class Target,class Object> class TAO_EC_Disconnected_Command;

class TAO_ORBSVCS_Export TAO_EC_ProxyPushSupplier_Set
{
  // = TITLE
  //   ProxyPushSupplier_Set
  //
  // = DESCRIPTION
  //   Many components in the Event Channel need to keep a collection
  //   of ProxyPushSuppliers; this collections must be able to cope
  //   with several concurrency issues:
  //   + Some threads may need to iterate over the collection and
  //     invoke a method on each element.  Locking the collection
  //     while this is done is not feasible in all cases: under some
  //     configurations the same thread that is iterating over the
  //     collection may need to make changes.
  //   + A recursive lock does not solve the concurrency problems
  //     correctly because in the common case we don't want to stop
  //     other threads from doing the same iteration, and changes to
  //     the collection could be very uncommon wrt dispatching over
  //     the set of ProxyPushSuppliers.
  //   + Using a reader-write lock with upgrades does not solve the
  //     problem either: upgrading the lock can fail and still
  //     invalidates the iterators that the thread has.
  //   + Copying the collection to iterate over it is a good solution
  //     for non-real-time systems, but again the copying could be
  //     non-deterministic, and is expensive since most iterations
  //     will not result in changes.
  //
  //   This class encapsulates a protocol to solve this concurrency
  //   issues (and other minor ones).  The first solution is to delay
  //   changes by putting them on an "operation queue", the operations
  //   are stored as command objects in this queue and executed once
  //   the system is quiescent (i.e. no threads are iterating over the
  //   collection).
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
  // = VARIANTS
  //
  //   We identify several sources of variation:
  //
  //   + Immediate changes without delay (only locking). This is only
  //     useful in configurations where a separate thread dispatches
  //     the events, and thus, can only be used with real locks.
  //     The busy()/idle() methods use a read acquire/release, the
  //     connected()/disconnected() methods use a write
  //     acquire/release. We can either use RW or regular mutexes.
  //     IMPLEMENTATION: a derived class with strategized locking
  //     (ACE_Lock? or templates?).
  //
  //   + Copying data for iteration:
  //     Can use a regular mutex (as in the previous case), or even a
  //     null lock.
  //     @@ Must stablish if there is a simple way to encapsulate this
  //     in the Set class, as opposed to the clients of the class.
  //     @@ The implementation is essentially the same as above, but
  //     the clients must make the copy.
  //     IMPLEMENTATION: As above.
  //
  //   + Delayed operations:
  //     Only makes sense if the thread dispatching is the same thread
  //     where the upcalls run.
  //     Can require regular locks or null locks (for ST
  //     implementations); notice that this will require templates
  //     to parametrize the mutexes and condition variables.
  //     There are two main variations:
  //
  //     - An upcall can result in new dispatches: in this case we
  //       have to keep track of a the list of current threads using
  //       a Set, to avoid dead-locks.
  //       IMPLEMENTATION: the design is not complete, probably
  //       similar to the next one.
  //
  //     - Otherwise we just need to control the concurrency using the
  //       algorithm described below.
  //       IMPLEMENTATION: a derived parametric class (the arguments
  //       are the types of locks).
  //
  // = DELAYED OPERATIONS AND CONCURRENCY
  //
  //   The algorithm proposed so far is:
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
  // = MEMORY MANAGMENT
  //   It assumes ownership of the ProxyPushSuppliers added to the
  //   collection: simply by increasing their reference count.
  //
  // = LOCKING
  //   Locking is provided by derived classes.
  //
  // = TODO
  //
public:
  TAO_EC_ProxyPushSupplier_Set (void);
  // Constructor.

  virtual ~TAO_EC_ProxyPushSupplier_Set (void);
  // Destructor.

  typedef ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*> SupplierSet;
  typedef ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*> SupplierSetIterator;
  // The actual implementation of the collection, notice that only
  // iteration is exposed, methods to insert and remove objects are
  // hidden behind proper interfaces.

  SupplierSetIterator begin (void);
  SupplierSetIterator end (void);
  // Iterators over the set of ProxyPushSuppliers
  // Remember that all clients of this class must call busy() before
  // starting an iteration and call idle() once the iteration is
  // finished.  Otherwise the iterators may be invalidated by other
  // threads.
  // A helper object (the busy_lock) is provided to simplify this task
  // and make it exception safe.

  typedef TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set> Busy_Lock;
  Busy_Lock& busy_lock (void);
  // This object is an adapter to the busy/idle protocol.

  void busy_hwm (CORBA::ULong hwm);
  CORBA::ULong busy_hwm (void) const;
  void max_write_delay (CORBA::ULong hwm);
  CORBA::ULong max_write_delay (void) const;
  // This two attributes control the maximum number of concurrent
  // readers allowed in the set (busy_hwm) and the maximum number of
  // threads that can proceed after there is a modification posted.

  virtual int busy (void) = 0;
  virtual int idle (void) = 0;
  // The implementation of this methods is provided by derived
  // classes, that provide appropiate locking.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&) = 0;
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

protected:
  virtual void connected_i (TAO_EC_ProxyPushSupplier* supplier,
                            CORBA::Environment &env);
  // The implementation of connected(), without locking.
  // It does not increase the reference count on the supplier

  virtual void disconnected_i (TAO_EC_ProxyPushSupplier* supplier,
                               CORBA::Environment &env);
  // The implementation of disconnected(), without locking.
  // It decreases the reference count on the supplier if the operation
  // is successful.

  typedef TAO_EC_Connected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier> Connected_Command;
  typedef TAO_EC_Connected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier> Disconnected_Command;

  friend class TAO_EC_Connected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>;
  friend class TAO_EC_Disconnected_Command<TAO_EC_ProxyPushSupplier_Set,TAO_EC_ProxyPushSupplier>;
  // This two classes call the connected_i() and disconnected_i()
  // methods, that's ok because they do while this class is holding
  // its lock.

  virtual void execute_delayed_operations (void);
  // Derived classes that implement delayed disconnects and connects
  // must override this method.

protected:
  SupplierSet all_suppliers_;
  // The collection of ProxyPushSupplier objects.

  TAO_EC_Busy_Lock_Adapter<TAO_EC_ProxyPushSupplier_Set> busy_lock_;
  // The busy lock object

private:
  CORBA::ULong busy_hwm_;
  CORBA::ULong max_write_delay_;
  // Control variables for the concurrency policies.
};

#if defined (__ACE_INLINE__)
#include "EC_ProxyPushSupplier_Set.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PROXYPUSHSUPPLIER_SET_H */

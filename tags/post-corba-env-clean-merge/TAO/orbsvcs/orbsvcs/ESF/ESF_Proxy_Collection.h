/* -*- C++ -*- */
/**
 *  @file   ESF_Proxy_Collection.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PROXY_COLLECTION_H
#define TAO_ESF_PROXY_COLLECTION_H

#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class Target> class TAO_ESF_Worker;

/**
 * @class TAO_ESF_Proxy_Collection
 *
 * @brief ESF_Proxy_Collection
 *
 * Many components in an Event Service need to keep a collection
 * of proxies; these collections must be able to cope with several
 * concurrency issues:
 * + Some threads may need to iterate over the collection and
 *   invoke a method on each element.  Locking the collection
 *   while this is done is not feasible in all cases: under some
 *   configurations the same thread that is iterating over the
 *   collection may need to make changes to the set.
 * + A recursive lock does not solve the concurrency problems
 *   because recursive changes to the collection still invalidate
 *   the iterators.
 *
 * There are several solutions to this problem (see the VARIANTS)
 * section, and there is no single one that works bests in all
 * cases.  As usual, we wish the strategize the protocol used to
 * serialize iterations and changes to the collection.  This class
 * encapsulates that protocol.
 *
 * The usual form of the Iterator pattern does not work well in
 * this case: in several variants the synchronization protocol and
 * the iteration loop must collaborate to work efficiently.
 * Exposing an external iterator would require that every other
 * component in the system can support all the synchronization
 * protocols.   It is possible to hide some of that complexity
 * using heavy weight iterators, but their use is ackward,
 * specially since the Koening-style iterators have become more
 * popular.
 *
 * Regular member functions are used to insert, remove and update
 * members of the collection and to shutdown (i.e. perform final
 * cleanup operations).
 *
 * The class must also collaborate with other components of the
 * EC to efficiently and safely perform memory managment of the
 * members in the collection.
 *
 * The PROXY object must be reference counted with the following
 * operations:
 *
 * + _incr_refcnt() - increment the reference count.
 * + _decr_refcnt() - decrement the reference count.
 *
 * = VARIANTS
 *
 * We identify several sources of variation:
 *
 * + Immediate_Changes: in this variant the iteration in performed
 *   while holding some kind of synchronization primitive, such as a
 *   thread mutex, a recursive mutex, a RW lock, etc.
 *   This is only useful in configurations where a separate thread
 *   dispatches the events, and thus, can only be used with real
 *   locks.
 * + Copy_On_Read: before performing the iteration the collection
 *   is duplicated into a temporary array.  Thus no locks are held
 *   during the iteration.  This is a very expensive approach, but
 *   useful in many cases.
 *   The kind of lock is also strategized in this case.
 * + Copy_On_Write: this is very similar to the previous approach,
 *   but the collection is only duplicated when a change is required
 *   while some thread is performing an iteration.  The iteration
 *   continues over the original copy, while the changes are
 *   performed in the duplicate.  The new copy of the collection is
 *   used for any subsequent operations, the original is discarded
 *   when the last thread using it completes its work.
 *   This approach optimizes for the case where no changes are
 *   is duplicated into a temporary array.  Thus no locks are held
 *   during the iteration.  This is a very expensive approach, but
 *   useful in many cases.
 *   The kind of lock is also strategized in this case.
 * + Delayed_Changes: before starting the iteration a counter is
 *   incremented, this counter is used to keep track of the number
 *   of threads concurrently using the collection.
 *   If a thread wants to perform a change to the collection it must
 *   first verify that there are no threads iterating over it.  If
 *   there are any threads then the thread queues the modification
 *   for later execution, using the Command pattern.
 *   The kind of lock is strategized, as this approach is used in
 *   single threaded configurations.
 *   There are two main variations:
 *   - An upcall can result in new dispatches: in this case we
 *     have to keep track of a the list of current threads using
 *     a Set, to avoid dead-locks. The design is not complete,
 *     probably similar to the next one.
 *   - Otherwise we just need to control the concurrency using the
 *     algorithm described below.
 *
 * It assumes ownership of the proxies added to the collection,
 * it increases the reference count.
 *
 * Locking is provided by derived classes.
 */
template<class PROXY>
class TAO_ESF_Proxy_Collection
{
public:
  /// destructor
  virtual ~TAO_ESF_Proxy_Collection (void);

  /**
   * Iterate over the collection and invoke worker->work() for each
   * member of the collection.
   * This encapsulates
   */
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker
                         TAO_ENV_ARG_DECL) = 0;

  /// Insert a new element into the collection.  The collection assumes
  /// ownership of the element.
  virtual void connected (PROXY *proxy
                          TAO_ENV_ARG_DECL) = 0;

  /**
   * Insert an element into the collection.  No errors can be raised
   * if the element is already present.
   * The collection assumes ownership, i.e. must invoke
   * <proxy->_decr_refcnt()> if the element is already present in the
   * collection.
   */
  virtual void reconnected (PROXY *proxy
                            TAO_ENV_ARG_DECL) = 0;

  /// Remove an element from the collection.
  virtual void disconnected (PROXY *proxy
                             TAO_ENV_ARG_DECL) = 0;

  /// The EC is shutting down, must release all the elements.
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL) = 0;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Proxy_Collection.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Proxy_Collection.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Proxy_Collection.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PROXY_COLLECTION_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file   RTCosScheduling_PCP_Manager.h
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 *  @author based upon work by Greg Cooper
 *  @author University of Rhode Island
 */
//=============================================================================

#ifndef PCP_MANAGER_H
#define PCP_MANAGER_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Shared_Memory_MM.h"
#include "ace/Map_T.h"
#include "ace/SString.h"
#include "tao/RTCORBA/RTCORBA.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {


#if !defined (LOCK_ARRAY_SIZE)
  #define LOCK_ARRAY_SIZE 1024
#endif /* LOCK_ARRAY_SIZE */

#ifndef COS_SCHEDULING_CONTAINERS
#define COS_SCHEDULING_CONTAINERS
  /*
   * ResourceCeilingMap
   *
   * This typedef is used in the RTCORBA 1.0 Scheduling Service to
   * map names of resources on a server with priority ceilings.
   */
  typedef ACE_CString COS_SCHEDULING_RESOURCE_KEY;
  typedef int COS_SCHEDULING_RESOURCE_VALUE;

  typedef ACE_Map_Manager_Adapter<
    COS_SCHEDULING_RESOURCE_KEY,
    COS_SCHEDULING_RESOURCE_VALUE,
    ACE_Noop_Key_Generator<COS_SCHEDULING_RESOURCE_KEY> > CosSchedulingResourceMap;
#endif /* COS_SCHEDULING_CONTAINERS */


/**
* @class CosSchedulingLockList
*
* @brief    This program provides an abstract mechanism
* for the PCP_Manager class to store its lists of locks in
* shared memory.  It provides an efficient allocation and
* retrieval system and uses a pseudo-linked-list based on
* offsets (rather than absolute pointers) which allows it
* to work regardless of where it is mapped in a process's
* address space.  This means it can be used in a shared
* memory segment.
*
*/
struct CosSchedulingLockNode
{
  /// unique ID of the thread owning the lock
  int threadID_;

  /// the lock's priority ceiling
  int priority_ceiling_;

  /// the thread's original global priority
  int priority_;

  /// the thread's elevated priority
  int elevated_priority_;

  /// offset to the next lock in the list
  int next_offset_;

  /// Condition Variable to wait on Mutex
  ACE_SYNCH_CONDITION *condition_;

  /**
   * Translates the offset to the next lock
   * to a pointer and returns it
   */
  struct CosSchedulingLockNode *next();

  /**
   * Translates the pointer into an offset and
   * stores it in the structure
   */
  void next(const struct CosSchedulingLockNode *Next);

  /**
   * Copies the relevant fields while preserving those which should not be
   *modified
   */
  const CosSchedulingLockNode& operator=(const CosSchedulingLockNode& L);

};

class CosSchedulingLockList
{
  public:
  /**
   * Creates a CosSchedulingLockList structure using the storage in lock_array:
   * if Init is 1, the array is initialized to indicate that
   * none of the nodes are in use
   *
   * @param lock_array The shared memory space for the CosSchedulingLockList
   * @param size The size of the lock list
   * @param mutex the mutex that guards the CosSchedulingLockList
   */
  CosSchedulingLockList(CosSchedulingLockNode *lock_array,
                        const int size,
                        TAO_SYNCH_MUTEX *mutex);

  /**
   * Calls ACE_Thread::remove() on all conditions in the list;
   * should only be called when SchedInit terminates
   * (may not be necessary then)
   *
   * @param size the number of locks to destroy, starting at head
   */
  void destroy(const int size);

  /**
   * Adds L to the granted list, if space is available
   * (returns success); the list of granted locks is kept sorted
   * by priority-ceiling so that the highest ceiling can
   * be found quickly at the head
   *
   * @param L the CosSchedulingLockNode to add to the granted list.
   */
  int grant_lock(const CosSchedulingLockNode& L);

  /**
   * Adds L to the pending list, if space is available
   * (returns success); the list of pending locks is kept
   * sorted by priority so that the highest priority thread
   * awaiting a lock will be at the head; returns pointer to
   * condition variable upon success, NULL otherwise
   *
   * @param L The lock to add to the pending list.
   * @param mutex The mutex that guards the locks.
   */
  int defer_lock(const CosSchedulingLockNode& L,
    TAO_SYNCH_MUTEX &mutex);

  /**
   * Removes a node from the granted lock list whose threadID_
   * matches that of L, replacing L with the removed lock
   *
   * @param L released lock
   */
  int release_lock(CosSchedulingLockNode& L);

  /**
   * Removes the first node from the pending lock list,
   * replacing L with the removed lock
   *
   * @param L  Reference to the lock that is removed from pending
   */
  int remove_deferred_lock(CosSchedulingLockNode& L);

  /**
   * Returns a pointer to the node containing the highest ceiling (the
   * first node in the list of held locks)
   */
  CosSchedulingLockNode *highest_ceiling();


  /**
   * Returns a pointer to the node with the highest priority
   * (from the first node in the list of pending locks
   */
  CosSchedulingLockNode *highest_priority();

  private:

  /// A list of free locks
  CosSchedulingLockNode *free_;

  /// A list of Free locks
  CosSchedulingLockNode *granted_;

  /// A list of pending locks awaiting to be locked
  CosSchedulingLockNode *pending_;

};


/**
* @class PCP_Manager
*
* @brief PCP_Manager handles Priority Ceiling Control Protocol for the
* RTCORBA 1.0 ServerScheduler
* Each thread needs a PCP_Manager object: these are created by the
* PCP_Manager_Factory object, of which only one is needed per process
*
*/
class PCP_Manager
{
public:

  /**
   * Initializes a PCP_Manager object with the given lists, mutex,
   * condition variable, and priority mapper.
   *
   * @param locks A list of the locks to use in the PCP_Manager.
   * @param mutex The mutex to guard the locks.
   */
  PCP_Manager(CosSchedulingLockList *locks,
              TAO_SYNCH_MUTEX *mutex,
              const RTCORBA::Current_var current);

  /**
   * Acquires a lock on a shared resource using the
   * priority ceiling protocol
   *
   * @param PriorityCeiling The priority ceiling of the lock
   * @param priority The priority to lock at.
   */
  void lock(const int PriorityCeiling, const int Priority);

  /**
   * Releases a lock previously granted with lock()
   */
  void release_lock();

  /**
   * Returns the mThreadID data member
   */
  int threadID();

  private:
  int                   threadID_;  /// ID of thread owning this object
  CosSchedulingLockList *locks_;     /// combined list of locks
  TAO_SYNCH_MUTEX       *mutex_;     /// Mutex to guard lock list
  RTCORBA::Current_var  current_; /// reference to set local priority

};

/**
* @class PCP_Manager_Factory
*
* @brief Creates PCP_Managers.  Each process needs only one of
* these objects: it can create a PCP_Manager object for each
* thread as need arises.
*/
class PCP_Manager_Factory
{
public:

  /**
   * Initializes a PCP_Manager_Factory: each process should only
   * do this once.  It attaches a shared memory segment and retrieves
   * pointers to the granted and pending lock lists as well as
   * the mutex and condition variable.
   */
  PCP_Manager_Factory(const char *shared_file);

  ~PCP_Manager_Factory();

  /**
   * Creates a new PCP manager object using the lists and
   * synchronization objects found in shared memory.
   */
  PCP_Manager New_PCP_Manager(RTCORBA::Current_var current);

  private:
  CosSchedulingLockList *locks_;      /// lists of granted and pending locks
  TAO_SYNCH_MUTEX       mutex_;       /// The mutex for locking the lock list
  ACE_Shared_Memory_MM  mem_;         /// shared memory space
  char                  *shm_key_;    /// Key for shared memory
  CosSchedulingLockNode *lock_array_; /// The lock list

};

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/RTCosScheduling/RTCosScheduling_PCP_Manager.inl"
#endif /* __ACE_INLINE__ */


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* PCP_MANAGER_H */

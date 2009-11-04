/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RTCosScheduling_PCP_Manager.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "orbsvcs/RTCosScheduling/RTCosScheduling_PCP_Manager.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Thread.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/RTCosScheduling/RTCosScheduling_PCP_Manager.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO {

CosSchedulingLockList::CosSchedulingLockList(CosSchedulingLockNode *lock_array,
  const int size,
  TAO_SYNCH_MUTEX *mutex)
{
  try
    {
          /*
       *  The pointers to the beginnings of the lists must be globally visible,
       *  so I have chosen to use the first three locks in the array.
       *  lock_array[0].next() (== free_->next()   ) -> start of free list
       *  lock_array[1].next() (== granted_->next()) -> start of granted list
       *  lock_array[2].next() (== pending_->next()) -> start of pending list
       */

      /// start of the free list
      lock_array[0].next(&lock_array[3]);

      /// start with an empty granted list
      lock_array[1].next(0);

      /// start with an empty pending list
      lock_array[2].next(0);

      /// initialize the free list (link together the elements in the array)
      for (int i = 3; i < (size - 1); ++i)
        {
          lock_array[i].next(&lock_array[i + 1]);
           ACE_NEW_THROW_EX(lock_array[i].condition_,
                           ACE_SYNCH_CONDITION(*mutex),
                           CORBA::NO_MEMORY());
        }
      lock_array[size - 1].next(0); /// terminate the free list


      /// Update the positions
      this->free_ = &lock_array[0];
      this->granted_ = &lock_array[1];
      this->pending_ = &lock_array[2];
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG((LM_ERROR,
                 "Error in %s: Line %d - Could not generate a Locklist in shared memory\n",
                 __FILE__,
                 __LINE__));
      ex._tao_print_exception ("Exception: CosSchedulingLockList()");
    }
}

void
CosSchedulingLockList::destroy(const int size)
{
  for (int i = 3; i < size; ++i)
    {
      this->free_[i].condition_->remove();
      delete &free_[i].condition_;
    }
}

int
CosSchedulingLockList::grant_lock(const CosSchedulingLockNode& L)
{
  if (this->free_->next() == 0)
    {
      return 0; /// no free nodes left
    }
  /// take a node from the free list
  CosSchedulingLockNode *new_lock = this->free_->next();
  this->free_->next(this->free_->next()->next());
  *new_lock = L;

  if ((this->granted_->next() == 0)
    || (this->granted_->next()->priority_ceiling_ <= L.priority_ceiling_))
    {
      /// insert at the head of the list
      new_lock->next(this->granted_->next());
      this->granted_->next(new_lock);
    }
  else
    {
      /// find the proper location to insert
      /// the new lock (ordered by priority ceiling)
      CosSchedulingLockNode *current_lock = granted_->next();
      while ((current_lock->next() != 0)
        && (current_lock->next()->priority_ceiling_ > L.priority_ceiling_))
        {
         current_lock = current_lock->next();
        }
      new_lock->next(current_lock->next());
      current_lock->next(new_lock);
    }

  return 1;
}

int
CosSchedulingLockList::defer_lock(const CosSchedulingLockNode& L,
  TAO_SYNCH_MUTEX & mutex)
{
  if (this->free_->next() == 0)
    {
      return 0; /// no free nodes left
    }
  CosSchedulingLockNode *new_lock = free_->next();
  this->free_->next(free_->next()->next());
  *new_lock = L;

  if ((this->pending_->next() == 0)
    ||(this->pending_->next()->priority_ <= L.priority_))
    {
      /// insert at the head of the list
      new_lock->next(this->pending_->next());
      this->pending_->next(new_lock);
    }
  else
    {
      /// find the proper location to insert the new lock
      CosSchedulingLockNode *current_lock = pending_->next();
      while ((current_lock->next() != 0)
        && (current_lock->next()->priority_ceiling_ > L.priority_ceiling_))
        {
          current_lock = current_lock->next();
        }
      new_lock->next(current_lock->next());
      current_lock->next(new_lock);
    }


  if (new_lock->condition_)
    {
      new_lock->condition_->wait(mutex);
      return 1;
    }
  else
    {
    return 0;
    }
}


int
CosSchedulingLockList::release_lock(CosSchedulingLockNode& L)
{
  if (this->granted_->next() == 0)
    {
      return 0;  /// empty list of granted locks
    }

  if (this->granted_->next()->threadID_ == L.threadID_)
    {
      /// remove the lock at the head of the list and put it on the free list

      /// Set the Lock to the next one in the granted list
      L = *(this->granted_->next());

      /// (sets next offset from previous statement)
      L.next(this->granted_->next()->next());

      /// set the next granted's next one to be the next free one
      this->granted_->next()->next(this->free_->next());

      /// Set the next free one to be the next granted one
      this->free_->next(this->granted_->next());

      /// Set the next granted on to be the Lock's next one
      this->granted_->next(L.next());

      /// Set the Locks next on to NULL
      L.next(0);

      return 1;
    }

  /// find the lock to remove
  CosSchedulingLockNode *current_lock = granted_->next();
  while ((current_lock->next() != 0)
    && (current_lock->next()->threadID_ != L.threadID_))
    {
      current_lock = current_lock->next();
    }
  if (current_lock->next() != 0)
    {
      /// removes lock and prepends to the free list, as above
      L = *(current_lock->next());
      L.next(current_lock->next()->next());
      current_lock->next()->next(this->free_->next());
      this->free_->next(current_lock->next());
      current_lock->next(L.next());
      L.next(0);
      return 1;
    }

  return 0;
}

int
CosSchedulingLockList::remove_deferred_lock(CosSchedulingLockNode& L)
{
  if (this->pending_->next() == 0)
    {
      return 0; /// empty list of pending locks
    }

  /// take pending lock off the head of the list
  /// (highest priority request) and add to the free list
  L = *(this->pending_->next());
  CosSchedulingLockNode * fn = this->pending_->next();
  this->pending_->next(this->pending_->next()->next());
  fn->next(this->free_->next());
  this->free_->next(fn);

  return 1;
}

PCP_Manager::PCP_Manager(CosSchedulingLockList *locks,
  TAO_SYNCH_MUTEX *mutex,
  const RTCORBA::Current_var current)
: locks_(locks),
  mutex_(mutex),
  current_(current)
{
  /// Get the thread ID
  this->threadID_ = (ACE_OS::getpid() << 16) + int(ACE_Thread::self());
}

void
PCP_Manager::lock(const int priority_ceiling, const int priority)
{
  try
    {

      /// we do not want the thread to be pre-empted inside
      /// this critical section, so we
      /// will set its priority to the highest possible
      //  This is not completely necessary since the server should be running
      //  at RTCORBA::maxPriority
      this->current_->the_priority(RTCORBA::maxPriority);
      this->mutex_->acquire();

      /// create a structure to store my own lock request
      CosSchedulingLockNode MyLock;
      MyLock.threadID_ = this->threadID_;
      MyLock.priority_ceiling_ = MyLock.elevated_priority_ = priority_ceiling;
      MyLock.priority_ = priority;
      /// Start by assuming we don't have the lock then go look for it
      int HaveLock = 0;
      while (!HaveLock)
        {
          /// retrieve the highest priority ceiling from the list
          CosSchedulingLockNode *highest_lock = this->locks_->highest_ceiling();
          int prio_ceiling;
          /// check to see if are at the highest priority,
          /// if so set the priority ceiling
          if (highest_lock)
            {
               prio_ceiling = highest_lock->priority_ceiling_;
            }
          else
            {
              prio_ceiling = -1;
            }

          /// if I hold the highest ceiling or my priority is higher than the
          /// highest ceiling, just get the lock
          if ((highest_lock == 0) ||
              (highest_lock->threadID_ == this->threadID_) ||
              (highest_lock->priority_ceiling_ < priority))
            {
              /// Try and grant the lock, if it is not granted,
              /// then there are unfortunately no more lock nodes
              if (!this->locks_->grant_lock (MyLock))
                {
                  ACE_ERROR ((LM_ERROR,
                              "Fatal error--out of lock nodes!!!"));
                }
                /// Lock obtained from grant_lock, don't loop again
                HaveLock = 1;
            }
          else
            {
              /// There is another lock out there active, put this one
              /// in the list of pending locks
              if (this->locks_->defer_lock(MyLock, *this->mutex_))
                {
                  /// done waiting for it, remove it from the pending
                  /// lock list, will try again to grant on next loop
                  /// iteration
                  this->locks_->remove_deferred_lock (MyLock);
                }
              else
                {
                  ACE_ERROR((LM_ERROR,
                             "Error in deferring lock\n"));
                }
            }
        }

      /// remove mutex on the lock list
      this->mutex_->release();

      /// at this point we have the right to set the OS priority
      /// Do so at the priority ceiline.
      this->current_->the_priority(priority_ceiling);

    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG((LM_ERROR,
                 "Error in %s: Line %d - Could lock resource\n"
                 __FILE__,
                 __LINE__));
      ex._tao_print_exception ("Exception: PCP_Manager::lock");
    }
}

void PCP_Manager::release_lock()
{
  try
    {
      /// To prevent pre-emption in the critical section,
      /// which could lead to unbounded blocking
      this->current_->the_priority(RTCORBA::maxPriority);

      /// set up the mutex
      this->mutex_->acquire();

      /// remove the lock node from the list of locks
      CosSchedulingLockNode L;
      L.threadID_ = this->threadID_;
      this->locks_->release_lock(L);

      /// Done with the list, release the mutex
      this->mutex_->release();

      /// Let the highest priority lock signal the condition variable
      CosSchedulingLockNode *waiter = this->locks_->highest_priority();
      if (waiter)
        {
          waiter->condition_->signal();
        }

      /// We do not need to restore priority because we have already set this
      // thread to wait at RTCORBA::maxPriority at the start of this method
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG((LM_ERROR,
                 "Error in %s: Line %d - Could not release lock\n"
                 __FILE__,
                 __LINE__));
      ex._tao_print_exception ("Exception: PCP_Manager::release_lock");
    }
}


PCP_Manager_Factory::PCP_Manager_Factory(const char *shared_file)
{
  try
    {
#if !defined (ACE_LACKS_MMAP)
      char temp_file[MAXPATHLEN + 1];

      /// Get the temporary directory
      if (ACE::get_temp_dir (temp_file,
                             MAXPATHLEN - ACE_OS_String::strlen(shared_file))
          == -1)
        {
          ACE_DEBUG((LM_ERROR,
                     "Error in %s: Line %d - Shared File Name too long\n"
                     __FILE__,
                     __LINE__));
          ACE_OS::exit(1);
        }

      /// Add the filename to the end
      ACE_OS_String::strcat (temp_file, shared_file);

      /// Store in the global variable.
      this->shm_key_ = temp_file;

      if (ACE_OS::mkxxstemp (this->shm_key_) == 0
          || (ACE_OS::unlink (this->shm_key_) == -1
#ifndef ACE_HAS_WINCE
             && errno == EPERM
#endif /* ACE_HAS_WINCE */
              ))
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n",
                    this->shm_key_));

#else /* !ACE_LACKS_MMAP */
        ACE_DEBUG((LM_ERROR,
                   "Error in %s: Line %d - ACE_LACKS_MMAP - cannot create shared memory\n"
                   __FILE__,
                   __LINE__));
        ACE_OS::exit();
#endif /* !ACE_LACKS_MMAP */

      /// determine space requirements for the lock list
      u_int CosSchedulingLockList_space =
        LOCK_ARRAY_SIZE * sizeof (CosSchedulingLockNode);

      /// allocate space in shared memory for size of the lock list
      int result =
        this->mem_.open(this->shm_key_, CosSchedulingLockList_space);

      /// Make sure shared memory CosSchedulingLockList is ok, scheduling
      /// service cannot run without it.
      if (result == -1)
        {
          ACE_ERROR((LM_ERROR,
                   "Error in %s: Line %d - Error in creating the shared "
                   " memory segment to hold Lock information, "
                   "aborting ServerScheduler.\n"
                   __FILE__,
                   __LINE__));
          ACE_OS::exit(1);
        }


      /// Make the shared memory a place for a lock list
      this->lock_array_ = static_cast<CosSchedulingLockNode *> (this->mem_.malloc(CosSchedulingLockList_space));
      /// get the pointer to the list of locks and
      /// construct a lock list manager object
      if (this->lock_array_ == 0)
        {
          ACE_ERROR((LM_ERROR,
                   "Error in %s: Line %d - Error in creating "
                   "array to hold lock information "
                   "ServerScheduler not created\n"
                   __FILE__,
                   __LINE__));
          ACE_OS::exit(1);
        }
      else
        {
          /// construct the new lock list in shared memory
          ACE_NEW_THROW_EX(this->locks_,
                           CosSchedulingLockList(this->lock_array_,
                             LOCK_ARRAY_SIZE,
                             &this->mutex_),
                           CORBA::NO_MEMORY()
                          );
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR((LM_ERROR,
                 "Error in %s: Line %d - Error in creating "
                 "PCP_Manager_Factory to create new PCP_Managers\n"
                 __FILE__,
                 __LINE__));
      ex._tao_print_exception ("PCP_Manager_Factory::PCP_Manager_Factory\n");
    }
}

PCP_Manager_Factory::~PCP_Manager_Factory()
{
  /// throw out all the old Locks
  this->locks_->destroy(LOCK_ARRAY_SIZE);
  /// and delete the shared memory
  this->mem_.remove();
  delete this->locks_;
}

}

TAO_END_VERSIONED_NAMESPACE_DECL

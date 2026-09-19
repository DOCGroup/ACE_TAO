// -*- C++ -*-

//==========================================================================
/**
 *  @file    Synch_Traits.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//==========================================================================

#ifndef ACE_SYNCH_TRAITS_H
#define ACE_SYNCH_TRAITS_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"    /* Need to know threads, template settings */
#include "ace/Lock.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decl
class ACE_Null_Mutex;
class ACE_Null_Semaphore;
class ACE_Null_Mutex;
class ACE_Thread_Mutex;
class ACE_Process_Mutex;
class ACE_Recursive_Thread_Mutex;
class ACE_RW_Thread_Mutex;
class ACE_Thread_Semaphore;

template <class MUTEX> class ACE_Condition;
using ACE_Null_Condition = ACE_Condition<ACE_Null_Mutex>;
using ACE_Condition_Thread_Mutex = ACE_Condition<ACE_Thread_Mutex>;
using ACE_Condition_Recursive_Thread_Mutex = ACE_Condition<ACE_Recursive_Thread_Mutex>;

/**
 * @class ACE_NULL_SYNCH
 *
 * @brief Implement a do nothing Synchronization wrapper that
 *        typedefs the @c ACE_Condition and @c ACE_Mutex to the
 *        @c Null* versions.
 */
class ACE_Export ACE_NULL_SYNCH
{
public:
  using MUTEX = ACE_Null_Mutex;
  using NULL_MUTEX = ACE_Null_Mutex;
  using PROCESS_MUTEX = ACE_Null_Mutex;
  using RECURSIVE_MUTEX = ACE_Null_Mutex;
  using RW_MUTEX = ACE_Null_Mutex;
  using CONDITION = ACE_Null_Condition;
  using RECURSIVE_CONDITION = ACE_Null_Condition;
  using SEMAPHORE = ACE_Null_Semaphore;
  using NULL_SEMAPHORE = ACE_Null_Mutex;
};

#if defined (ACE_HAS_THREADS)

class ACE_Process_Mutex;

/**
 * @class ACE_MT_SYNCH
 *
 * @brief Implement a default thread safe synchronization wrapper that
 *        typedefs the @c ACE_Condition and @c ACE_Mutex to the
 * @c ACE_Condition and @c ACE_Mutex versions.
 *
 * @todo This should be a template, but SunC++ 4.0.1 complains about
 *       this.
 */
class ACE_Export ACE_MT_SYNCH
{
public:
  using MUTEX = ACE_Thread_Mutex;
  using NULL_MUTEX = ACE_Null_Mutex;
  using PROCESS_MUTEX = ACE_Process_Mutex;
  using RECURSIVE_MUTEX = ACE_Recursive_Thread_Mutex;
  using RW_MUTEX = ACE_RW_Thread_Mutex;
  using CONDITION = ACE_Condition_Thread_Mutex;
  using RECURSIVE_CONDITION = ACE_Condition_Recursive_Thread_Mutex;
  using SEMAPHORE = ACE_Thread_Semaphore;
  using NULL_SEMAPHORE = ACE_Null_Semaphore;
};

#endif /* ACE_HAS_THREADS */

#define ACE_SYNCH_MUTEX ACE_SYNCH::MUTEX
#define ACE_SYNCH_NULL_MUTEX ACE_SYNCH::NULL_MUTEX
#define ACE_SYNCH_RECURSIVE_MUTEX ACE_SYNCH::RECURSIVE_MUTEX
#define ACE_SYNCH_RW_MUTEX ACE_SYNCH::RW_MUTEX
#define ACE_SYNCH_CONDITION ACE_SYNCH::CONDITION
#define ACE_SYNCH_RECURSIVE_CONDITION ACE_SYNCH::RECURSIVE_CONDITION
#define ACE_SYNCH_NULL_SEMAPHORE ACE_SYNCH::NULL_SEMAPHORE
#define ACE_SYNCH_SEMAPHORE ACE_SYNCH::SEMAPHORE

// These are available on *all* platforms
#define ACE_SYNCH_PROCESS_SEMAPHORE ACE_Process_Semaphore
#define ACE_SYNCH_PROCESS_MUTEX  ACE_Process_Mutex

#if defined (ACE_HAS_THREADS)
#define ACE_SYNCH ACE_MT_SYNCH
#else /* ACE_HAS_THREADS */
#define ACE_SYNCH ACE_NULL_SYNCH
#endif /* ACE_HAS_THREADS */

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_SYNCH_TRAITS_H */

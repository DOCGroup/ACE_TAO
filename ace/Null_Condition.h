// -*- C++ -*-

//==========================================================================
/**
 *  @file    Null_Condition.h
 *
 *  $Id$
 *
 *   Moved from Synch.h.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_NULL_CONDITION_H
#define ACE_NULL_CONDITION_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "ace/os_include/os_errno.h"

class ACE_Time_Value;

/**
 * @class ACE_Null_Condition
 *
 * @brief Implement a do nothing <ACE_Condition> variable wrapper,
 * i.e., all methods are no ops.  This class is necessary since
 * some C++ compilers are *very* lame...
 */
class ACE_Export ACE_Null_Condition
{
public:
  ACE_Null_Condition (const ACE_Null_Mutex &m,
                      const ACE_TCHAR * = 0,
                      void * = 0)
    : mutex_ ((ACE_Null_Mutex &) m) {}

  ~ACE_Null_Condition (void) {}

  /// Returns 0.
  int remove (void) {return 0;}

  /// Returns -1 with <errno> == <ETIME>.
  int wait (const ACE_Time_Value * = 0) {errno = ETIME; return -1;}

  /// Returns -1 with <errno> == <ETIME>.
  int wait (ACE_Null_Mutex &m,
            const ACE_Time_Value * = 0) {errno = ETIME; return -1;}

  /// Returns 0.
  int signal (void) {return 0;}

  /// Returns 0.
  int broadcast (void) {return 0;}
  ACE_Null_Mutex &mutex (void) {return this->mutex_;};

  /// Dump the state of an object.
  void dump (void) const {}

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Null_Mutex &mutex_; // Reference to mutex lock.

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Condition &);
  ACE_Null_Condition (const ACE_Null_Condition &);
};

#include /**/ "ace/post.h"
#endif /* ACE_NULL_CONDITION_H */

// -*- C++ -*-

//==========================================================================
/**
 *  @file    Null_Mutex.h
 *
 *  $Id$
 *
 *   Moved from Synch.h.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_NULL_MUTEX_H
#define ACE_NULL_MUTEX_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_errno.h"
#include "ace/Global_Macros.h"

class ACE_Time_Value;

/**
 * @class ACE_Null_Mutex
 *
 * @brief Implement a do nothing <ACE_Mutex>, i.e., all the methods are
 * no ops.
 */
class ACE_Export ACE_Null_Mutex
{
public:
  ACE_Null_Mutex (const ACE_TCHAR * = 0)
    : lock_ (0) {}
  ~ACE_Null_Mutex (void) {}
  /// Return 0.
  int remove (void) {return 0;}

  /// Return 0.
  int acquire (void) {return 0;}

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value &timeout) {errno = ETIME; return -1;}

  /// Return -1 with <errno> == <ETIME>.
  int acquire (ACE_Time_Value *timeout) {errno = ETIME; return -1;}

  /// Return 0.
  int tryacquire (void) {return 0;}

  /// Return 0.
  int release (void) {return 0;}

  /// Return 0.
  int acquire_write (void) {return 0;}

  /// Return 0.
  int tryacquire_write (void) {return 0;}

  /// Return 0.
  int tryacquire_write_upgrade (void) {return 0;}

  /// Return 0.
  int acquire_read (void) {return 0;}

  /// Return 0.
  int tryacquire_read (void) {return 0;}

  /// Dump the state of an object.
  void dump (void) const {}

  /// Declare the dynamic allocation hooks.
  //ACE_ALLOC_HOOK_DECLARE;

  int lock_; // A dummy lock.
};

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
/**
 * @class ACE_Null_Mutex_Guard
 *
 * @brief This data structure is meant to be used within a method or
 * function...  It performs automatic aquisition and release of
 * an ACE_Null_Mutex.
 *
 * This class is obsolete and should be replaced by
 * ACE_Guard<ACE_Null_Mutex>.
 */
class ACE_Export ACE_Null_Mutex_Guard
{
public:
  ACE_Null_Mutex_Guard (ACE_Null_Mutex &) {}
  ~ACE_Null_Mutex_Guard (void) {}
  int remove (void) {return 0;}
  int locked (void) {return 1;}
  int acquire (void) {return 0;}
  int tryacquire (void) {return 0;}
  int release (void) {return 0:}
  void dump (void) const {}

private:
  // = Prevent assignment and initialization.
  void operator= (const ACE_Null_Mutex_Guard &);
  ACE_Null_Mutex_Guard (const ACE_Null_Mutex_Guard &);
};
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */

template <class ACE_LOCK>
class ACE_Guard;

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Guard<ACE_Null_Mutex>
 *
 * @brief Template specialization of <ACE_Guard> for the
 * <ACE_Null_Mutex>.
 *
 * This specialization is useful since it helps to speedup
 * performance of the "Null_Mutex" considerably.
 */
class ACE_Export ACE_Guard<ACE_Null_Mutex>
{
public:
  // = Initialization and termination methods.
  ACE_Guard (ACE_Null_Mutex &) {}
  ACE_Guard (ACE_Null_Mutex &, int) {}
  ACE_Guard (ACE_Null_Mutex &, int, int) {}
#if defined (ACE_WIN32)
  ~ACE_Guard (void) {}
#endif /* ACE_WIN32 */

  int acquire (void) { return 0; }
  int tryacquire (void) { return 0; }
  int release (void) { return 0; }
  void disown (void) {}
  int locked (void) { return 1; }
  int remove (void) { return 0; }
  void dump (void) const {}

private:
  // = Prevent assignment and initialization.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Guard<ACE_Null_Mutex> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Guard (const ACE_Guard<ACE_Null_Mutex> &))
};

template <class ACE_LOCK>
class ACE_Write_Guard;

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Write_Guard<ACE_Null_Mutex>
 *
 */
class ACE_Export ACE_Write_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
public:
  ACE_Write_Guard (ACE_Null_Mutex &m)
    : ACE_Guard<ACE_Null_Mutex> (m) {}
  ACE_Write_Guard (ACE_Null_Mutex &m, int blocked)
    : ACE_Guard<ACE_Null_Mutex> (m, blocked) {}

  int acquire_write (void) { return 0; }
  int acquire (void) { return 0; }
  int tryacquire_write (void) { return 0; }
  int tryacquire (void) { return 0; }
  void dump (void) const {}
};

template <class ACE_LOCK>
class ACE_Read_Guard;

ACE_TEMPLATE_SPECIALIZATION
/**
 * @class ACE_Read_Guard<ACE_Null_Mutex>
 *
 */
class ACE_Export ACE_Read_Guard<ACE_Null_Mutex> : public ACE_Guard<ACE_Null_Mutex>
{
public:
  ACE_Read_Guard (ACE_Null_Mutex &m)
    : ACE_Guard<ACE_Null_Mutex> (m) {}
  ACE_Read_Guard (ACE_Null_Mutex &m, int blocked)
    : ACE_Guard<ACE_Null_Mutex> (m, blocked) {}

  int acquire_read (void) { return 0; }
  int acquire (void) { return 0; }
  int tryacquire_read (void) { return 0; }
  int tryacquire (void) { return 0; }
  void dump (void) const {}
};

#include /**/ "ace/post.h"
#endif /* ACE_NULL_MUTEX_H */

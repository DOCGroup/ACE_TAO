/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    TSS_Test_Errno.h
 *
 *  $Id$
 *
 *  This file contains the definition of Errno.  Some compilers need
 *  it in a .h file for template instantiation (such as AIX C Set
 *  ++).
 *
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#include "ace/Guard_T.h"
#include "ace/Thread_Mutex.h"

/**
 * @class Errno
 *
 * @brief Define a simple Errno abstraction
 *
 * This class gets its own header file to work around AIX C++
 * compiler "features" related to template instantiation...  It is
 * only used by TSS_Test.cpp.
 */
class Errno
{
public:
  Errno()
  {
    ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_Mon, *Errno::lock_));
    created_ += 1;
  }
  ~Errno()
  {
    ACE_MT (ACE_GUARD (ACE_Thread_Mutex, ace_Mon, *Errno::lock_));
    deleted_ += 1;
  }

  int error (void) { return this->errno_; }
  void error (int i) { this->errno_ = i; }

  int line (void) { return this->lineno_; }
  void line (int l) { this->lineno_ = l; }

  // Errno::flags_ is a static variable, so we've got to protect it
  // with a mutex since it isn't kept in thread-specific storage.
  int flags (void)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_Mon, *Errno::lock_, -1));

    return Errno::flags_;
  }
  int flags (int f)
  {
    ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, *Errno::lock_, -1));

    Errno::flags_ = f;
    return 0;
  }

  static int created (void)
  {
    return created_;
  }

  static int deleted (void)
  {
    return deleted_;
  }

#if defined (ACE_HAS_THREADS)
  static
  ACE_Thread_Mutex *
  allocate_lock (void)
  {
    ACE_NEW_RETURN (Errno::lock_, ACE_Thread_Mutex, 0);
    return Errno::lock_;
  }

  static
  void
  deallocate_lock () { delete lock_; lock_ = 0; }
#endif /* ACE_HAS_THREADS */

private:
  // = errno_ and lineno_ will be thread-specific data so they don't
  // need a lock.
  int errno_;
  int lineno_;

  static int flags_;
  static int created_;
  static int deleted_;
#if defined (ACE_HAS_THREADS)
  // flags_ needs a lock.
  static ACE_Thread_Mutex *lock_;
#endif /* ACE_HAS_THREADS */
};

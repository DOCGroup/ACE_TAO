/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Singleton.h
//
// = DESCRIPTION
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu), Douglas C. Schmidt, Chris
//    Lahey, Rich Christy, and David Levine.
//
// ============================================================================

#ifndef ACE_SINGLETON_H
#define ACE_SINGLETON_H

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class TYPE, class ACE_LOCK>
class ACE_Singleton : public ACE_Cleanup
{
  // = TITLE
  //     A Singleton Adapter uses the Adapter pattern to turn ordinary
  //     classes into Singletons optimized with the Double-Checked
  //     Locking optimization pattern.
  //
  // = DESCRIPTION
  //     This implementation is a slight variation on the GoF
  //     Singleton pattern.  In particular, a single
  //     <ACE_Singleton<TYPE, ACE_LOCK> > instance is allocated here,
  //     not a <TYPE> instance.  The reason for this is to allow
  //     registration with the <ACE_Object_Manager>, so that the
  //     Singleton can be cleaned up when the process exits.  For this
  //     scheme to work, a (static) <cleanup> function must be
  //     provided.  <ACE_Singleton> provides one so that TYPE doesn't
  //     need to.
  //
  //     If you want to make sure that only the singleton instance of
  //     <T> is created, and that users cannot create their own
  //     instances of <T>, do the following to class <T>:
  //
  //     (a) Make the constructor of <T> private (or protected)
  //     (b) Make Singleton a friend of <T>
  //
  //     Here is an example:
  //
  //     class foo
  //     {
  //       friend class ACE_Singleton<foo, ACE_Null_Mutex>;
  //     private:
  //       foo () { cout << "foo constructed" << endl; }
  //       ~foo () { cout << "foo destroyed" << endl; }
  //     };
  //
  //     typedef ACE_Singleton<foo, ACE_Null_Mutex> FOO;
  //
  //     NOTE:  the best types to use for ACE_LOCK are
  //     ACE_Recursive_Thread_Mutex and ACE_Null_Mutex.
  //     ACE_Recursive_Thread_Mutex should be used in multi-threaded
  //     programs in which it is possible for more than one thread to
  //     access the <ACE_Singleton<TYPE, ACE_LOCK>> instance.
  //     ACE_Null_Mutex can be used otherwise.  The reason that these
  //     types of locks are best has to do with their allocation by
  //     the ACE_Object_Manager.  Single ACE_Recursive_Thread_Mutex
  //     and ACE_Null_Mutex instances are used for all ACE_Singleton
  //     instantiations.  However, other types of locks are allocated
  //     per ACE_Singleton instantiation.
  //
public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  virtual void cleanup (void *param = 0);
  // Cleanup method, used by <ace_cleanup_destroyer> to destroy the
  // <ACE_Singleton>.

  static void dump (void);
  // Dump the state of the object.

protected:
  ACE_Singleton (void);
  // Default constructor.

  TYPE instance_;
  // Contained instance.

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_Singleton<TYPE, ACE_LOCK> *singleton_;
  // Pointer to the Singleton (ACE_Cleanup) instance.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static ACE_Singleton<TYPE, ACE_LOCK> *&instance_i (void);
  // Get pointer to the Singleton instance.
};

template <class TYPE, class ACE_LOCK>
class ACE_TSS_Singleton : public ACE_Cleanup
{
  // = TITLE
  //     This class uses the Adapter pattern to turn ordinary classes
  //     into Thread-specific Singletons optimized with the
  //     Double-Checked Locking optimization pattern.
  //
  // = DESCRIPTION
  //     This implementation is another variation on the GoF Singleton
  //     pattern.  In this case, a single <ACE_TSS_Singleton<TYPE,
  //     LOCK> > instance is allocated here, not a <TYPE> instance.
  //     Each call to the <instance> static method returns a Singleton
  //     whose pointer resides in thread-specific storage.  As with
  //     <ACE_Singleton>, we use the <ACE_Object_Manager> so that the
  //     Singleton can be cleaned up when the process exits.  For this
  //     scheme to work, a (static) <cleanup> function must be
  //     provided.  <ACE_Singleton> provides one so that TYPE doesn't
  //     need to.
public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  virtual void cleanup (void *param = 0);
  // Cleanup method, used by <ace_cleanup_destroyer> to destroy the
  // singleton.

  static void dump (void);
  // Dump the state of the object.

protected:
  ACE_TSS_Singleton (void);
  // Default constructor.

  ACE_TSS_TYPE (TYPE) instance_;
  // Contained instance.

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_TSS_Singleton<TYPE, ACE_LOCK> *singleton_;
  // Pointer to the Singleton (ACE_Cleanup) instance.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static ACE_TSS_Singleton<TYPE, ACE_LOCK> *&instance_i (void);
  // Get pointer to the TSS Singleton instance.
};

#if defined (__ACE_INLINE__)
#include "ace/Singleton.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Singleton.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Singleton.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SINGLETON_H */

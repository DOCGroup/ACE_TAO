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
//    Tim Harrison (harrison@cs.wustl.edu), Douglas C. Schmidt, Chris Lahey, and Rich Christy
// 
// ============================================================================

#if !defined (ACE_SINGLETON_H)
#define ACE_SINGLETON_H

#include "ace/Synch.h"

template <class TYPE, class LOCK>
class ACE_Singleton
  // = TITLE
  //     A Singleton Adapter.
  //   
  // = DESCRIPTION
  //     Uses the Adapter pattern to turn ordinary classes into
  //     Singletons optimized with the Double-Check pattern.
{
public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  static TYPE *instance (TYPE*);
  // Set the Singleton instance.

  static void dump (void);
  // Dump the state of the object.

protected:
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static TYPE *instance_;
  // Pointer to the Singleton instance.

  static LOCK ace_singleton_lock_;
  // Lock the creation of the singleton.  
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static TYPE *&instance_i (void);
  // Get pointer to the Singleton instance

  static LOCK &singleton_lock_i (void);
  // Get reference to Singleton lock;
};

template <class TYPE, class LOCK>
class ACE_TSS_Singleton
  // = TITLE
  //     A Thread Specific Storage Singleton Adapter.
  //   
  // = DESCRIPTION
  //     Uses the Adapter pattern to turn ordinary classes into
  //     Singletons optimized with the Double-Check pattern.
{
public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  static void dump (void);
  // Dump the state of the object.

protected:
#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_TSS<TYPE> *instance_;
  // Pointer to the Singleton instance.

  static LOCK ace_singleton_lock_;
  // Lock the creation of the singleton.                                        
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */
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

// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    TAO_Singleton.h
//
// = DESCRIPTION
//     Header file for the TAO-specific Singleton implementation.
//     Based entirely on tao/TAO_Singleton.*.
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_SINGLETON_H
#define TAO_SINGLETON_H

#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


template <class TYPE, class ACE_LOCK>
class TAO_Singleton : public ACE_Cleanup
{
  // = TITLE
  //     TAO-specific Singleton class
  //
  // = DESCRIPTION
  //     TAO_Singletons are used by TAO to register TAO-specific
  //     singleton instances with the TAO_Object_Manager.  This
  //     ensures that TAO singletons are isolated from ACE's
  //     Object_Manager, thus allowing TAO to be safely dynamically
  //     unloaded.

public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  virtual void cleanup (void *param = 0);
  // Cleanup method, used by <ace_cleanup_destroyer> to destroy the
  // singleton.

  static void dump (void);
  // Dump the state of the object.

protected:
  TAO_Singleton (void);
  // Default constructor.

  TYPE instance_;
  // Contained instance.

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static TAO_Singleton<TYPE, ACE_LOCK> *singleton_;
  // Pointer to the Singleton (ACE_Cleanup) instance.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static TAO_Singleton<TYPE, ACE_LOCK> *&instance_i (void);
  // Get pointer to the TAO Singleton instance.
};

template <class TYPE, class ACE_LOCK>
class TAO_TSS_Singleton : public ACE_Cleanup
{
  // = TITLE
  //     TAO-specific Singleton class
  //
  // = DESCRIPTION
  //     TAO_Singletons are used by TAO to register TAO-specific
  //     singleton instances with the TAO_Object_Manager.  This
  //     ensures that TAO singletons are isolated from ACE's
  //     Object_Manager, thus allowing TAO to be safely dynamically
  //     unloaded.

public:
  static TYPE *instance (void);
  // Global access point to the Singleton.

  virtual void cleanup (void *param = 0);
  // Cleanup method, used by <ace_cleanup_destroyer> to destroy the
  // singleton.

  static void dump (void);
  // Dump the state of the object.

protected:
  TAO_TSS_Singleton (void);
  // Default constructor.

  ACE_TSS_TYPE (TYPE) instance_;
  // Contained instance.

#if !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static TAO_TSS_Singleton<TYPE, ACE_LOCK> *singleton_;
  // Pointer to the Singleton (ACE_Cleanup) instance.
#endif /* ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES */

  static TAO_TSS_Singleton<TYPE, ACE_LOCK> *&instance_i (void);
  // Get pointer to the TAO TSS Singleton instance.
};

#if defined (__ACE_INLINE__)
#include "tao/TAO_Singleton.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/TAO_Singleton.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("TAO_Singleton.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include "ace/post.h"

#endif  /* TAO_SINGLETON_H */

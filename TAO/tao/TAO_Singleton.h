// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_Singleton.h
 *
 *   Header file for the TAO-specific Singleton implementation.
 *   Based entirely on ace/Singleton.*.
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SINGLETON_H
#define TAO_SINGLETON_H

#include /**/ "ace/pre.h"

#include "ace/TSS_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/Cleanup.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Singleton
 *
 * @brief TAO-specific Singleton class
 *
 * TAO_Singletons are used by TAO to register TAO-specific
 * singleton instances with the TAO_Object_Manager.  This
 * ensures that TAO singletons are isolated from ACE's
 * Object_Manager, thus allowing TAO to be safely dynamically
 * unloaded.
 */
template <class TYPE, class ACE_LOCK>
class TAO_Singleton : public ACE_Cleanup
{

public:
  /// Global access point to the Singleton.
  static TYPE *instance ();

  /// Cleanup method, used by @c ace_cleanup_destroyer to destroy the
  /// singleton.
  virtual void cleanup (void *param = 0);

  /// Dump the state of the object.
  static void dump ();

protected:
  /// Default constructor.
  TAO_Singleton ();

  /// Contained instance.
  TYPE instance_;

  /// Pointer to the Singleton (ACE_Cleanup) instance.
  static TAO_Singleton<TYPE, ACE_LOCK> *singleton_;

  /// Get pointer to the TAO Singleton instance.
  static TAO_Singleton<TYPE, ACE_LOCK> *&instance_i ();
};

/**
 * @class TAO_TSS_Singleton
 *
 * @brief TAO-specific Singleton class
 *
 * TAO_Singletons are used by TAO to register TAO-specific
 * singleton instances with the TAO_Object_Manager.  This
 * ensures that TAO singletons are isolated from ACE's
 * Object_Manager, thus allowing TAO to be safely dynamically
 * unloaded.
 */
template <class TYPE, class ACE_LOCK>
class TAO_TSS_Singleton : public ACE_Cleanup,
                          private ACE_Copy_Disabled
{
public:
  /// Global access point to the Singleton.
  static TYPE *instance ();

  /// Cleanup method, used by @c ace_cleanup_destroyer to destroy the
  /// singleton.
  virtual void cleanup (void *param = 0);

  /// Dump the state of the object.
  static void dump ();

protected:
  /// Default constructor.
  TAO_TSS_Singleton ();

  /// Contained instance.
  ACE_TSS_TYPE (TYPE) instance_;

  /// Pointer to the Singleton (ACE_Cleanup) instance.
  static TAO_TSS_Singleton<TYPE, ACE_LOCK> *singleton_;

  /// Get pointer to the TAO TSS Singleton instance.
  static TAO_TSS_Singleton<TYPE, ACE_LOCK> *&instance_i ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/TAO_Singleton.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/TAO_Singleton.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("TAO_Singleton.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif  /* TAO_SINGLETON_H */

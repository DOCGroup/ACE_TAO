// -*- C++ -*-

// ===================================================================
/**
 *  @file   Cleanup_Func_Registry.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_CLEANUP_FUNC_REGISTRY_H
#define TAO_CLEANUP_FUNC_REGISTRY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

#include "ace/Array_Base.h"
#include "ace/Copy_Disabled.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Cleanup_Func_Registry
 *
 * @brief
 * This is a helper class that is designed to perform cleanup on
 * thread-specific objects registered in the ORB Core TSS resources by
 * invoking the corresponding cleanup function on each object.  Hence,
 * there is a tight coupling between this class and the TAO ORB Core.
 */
class TAO_Export TAO_Cleanup_Func_Registry : private ACE_Copy_Disabled
{
  friend class TAO_ORB_Core;

public:

  /// Constructor.
  TAO_Cleanup_Func_Registry (void);

  /// Return the number of registered cleanup functions.
  size_t size (void) const;

protected:

  /// Register a cleanup function.  The number of slot the cleanup
  /// function is placed is in will match the one reserved for the
  /// corresponding thread specific object in the ORB Core TSS
  /// resources.  The @a slot_id is returned via the second reference
  /// argument.  This method returns 0 on failure, and -1 on failure.
  int register_cleanup_function (ACE_CLEANUP_FUNC func, size_t &slot_id);

  /// Invoke the corresponding cleanup function on each
  /// thread-specific object.
  void cleanup (ACE_Array_Base<void *> &ts_objects);

private:

  /// Array of registered cleanup functions.  The number of
  /// registered cleanup functions should be the same as the number
  /// of registered thread-specific objects in the ORB Core TSS
  /// resources.
  ACE_Array_Base<ACE_CLEANUP_FUNC> cleanup_funcs_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Cleanup_Func_Registry.inl"
#endif  /* __ACE_INLINE__ */


#include /**/ "ace/post.h"

#endif  /* TAO_CLEANUP_FUNC_REGISTRY_H */

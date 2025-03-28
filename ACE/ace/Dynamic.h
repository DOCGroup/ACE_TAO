// -*- C++ -*-

//==========================================================================
/**
 *  @file    Dynamic.h
 *
 *  @author Doug Schmidt
 *  @author Irfan Pyarali.
 */
//==========================================================================

#ifndef ACE_DYNAMIC_H
#define ACE_DYNAMIC_H
#include /**/ "ace/pre.h"

#include /**/ "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Dynamic
 *
 * @brief Checks to see if an object was dynamically allocated.
 *
 * This class holds the pointer in a thread-safe manner between
 * the call to operator new and the call to the constructor.
 */
class ACE_Export ACE_Dynamic
{
public:
  /// Constructor.
  ACE_Dynamic ();

  /// Destructor.
  ~ACE_Dynamic () = default;

  /**
   * Sets a flag that indicates that the object was dynamically
   * created. This method is usually called in operator new and then
   * checked and reset in the constructor.
   */
  void set ();

  /// @c true if we were allocated dynamically, else @c false.
  bool is_dynamic ();

  /// Resets state flag.
  void reset ();

  static ACE_Dynamic *instance ();

  ACE_ALLOC_HOOK_DECLARE;

private:
  /**
   * Flag that indicates that the object was dynamically created. This
   * method is usually called in operator new and then checked and
   * reset in the constructor.
   */
  bool is_dynamic_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Dynamic.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_DYNAMIC_H */

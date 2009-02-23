// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORB_Core_Auto_Ptr.h
 *
 *  $Id$
 *
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_ORB_CORE_AUTO_PTR_H
#define TAO_ORB_CORE_AUTO_PTR_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * @class TAO_ORB_Core_Auto_Ptr
 *
 * @brief Define a TAO_ORB_Core auto_ptr class.
 *
 * This class is used as an aid to make ORB initialization exception
 * safe.  It ensures that the ORB core is deallocated through its
 * reference counting mechanism if an exception is thrown.
 */
class TAO_Export TAO_ORB_Core_Auto_Ptr
{
public:

  /**
   * @name Initialization and termination methods
   */
  //@{
  explicit TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core *p = 0);
  TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core_Auto_Ptr &ap);
  TAO_ORB_Core_Auto_Ptr &operator= (TAO_ORB_Core_Auto_Ptr &rhs);
  ~TAO_ORB_Core_Auto_Ptr (void);
  //@}

  /**
   * @name Accessor methods.
   */
  //@{
  TAO_ORB_Core &operator *() const;
  TAO_ORB_Core *get (void) const;
  TAO_ORB_Core *release (void);
  void reset (TAO_ORB_Core *p = 0);
  TAO_ORB_Core *operator-> () const;
  //@}

protected:
  TAO_ORB_Core *p_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core_Auto_Ptr.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_CORE_AUTO_PTR_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file Refcounted_ObjectKey.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_REFCOUNTED_OBJECTKEY_H
#define TAO_REFCOUNTED_OBJECTKEY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Object_KeyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class Refcounted_ObjectKey
   *
   * @brief A wrapper class that ties together a refcount to an
   * ObjectKey.
   *
   * The refcount in this class is manipulated within the context of
   * the lock in the TAO::ObjectKey_Table. Manipulating the refcount
   * from anywhere else is strictly forbidden.
   */
  class TAO_Export Refcounted_ObjectKey
  {
  public:
    /// Constructor
    Refcounted_ObjectKey (const ObjectKey &ref);

    /// Accessor for the underlying ObjectKey.
    const ObjectKey &object_key (void) const;

  protected:
    friend class ObjectKey_Table;

    /// Protected destructor
    ~Refcounted_ObjectKey (void);

    /// Methods for incrementing refcount.
    void incr_refcount (void);

    /// Methods for decrementing refcount. Return the refcount, used by the
    /// ObjectKey table.
    CORBA::ULong decr_refcount (void);

  private:
    /// The object key
    ObjectKey object_key_;

    /// The refcount on the object key..
    CORBA::ULong ref_count_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Refcounted_ObjectKey.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /*TAO_REFCOUNTED_OBJECTKEY_H*/

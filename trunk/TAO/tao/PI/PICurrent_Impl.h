// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent_Impl.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_IMPL_H
#define TAO_PI_CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PI_includeC.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Array_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations.
class TAO_ORB_Core;

namespace TAO
{
  /**
   * @class PICurrent_Impl
   *
   * @brief Implementation of the PortableInterceptor::Current
   *        interface.
   *
   * This class implements both the "request scope current" and the
   * "thread scope current" objects as required by Portable
   * Interceptors.
   */
  class TAO_PI_Export PICurrent_Impl
  {
  public:
    /// Constructor.
    PICurrent_Impl (TAO_ORB_Core *orb_core= 0,
                    size_t tss_slot= 0,
                    PICurrent_Impl *pop= 0);
    /// Destructor.
    ~PICurrent_Impl (void);

    /// Retrieve information stored in the slot table at the given
    /// SlotId.
    CORBA::Any *get_slot (PortableInterceptor::SlotId identifier);

    /// Set information in the slot table at the given SlotId.
    void set_slot (PortableInterceptor::SlotId identifier,
                   const CORBA::Any & data);

    /// Logically/Lazy (shallow) copy the given object's slot table.
    void take_lazy_copy (PICurrent_Impl *p);

    /// Push a new PICurrent_Impl on stack
    void push (void);

    /// Pop old PICurrent_Impl from stack
    void pop (void);

  private:
    /// Force this object to convert from a logical (referenced)
    /// copy, to a physical (or deep, actual) copy.
    void convert_from_lazy_to_real_copy ();

    /// Set the callback PICurrent_Impl object that will be notified
    /// of this object's impending destruction or change.
    /// Set to 0 to clear. (NOTE Only handles a SINGLE object at
    /// at time, does NOT warn previous callback that this has
    /// been changed.)
    void set_callback_for_impending_change (PICurrent_Impl *p);

    /// Typedef for the underyling "slot table."
    typedef ACE_Array_Base<CORBA::Any> Table;

    /// Return a reference to the slot table currently associated
    /// with this PICurrent_Impl object.
    /**
     * @return Logically copied slot table if available, otherwise
     *         underlying slot table.
     */
    Table & current_slot_table ();

    /// Prevent copying through the copy constructor and the assignment
    /// operator.
    //@{
    PICurrent_Impl (const PICurrent_Impl &);
    void operator= (const PICurrent_Impl &);
    //@}

  private:
    /// Allow for stack of PICurrent_Impl as required.
    TAO_ORB_Core   *orb_core_;
    size_t         tss_slot_;
    PICurrent_Impl *pop_;
    PICurrent_Impl *push_;

    /// Array of CORBA::Anys that is the underlying "slot table."
    Table slot_table_;

    /// Access to logical copy from a PICurrent_Impl in another
    /// scope, i.e. either the request scope or the thread scope.
    PICurrent_Impl *lazy_copy_;

    /// PICurrent_Impl object that will be notified of this object's
    /// impending destruction or change to its slot_table_. This is
    /// the PICurrent_Impl that has access to our slot_table_ via its
    /// lazy_copy_ pointer. As necessary this allows that object's
    /// convert_from_lazy_to_real_copy() to be called.
    PICurrent_Impl *impending_change_callback_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PI/PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_IMPL_H */

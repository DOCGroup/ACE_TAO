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

#include "tao/PI/PICurrentC.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/Array_Base.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations.
class TAO_ORB_Core;

namespace TAO
{
  class PICurrent_Copy_Callback;
  class PICurrent_Impl;

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

    /// Typedef for the underyling "slot table."
    typedef ACE_Array_Base<CORBA::Any> Table;

    /// Constructor.
    PICurrent_Impl (void);

    /// Destructor.
    ~PICurrent_Impl (void);

    /// Retrieve information stored in the slot table at the given
    /// SlotId.
    CORBA::Any *get_slot (PortableInterceptor::SlotId identifier
                          ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::InvalidSlot));

    /// Set information in the slot table at the given SlotId.
    void set_slot (PortableInterceptor::SlotId identifier,
                   const CORBA::Any & data
                   ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::InvalidSlot));

    /// Set the PICurrent copy callback object responsible for deep
    /// copying the source PICurrent's slot table.
    void copy_callback (PICurrent_Copy_Callback *cb);

    /// Set the PICurrent destruction callback object that will be
    /// notified of this object's destruction.
    void destruction_callback (PICurrent_Impl *p);

    /// Execute the destruction callback object
    void execute_destruction_callback (Table *old_lc_slot_table);

    /// Return a reference to the underlying slot table.
    Table & slot_table (void);

    /// Return a reference to the slot table currently associated
    /// with this PICurrent_Impl object.
    /**
     * @return Logically copied slot table if available, otherwise
     *         underlying slot table.
     */
    Table & current_slot_table (void);

    /// Logically (shallow) copy the given slot table.
    // returns true if copied, false if it would be self-referencing.
    bool lc_slot_table (PICurrent_Impl *p);

    /// Return pointer to the logically copied slot table.
    /**
     * @return Zero if no logically copied slot table.  Non-zero
     *         otherwise.
     */
    Table *lc_slot_table (void) const;

  private:

    /// Prevent copying through the copy constructor and the assignment
    /// operator.
    //@{
    PICurrent_Impl (const PICurrent_Impl &);
    void operator= (const PICurrent_Impl &);
    //@}

  private:

    /// Array of CORBA::Anys that is the underlying "slot table."
    Table slot_table_;

    /// Table that was logically copied from a PICurrent in another
    /// scope, i.e. either the request scope or the thread scope.
    Table *lc_slot_table_;

    /// Callback object responsible for performing deep copies of a
    /// PICurrent's slot table. This is the PICurrent that has our slot_table_
    /// referred as lc_slot_table_. This copy is there to make sure that when
    /// we want to modify our table, that we can first copy our table to
    /// the PICurrent that refers to our table so that it has an unique copy
    /// of the data.
    PICurrent_Copy_Callback *copy_callback_;

    /// PICurrent_Impl object that will be notified of this object's
    /// destruction. This is the PICurrent that has our slot_table_ as
    /// lc_slot_table_.
    PICurrent_Impl *destruction_callback_;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PI/PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_IMPL_H */

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

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "ace/Array_Base.h"
#include "Any.h"
#include "PICurrentC.h"

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
  class TAO_Export PICurrent_Impl
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
    void lc_slot_table (PICurrent_Impl *p);

    /// Return pointer to the logically copied slot table.
    /**
     * @return Zero if no logically copied slot table.  Non-zero
     *         otherwise.
     */
    Table *lc_slot_table (void) const;

  private:

    /// Prevent copying through the copy constructor and the assignment
    //operator.
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
    /// PICurrent's slot table.
    PICurrent_Copy_Callback *copy_callback_;

    /// PICurrent_Impl object that will be notified of this object's
    /// destruction.
    PICurrent_Impl *destruction_callback_;

  };
}

#if defined (__ACE_INLINE__)
# include "PICurrent_Impl.inl"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_IMPL_H */

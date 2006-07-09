// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_H
#define TAO_PI_CURRENT_H

#include /**/ "ace/pre.h"

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PICurrentC.h"

#include "tao/LocalObject.h"
#include "ace/Array_Base.h"

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations.
class TAO_ORB_Core;

namespace TAO
{
  class PICurrent_Impl;

  /**
   * @class PICurrent
   *
   * @brief Implementation of the PortableInterceptor::Current
   * interface.
   *
   * PortableInterceptor::Current is useful for passing data between
   * request interceptors, in addition to passing data from an
   * interceptor to the calling thread.
   */
  class TAO_PI_Export PICurrent
    : public virtual PortableInterceptor::Current,
      public virtual TAO_Local_RefCounted_Object
  {
  public:

    /// Constructor.
    PICurrent (TAO_ORB_Core &orb_core);

    /**
     * @name PortableInterceptor::Current Methods
     *
     * These are methods exposed by the PortableInterceptor::Current
     * interface.
     */
    //@{
    /// Retrieve information stored in the slot table at the given
    /// SlotId.
    virtual CORBA::Any *get_slot (PortableInterceptor::SlotId id
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::InvalidSlot));

    /// Set information in the slot table at the given SlotId.
    virtual void set_slot (PortableInterceptor::SlotId identifier,
                           const CORBA::Any & data
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::InvalidSlot));
    //@}

    virtual CORBA::ORB_ptr _get_orb (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      );

    /// Number of slots allocated in the slot table.
    PortableInterceptor::SlotId slot_count (void) const;

    /// Retrieve the PICurrent implementation from TSS, i.e. the thread
    /// scope current (TSC).
    PICurrent_Impl * tsc (void);

    /// Verify the validity of the given SlotId.
    void check_validity (const PortableInterceptor::SlotId &identifier
                         ACE_ENV_ARG_DECL);

    /// Initialize the PICurrent object.
    int initialize (PortableInterceptor::SlotId sc ACE_ENV_ARG_DECL);

  protected:

    /// Destructor
    /**
     * Protected destructor to enforce the fact this class is reference
     * counted, and should not be destroyed using delete() by anything
     * other than the reference counting mechanism.
     */
    virtual ~PICurrent (void);

  private:

    /// Prevent copying through the copy constructor and the assignment
    /// operator.
    //@{
    PICurrent (const PICurrent &);
    void operator= (const PICurrent &);
    //@}

  private:
    /// Reference to the orb core.
    TAO_ORB_Core &orb_core_;

    /// TSS slot assigned to this object.
    size_t tss_slot_;

    /// The number of allocated slots.
    PortableInterceptor::SlotId slot_count_;

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PI/PICurrent.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_H */

// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_H
#define TAO_PI_CURRENT_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "PI_includeC.h"

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
      public virtual ::CORBA::LocalObject
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
    virtual CORBA::Any *get_slot (PortableInterceptor::SlotId id);

    /// Set information in the slot table at the given SlotId.
    virtual void set_slot (PortableInterceptor::SlotId identifier,
                           const CORBA::Any & data);
    //@}

    virtual CORBA::ORB_ptr _get_orb ();

    /// Number of slots allocated in the slot table.
    PortableInterceptor::SlotId slot_count () const;

    /// Retrieve the PICurrent implementation from TSS, i.e. the thread
    /// scope current (TSC).
    PICurrent_Impl * tsc ();

    /// Verify the validity of the given SlotId.
    void check_validity (const PortableInterceptor::SlotId &identifier);

    /// Initialize the PICurrent object.
    void initialize (PortableInterceptor::SlotId sc);

  protected:
    /// Destructor
    /**
     * Protected destructor to enforce the fact this class is reference
     * counted, and should not be destroyed using delete() by anything
     * other than the reference counting mechanism.
     */
    virtual ~PICurrent () = default;

  private:
    /// Prevent copying through the copy constructor and the assignment
    /// operator.
    //@{
    PICurrent (const PICurrent &) = delete;
    void operator= (const PICurrent &) = delete;
    //@}

  private:
    /// Reference to the orb core.
    TAO_ORB_Core &orb_core_;

    /// TSS slot assigned to PICurrent_Impl objects in the OrbCore.
    /// Allocated by the orb_core_.add_tss_cleanup_func() when our
    /// initialize() method is called.
    size_t tss_slot_;

    /// The number of allocated PICurrent slots end user wants. (0 is
    /// uninitialized or PICurrent is not used as no data is to be stored).
    PortableInterceptor::SlotId slot_count_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "PICurrent.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_H */

// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Current.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_CURRENT_H
#define TAO_SSLIOP_CURRENT_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Current_Impl.h"

#include "orbsvcs/SSLIOPC.h"
#include "tao/ORB_Core.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_SSLIOP_Current
 *
 * @brief Implementation of the TAO SSLIOP::Current extension.
 *
 * This object can be used to obtain SSL session related information
 * about the current execution context.  For example, SSL peer
 * certificate chains for the current request can be obtained from
 * this object.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Current
  : public SSLIOP::Current,
    public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_SSLIOP_Current (size_t tss_slot, const char *orb_id);

  /// Return the peer certificate chain associated with the current
  /// upcall and SSL session.
  virtual SSLIOP::SSL_Cert * get_peer_certificate_chain (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     SSLIOP::Current::NoContext));

  void setup (TAO_SSLIOP_Current_Impl *impl);
  // Setup the Current.

  void teardown (void);
  // Teardown the Current for this request.

protected:

  /// Destructor
  /// Protected to force allocation on the heap.
  ~TAO_SSLIOP_Current (void);

  /// Fully initialize this object.  This method is used predominantly
  /// to set the ORB core pointer.
  int init (void);

  /// Set the TSS SSLIOP::Current implementation.
  int implementation (TAO_SSLIOP_Current_Impl *impl);

  /// Return the TSS SSLIOP::Current implementation.
  TAO_SSLIOP_Current_Impl *implementation (void);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_SSLIOP_Current (const TAO_SSLIOP_Current &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_SSLIOP_Current &))
  //@}

private:

  /// TSS slot assigned to this object.
  size_t tss_slot_;

  /// The ORBid of the ORB with which this object is registered.
  CORBA::String_var orb_id_;

  /// Pointer to the ORB Core corresponding to the ORB with which this
  /// object is registered.
  TAO_ORB_Core *orb_core_;

  /// Is setup complete?
  int setup_done_;

  /// Current previous from <this>.
  TAO_SSLIOP_Current_Impl *previous_current_impl_;

};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_CURRENT_H */

// -*- C++ -*-

// ===================================================================
/**
 *  @file   Security_Current.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SECURITY_CURRENT_H
#define TAO_SECURITY_CURRENT_H

#include /**/ "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"
#include "tao/ORB_Core.h"

#include "Security_Current_Impl.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Security_Current
 *
 * @brief Implementation of the SecurityLevel2::Current interface.
 *
 * This object can be used to obtain session related security
 * information about the current execution context.
 */
class TAO_Security_Export TAO_Security_Current
  : public SecurityLevel2::Current,
    public TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_Security_Current (size_t tss_slot, const char *orb_id);

  /**
   * @name SecurityLevel1::Current Methods
   *
   * These methods are founds in the SecurityLevel1::Current
   * interface.
   */
  //@{
  /// Return the security attributes corresponding to the types in the
  /// given attribute type list associated with the current request.
  virtual Security::AttributeList * get_attributes (
      const Security::AttributeTypeList & attributes
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /**
   * @name SecurityLevel2::Current Methods
   *
   * These methods are founds in the SecurityLevel2::Current
   * interface.
   */
  //@{
  /// Return the Credentials received from the client associate with
  /// the current request.
  virtual SecurityLevel2::ReceivedCredentials_ptr received_credentials (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Return the TSS slot ID assigned to the "SecurityCurrent" object.
  /**
   * The concrete TSS SecurityCurrent implementations will each use
   * this slot ID.
   */
  size_t tss_slot (void) const;

protected:

  /// Destructor
  /// Protected to force allocation on the heap.
  ~TAO_Security_Current (void);

  /// Fully initialize this object.  This method is used predominantly
  /// to set the ORB core pointer.
  int init (void);

  /// Set the TSS Security::Current implementation.
  /**
   * The pointer is actually one to a concrete implementation provided
   * by the underlying security mechanism.  For example, SSLIOP
   * implements the SecurityLevel2::Current interface.  Similarly,
   * SECIOP would do the same.
   *
   * There is no function that places the implementation pointer in
   * TSS.  The underlying security mechanism does that.
   */
  TAO_Security_Current_Impl *implementation (void);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_Security_Current (const TAO_Security_Current &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Security_Current &))
  //@}

private:

  /// TSS slot assigned to this object.
  size_t tss_slot_;

  /// The ORBid of the ORB with which this object is registered.
  CORBA::String_var orb_id_;

  /// Pointer to the ORB Core corresponding to the ORB with which this
  /// object is registered.
  TAO_ORB_Core *orb_core_;

};

#if defined (__ACE_INLINE__)
# include "Security_Current.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SECURITY_CURRENT_H */

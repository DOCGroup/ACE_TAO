// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_Credentials.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_CREDENTIALS_H
#define TAO_SSLIOP_CREDENTIALS_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"


/// Forward declarations.
struct x509_st;
typedef x509_st X509;


/**
 * @class TAO_SSLIOP_Credentials
 *
 * @brief SSLIOP-specific implementation of the
 *        SecurityLevel2::Credentials interface.
 *
 * This class encapsulates the X.509 certificate associated with a
 * given a principal.
 */
class TAO_SSLIOP_Credentials
  : public virtual SecurityLevel2::Credentials,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_SSLIOP_Credentials (X509 *cert);

  /// Return a duplicate (specifically a deep copy) of this credential
  /// object.
  virtual SecurityLevel2::Credentials_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the invocation credentials type this object is.
  /**
   * If this Credentials object was created by the
   * PrincipalAuthenticator then the "own" credentials type
   * (i.e. Security::SecOwnCredentials) is returned.  Specifically,
   * the credentials represented by this Credentials object are our
   * own, not the peer's credentials.
   * @par
   * If this Credentials object corresponds to the credentials
   * received from the peer, then the "received" credentials type
   * (i.e. Security::SecReceivedCredentials) is returned.
   * @par
   * If this Credentials object corresponds to the target object's
   * credentials then the "target" credentials type
   * (i.e. Security::SecTargetCredentials) is returned.
   */
  virtual Security::InvocationCredentialsType credentials_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AuthenticationStatus authentication_state (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * mechanism (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions accepting_options_supported (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void accepting_options_supported (
      Security::AssociationOptions accepting_options_supported
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions accepting_options_required (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void accepting_options_required (
      Security::AssociationOptions accepting_options_required
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions invocation_options_supported (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void invocation_options_supported (
      Security::AssociationOptions invocation_options_supported
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions invocation_options_required (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void invocation_options_required (
      Security::AssociationOptions invocation_options_required
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean get_security_feature (
      Security::CommunicationDirection direction,
      Security::SecurityFeature feature
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean set_attributes (
      const Security::AttributeList & requested_attributes,
      Security::AttributeList_out actual_attributes
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AttributeList * get_attributes (
      const Security::AttributeTypeList & attributes
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Verify that this credential is still valid.
  virtual CORBA::Boolean is_valid (
      Security::UtcT_out expiry_time
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean refresh (
      const CORBA::Any & refresh_data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Destructor.
  /**
   * Destructor is protected to enforce proper reference counting.
   * Specifically, this reference counted object should only be
   * deallocated by the reference counting mechanism.
   */
  ~TAO_SSLIOP_Credentials (void);

protected:

  /// Security association options supported by this Credentials
  /// object when accepting client invocations.
  /**
   * This attribute corresponds to the SSLIOP::target_supports field
   * in the security association options in the SSLIOP tagged
   * component that is embedded in IORs created by SSLIOP-enabled
   * server side ORBs.
   */
  Security::AssociationOptions accepting_options_supported_;

  /// Security association options required by this Credentials
  /// object when accepting client invocations.
  /**
   * This attribute corresponds to the SSLIOP::target_requires field
   * in the security association options in the SSLIOP tagged
   * component that is embedded in IORs created by SSLIOP-enabled
   * server side ORBs.
   */
  Security::AssociationOptions accepting_options_required_;

  /// Security association options supported by this Credentials
  /// object when invoking operations on the target object.
  Security::AssociationOptions invocation_options_supported_;

  /// Security association options required by this Credentials
  /// object when invoking operations on the target object.
  Security::AssociationOptions invocation_options_required_;

  /// The X.509 certificate associated with this SSLIOP Credentials
  /// object.
  X509 *x509_;

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_H */

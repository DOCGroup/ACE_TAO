// -*- C++ -*-

// ===================================================================
/**
 *  @file   PrincipalAuthenticator.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_PRINCIPAL_AUTHENTICATOR_H
#define TAO_PRINCIPAL_AUTHENTICATOR_H

#include "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityLevel2C.h"

#include "ace/Array_Base.h"

class TAO_PrincipalAuthenticator_Impl;

/**
 * @class TAO_PrincipalAuthenticator
 *
 * @brief Implementation of the SecurityLevel2::PrincipalAuthenticator
 *        interface.
 *
 * This class implements the chain-of-responsibility design pattern to
 * decide which security mechanism
 */
class TAO_Security_Export TAO_PrincipalAuthenticator
  : public virtual SecurityLevel2::PrincipalAuthenticator,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_PrincipalAuthenticator (void/* const Security::MechanismTypeList &mechanisms */);

  /**
   * @name SecurityLevel2::PrincipalAuthenticator Methods
   *
   * Methods exposed by the SecurityLevel2::PrincipalAuthenticator
   * interface.
   */
  //@{
  /// Return the authentication methods supported by the given
  /// mechanism.
  virtual Security::AuthenticationMethodList * get_supported_authen_methods (
      const char * mechanism
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Authenticate the principal, request privileges and create
  /// Credentials.
  /**
   * Created Credentials will be placed in the SecurityManager
   * object's own_credentials list.
   * @par
   * If authenticate() returns with a Security::SecAuthContinue,
   * e.g. due to an inability to complete authentication in a single
   * operation, then authentication must completed using the
   * continue_authentication() method.
   */
  virtual Security::AuthenticationStatus authenticate (
      Security::AuthenticationMethod method,
      const char * mechanism,
      const char * security_name,
      const CORBA::Any & auth_data,
      const Security::AttributeList & privileges,
      SecurityLevel2::Credentials_out creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Continue partially completed authentication process.
  virtual Security::AuthenticationStatus continue_authentication (
      const CORBA::Any & response_data,
      SecurityLevel2::Credentials_ptr creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /// Register a security mechanism-specific (e.g. SSLIOP, Kerberos,
  /// etc.) principal authenticator implementation with the main
  /// PrincipalAuthenticator object (i.e. this object).
  void register_principal_authenticator (TAO_PrincipalAuthenticator_Impl *);

protected:

  /// Destructor
  /**
   * Protected destructor to enforce the fact this class is reference
   * counted, and should not be destroyed using delete() by anything
   * other than the reference counting mechanism.
   */
  ~TAO_PrincipalAuthenticator (void);

private:

  /// Array of registered security mechanism-specific principal
  /// authenticators.
  ACE_Array_Base<TAO_PrincipalAuthenticator_Impl *> authenticators_;

};

#include "ace/post.h"

#endif  /* TAO_PRINCIPAL_AUTHENTICATOR_H */

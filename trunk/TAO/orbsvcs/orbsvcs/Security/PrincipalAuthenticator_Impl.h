// -*- C++ -*-

// ===================================================================
/**
 *  @file   PrincipalAuthenticator_Impl.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_PRINCIPAL_AUTHENTICATOR_IMPL_H
#define TAO_PRINCIPAL_AUTHENTICATOR_IMPL_H

#include "ace/pre.h"

#include "orbsvcs/SecurityLevel2C.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_PrincipalAuthenticator_Impl
 *
 * @brief Abstract base class with interface that all security
 *        mechansim-specific prinicipal authenticators are required to
 *        implement.
 *
 * All security mechanisms must implement this class.  The
 * SecurityLevel2::PrincipalAuthenticator implementation uses the
 * chain-of-responsibility design pattern to decide which security
 * mechanism will be used to create a given set of credentials.
 */
class TAO_PrincipalAuthenticator_Impl
{
public:

  /// Return the authenication method identifier.
  virtual Security::AuthenticationMethod authentication_method (void) const = 0;

  /// Return the security mechanism identifier.
  /**
   * The SecurityLevel2::PrincipalAuthenticator implementation will
   * use the returned value when deciding which security mechanism
   * should be used to create a given set of credentials.
   */
  virtual const char * mechanism (void) const = 0;

  /// Authenticate the principal, request privileges and create
  /// Credentials.
  /**
   * Created credentials will be placed in the SecurityManager
   * object's own_credentials list.
   * @par
   * If authenticate() returns with a Security::SecAuthContinue,
   * e.g. due to an inability to complete authentication in a single
   * operation, then authentication must completed using the
   * continue_authentication() method.
   */
  virtual Security::AuthenticationStatus authenticate (
      Security::AuthenticationMethod method,
      const char * security_name,
      const CORBA::Any & auth_data,
      const Security::AttributeList & privileges,
      SecurityLevel2::Credentials_out creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  /// Continue partially completed authentication process.
  virtual Security::AuthenticationStatus continue_authentication (
      const CORBA::Any & response_data,
      SecurityLevel2::Credentials_ptr creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

  //@}

};

#include "ace/post.h"

#endif  /* TAO_PRINCIPAL_AUTHENTICATOR_IMPL_H */

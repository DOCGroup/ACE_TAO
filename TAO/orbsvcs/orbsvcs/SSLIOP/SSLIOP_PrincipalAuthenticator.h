// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_PrincipalAuthenticator.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_SSLIOP_PRINCIPAL_AUTHENTICATOR_H
#define TAO_SSLIOP_PRINCIPAL_AUTHENTICATOR_H

#include "ace/pre.h"

#include "orbsvcs/Security/PrincipalAuthenticator_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_SSLIOP_PrincipalAuthenticator
 *
 * @brief SSLIOP-specific implementation of the
 *        TAO_PrincipalAuthenticator_Impl abstract base class.
 *
 * This class implements the SSLIOP-specific functionality used when
 * creating SSL credentials (X.509 certificates) with the
 * SecurityLevel2::PrincipalAuthenticator interface.
 */
class TAO_SSLIOP_PrincipalAuthenticator
  : public TAO_PrincipalAuthenticator_Impl
{
public:

  /// Constructor
  TAO_SSLIOP_PrincipalAuthenticator (void);

  /// Destructor
  virtual ~TAO_SSLIOP_PrincipalAuthenticator (void);

  /**
   * @name TAO_PrincipalAuthenticator_Impl Methods
   *
   * Methods required by the TAO_PrincipalAuthenticator_Impl abstract
   * base class.  These methods correspond to the methods in the
   * SecurityLevel2::PrincipalAuthenticator interface.
   */
  //@{

  /// Return the authenication method identifier.
  virtual Security::AuthenticationMethod authentication_method (void) const;

  /// Return the security mechanism identifier.
  /**
   * The SecurityLevel2::PrincipalAuthenticator implementation will
   * use the returned value when deciding which security mechanism
   * should be used to create a given set of credentials.
   */
  virtual const char * mechanism (void) const;

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
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Continue partially completed authentication process.
  virtual Security::AuthenticationStatus continue_authentication (
      const CORBA::Any & response_data,
      SecurityLevel2::Credentials_ptr creds,
      CORBA::Any_out continuation_data,
      CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

};

#include "ace/post.h"

#endif  /* TAO_SSLIOP_PRINCIPAL_AUTHENTICATOR_H */

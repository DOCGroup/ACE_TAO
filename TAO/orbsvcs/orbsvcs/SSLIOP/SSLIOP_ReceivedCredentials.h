// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_ReceivedCredentials.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_RECEIVED_CREDENTIALS_H
#define TAO_SSLIOP_RECEIVED_CREDENTIALS_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Credentials.h"

/**
 * @class TAO_SSLIOP_ReceivedCredentials
 *
 * @brief SSLIOP-specific implementation of the
 *        SecurityLevel2::ReceivedCredentials interface.
 *
 * An SSLIOP "received credential" encapsulates the peers X.509
 * certificate.
 */
class TAO_SSLIOP_ReceivedCredentials
  : public virtual TAO_SSLIOP_Credentials,
    public virtual SecurityLevel2::ReceivedCredentials
{
public:

  /// Constructor.
  TAO_SSLIOP_ReceivedCredentials (X509 *cert);

  /**
   * @name SecurityLevel2::Credentials Methods
   *
   * SecurityLevel2::Credentials methods implemented for
   * SecurityLevel2::ReceivedCredentials.
   */
  //@{
  virtual SecurityLevel2::Credentials_ptr copy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::InvocationCredentialsType credentials_type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  /**
   * @name SecurityLevel2::ReceivedCredentials Methods
   *
   * Methods required by the SecurityLevel2::ReceivedCredentials
   * interface.
   */
  //@{
  virtual SecurityLevel2::Credentials_ptr accepting_credentials (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions association_options_used (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::DelegationState delegation_state (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::DelegationMode delegation_mode (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_RECEIVED_CREDENTIALS_H */

// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_TargetCredentials.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_TARGET_CREDENTIALS_H
#define TAO_SSLIOP_TARGET_CREDENTIALS_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Credentials.h"


/**
 * @class TAO_SSLIOP_TargetCredentials
 *
 * @brief SSLIOP-specific implementation of the
 *        SecurityLevel2::TargetCredentials interface.
 *
 * 
 */
class TAO_SSLIOP_TargetCredentials
  : public virtual TAO_SSLIOP_Credentials,
    public virtual SecurityLevel2::TargetCredentials
{
public:

  /// Constructor
  TAO_SSLIOP_TargetCredentials (X509 *cert);

  /**
   * @name SecurityLevel2::Credentials Methods
   *
   * SecurityLevel2::Credentials methods implemented for
   * SecurityLevel2::TargetCredentials.
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
   * @name SecurityLevel2::TargetCredentials Methods
   *
   * Methods required by the SecurityLevel2::TargetCredentials
   * interface.
   */
  //@{
  virtual SecurityLevel2::Credentials_ptr initiating_credentials (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions association_options_used (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_TARGET_CREDENTIALS_H */

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

class TAO_SSLIOP_ReceivedCredentials;
typedef TAO_SSLIOP_ReceivedCredentials* TAO_SSLIOP_ReceivedCredentials_ptr;

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
  TAO_SSLIOP_ReceivedCredentials (X509 *cert, EVP_PKEY *evp);

  /**
   * @name SecurityLevel2::Credentials Methods
   *
   * SecurityLevel2::Credentials methods implemented for
   * SecurityLevel2::ReceivedCredentials.
   */
  //@{
  virtual SecurityLevel2::Credentials_ptr copy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::InvocationCredentialsType credentials_type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions association_options_used (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::DelegationState delegation_state (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::DelegationMode delegation_mode (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  //@{
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_SSLIOP_ReceivedCredentials_ptr _ptr_type;
  // typedef TAO_SSLIOP_ReceivedCredentials_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static int _tao_class_id;

  // The static operations.
  static TAO_SSLIOP_ReceivedCredentials_ptr _duplicate (TAO_SSLIOP_ReceivedCredentials_ptr obj);

  static TAO_SSLIOP_ReceivedCredentials_ptr _narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_SSLIOP_ReceivedCredentials_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_SSLIOP_ReceivedCredentials_ptr _nil (void)
    {
      return (TAO_SSLIOP_ReceivedCredentials_ptr)0;
    }

  virtual void *_tao_QueryInterface (ptr_arith_t type);
  //@}

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_RECEIVED_CREDENTIALS_H */

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

class TAO_SSLIOP_TargetCredentials;
typedef TAO_SSLIOP_TargetCredentials* TAO_SSLIOP_TargetCredentials_ptr;

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

  TAO_SSLIOP_TargetCredentials (X509 *cert, EVP_PKEY *evp);

  /**
   * @name SecurityLevel2::Credentials Methods
   *
   * SecurityLevel2::Credentials methods implemented for
   * SecurityLevel2::TargetCredentials.
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
   * @name SecurityLevel2::TargetCredentials Methods
   *
   * Methods required by the SecurityLevel2::TargetCredentials
   * interface.
   */
  //@{
  virtual SecurityLevel2::Credentials_ptr initiating_credentials (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationOptions association_options_used (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

  //@{
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_SSLIOP_TargetCredentials_ptr _ptr_type;
  // typedef TAO_SSLIOP_TargetCredentials_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static int _tao_class_id;

  // The static operations.
  static TAO_SSLIOP_TargetCredentials_ptr _duplicate (TAO_SSLIOP_TargetCredentials_ptr obj);

  static TAO_SSLIOP_TargetCredentials_ptr _narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_SSLIOP_TargetCredentials_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    );

  static TAO_SSLIOP_TargetCredentials_ptr _nil (void)
    {
      return (TAO_SSLIOP_TargetCredentials_ptr)0;
    }

  virtual void *_tao_QueryInterface (ptrdiff_t type);
  //@}

};


#include "ace/post.h"

#endif  /* TAO_SSLIOP_TARGET_CREDENTIALS_H */

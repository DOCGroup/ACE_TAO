// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Vault.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================


#ifndef TAO_SSLIOP_VAULT_H
#define TAO_SSLIOP_VAULT_H

#include "ace/pre.h"

#include "orbsvcs/SecurityReplaceableC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOPC.h"

#include <openssl/opensslconf.h>


/// Forward declarations for OpenSSL data structures.
extern "C"
{
  typedef struct x509_st X509;
  typedef struct evp_pkey_st EVP_PKEY;
}


/**
 * @class TAO_SSLIOP_Vault
 *
 * @brief SSLIOP-specific implementation of the
 *        SecurityReplaceable::Vault interface.
 *
 * This class implements the SSLIOP-specific functionality used when
 * creating SSL credentials (X.509 certificates) with the
 * SecurityLevel2::PrincipalAuthenticator interface.
 */
class TAO_SSLIOP_Vault
  : public virtual SecurityReplaceable::Vault,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_SSLIOP_Vault (void);

  /// Destructor
  virtual ~TAO_SSLIOP_Vault (void);

  /**
   * @name SecurityReplaceable::Vault Methods
   *
   * Methods required by the SecurityReplaceable::Vault interface.
   */
  //@{

  virtual Security::AuthenticationMethodList * get_supported_authen_methods (
      const char * mechanism
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::OIDList * supported_mech_oids (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
  virtual Security::AuthenticationStatus acquire_credentials (
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

  virtual Security::AuthenticationStatus continue_credentials_acquisition (
      const CORBA::Any & response_data,
        SecurityLevel2::Credentials_ptr creds,
        CORBA::Any_out continuation_data,
        CORBA::Any_out auth_specific_data
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::TaggedComponentList * create_ior_components (
      SecurityLevel2::Credentials_ptr creds_list
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationStatus init_security_context (
      SecurityLevel2::Credentials_ptr creds,
      const char * target_security_name,
      CORBA::Object_ptr target,
      Security::DelegationMode delegation_mode,
      const Security::OptionsDirectionPairList & association_options,
      const char * mechanism,
      const Security::Opaque & comp_data,
      const Security::ChannelBindings & chan_binding,
      Security::OpaqueBuffer_out security_token,
      SecurityReplaceable::ClientSecurityContext_out security_context
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::AssociationStatus accept_security_context (
      const SecurityLevel2::CredentialsList & creds_list,
      const Security::ChannelBindings & chan_bindings,
      const Security::Opaque & in_token,
      Security::Opaque_out out_token
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Security::MechandOptionsList * get_supported_mechs (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Create an OpenSSL X509 structure using the provided
  /// authentication data.
  X509 * make_X509 (const SSLIOP::File &certificate);

  /// Create an OpenSSL EVP_PKEY structure using the provided
  /// authentication data.
  EVP_PKEY * make_EVP_PKEY (const SSLIOP::File &key);

};

#include "ace/post.h"

#endif  /* TAO_SSLIOP_VAULT_H */

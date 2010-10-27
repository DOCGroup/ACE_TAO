// $Id$

#include "orbsvcs/Security/SL3_SecurityManager.h"
#include "orbsvcs/Security/SL3_ContextEstablishmentPolicy.h"
#include "orbsvcs/Security/SL3_ObjectCredentialsPolicy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SL3::SecurityManager::SecurityManager (
  SecurityLevel3::CredentialsCurator_ptr cc)
  : credentials_curator_ (SecurityLevel3::CredentialsCurator::_duplicate (cc))
{
}

TAO::SL3::SecurityManager::~SecurityManager (void)
{
}

SecurityLevel3::CredentialsCurator_ptr
TAO::SL3::SecurityManager::credentials_curator ()
{
  return
    SecurityLevel3::CredentialsCurator::_duplicate (
      this->credentials_curator_.in ());
}

SecurityLevel3::TargetCredentials_ptr
TAO::SL3::SecurityManager::get_target_credentials (CORBA::Object_ptr /* the_object */)
{
  throw CORBA::NO_IMPLEMENT ();
}

SecurityLevel3::ContextEstablishmentPolicy_ptr
TAO::SL3::SecurityManager::create_context_estab_policy (
    SecurityLevel3::CredsDirective creds_directive,
    const SecurityLevel3::OwnCredentialsList & creds_list,
    SecurityLevel3::FeatureDirective use_client_auth,
    SecurityLevel3::FeatureDirective use_target_auth,
    SecurityLevel3::FeatureDirective use_confidentiality,
    SecurityLevel3::FeatureDirective use_integrity)
{
  SecurityLevel3::ContextEstablishmentPolicy_ptr policy;
  ACE_NEW_THROW_EX (policy,
                    TAO::SL3::ContextEstablishmentPolicy (creds_directive,
                                                          creds_list,
                                                          use_client_auth,
                                                          use_target_auth,
                                                          use_confidentiality,
                                                          use_integrity),
                    CORBA::NO_MEMORY ());

  return policy;
}

SecurityLevel3::ObjectCredentialsPolicy_ptr
TAO::SL3::SecurityManager::create_object_creds_policy (
    const SecurityLevel3::OwnCredentialsList & creds_list)
{
  SecurityLevel3::ObjectCredentialsPolicy_ptr policy;
  ACE_NEW_THROW_EX (policy,
                    TAO::SL3::ObjectCredentialsPolicy (creds_list),
                    CORBA::NO_MEMORY ());

  return policy;
}

TAO_END_VERSIONED_NAMESPACE_DECL

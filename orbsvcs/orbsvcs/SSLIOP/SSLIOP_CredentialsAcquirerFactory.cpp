// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_CredentialsAcquirerFactory.h"
#include "orbsvcs/SSLIOP/SSLIOP_CredentialsAcquirer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

SecurityLevel3::CredentialsAcquirer_ptr
TAO::SSLIOP::CredentialsAcquirerFactory::make (
  TAO::SL3::CredentialsCurator_ptr curator,
  const CORBA::Any & acquisition_arguments)
{
  SecurityLevel3::CredentialsAcquirer_ptr ca;
  ACE_NEW_THROW_EX (ca,
                    TAO::SSLIOP::CredentialsAcquirer (curator,
                                                      acquisition_arguments),
                    CORBA::NO_MEMORY ());

  return ca;
}

TAO_END_VERSIONED_NAMESPACE_DECL

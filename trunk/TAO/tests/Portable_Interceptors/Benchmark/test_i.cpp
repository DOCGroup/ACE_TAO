// $Id$

#include "test_i.h"
#include "marker.h"

ACE_RCSID(Benchmark, test_i, "$Id$")

Secure_Vault_i::Secure_Vault_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
// ctor

CORBA::Short
Secure_Vault_i::ready (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 1;
}

void
Secure_Vault_i::authenticate (const char * user
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Test_Interceptors::Invalid))
{
  if (ACE_OS::strcmp (user, "root") != 0)
    ACE_THROW (Test_Interceptors::Invalid ());

}

CORBA::Long
Secure_Vault_i::update_records (CORBA::Long,
                const Test_Interceptors::Secure_Vault::Record &
                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
return 1;
}

void
Secure_Vault_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

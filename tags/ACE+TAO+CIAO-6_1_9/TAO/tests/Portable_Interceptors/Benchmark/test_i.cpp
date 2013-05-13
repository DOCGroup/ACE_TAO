// $Id$

#include "test_i.h"
#include "marker.h"
#include "ace/OS_NS_string.h"

Secure_Vault_i::Secure_Vault_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
// ctor

CORBA::Short
Secure_Vault_i::ready (void)
{
  return 1;
}

void
Secure_Vault_i::authenticate (const char * user)
{
  if (ACE_OS::strcmp (user, "root") != 0)
    throw Test_Interceptors::Invalid ();

}

CORBA::Long
Secure_Vault_i::update_records (CORBA::Long,
                const Test_Interceptors::Secure_Vault::Record &)
{
return 1;
}

void
Secure_Vault_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

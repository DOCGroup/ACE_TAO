ACE_RCSID (IOR_Endpoint_Hostnames, bogus_i, "$Id$")

#include "bogus_i.h"

bogus::bogus()
{
}

bogus::~bogus()
{
}

void
bogus::noop (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

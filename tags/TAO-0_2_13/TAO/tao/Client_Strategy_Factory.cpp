// $Id$

#include "tao/corba.h"

ACE_RCSID(tao, Client_Strategy_Factory, "$Id$")

TAO_Client_Strategy_Factory::TAO_Client_Strategy_Factory (void)
{
}

TAO_Client_Strategy_Factory::~TAO_Client_Strategy_Factory (void)
{
}

ACE_Lock *
TAO_Client_Strategy_Factory::create_iiop_profile_lock (void)
{
  return 0;
}

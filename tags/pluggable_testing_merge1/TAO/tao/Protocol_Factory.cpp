// $Id$

#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Protocol_Factory, "$Id$")

TAO_Protocol_Factory::TAO_Protocol_Factory (void)
{
}

TAO_Protocol_Factory::~TAO_Protocol_Factory (void)
{
}

int 
TAO_Protocol_Factory::init (int argc, char *argv[])
{
 return -1;
}

TAO_Acceptor *
TAO_Protocol_Factory::make_acceptor (void)
{
  return 0;
}

TAO_Connector *
TAO_Protocol_Factory::make_connector (void)
{
  return 0;
}

int 
TAO_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Protocol_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Protocol_Factory>;

#endif

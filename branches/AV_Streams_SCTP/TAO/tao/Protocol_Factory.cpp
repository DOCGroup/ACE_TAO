// $Id$

#include "tao/Protocol_Factory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Protocol_Factory, "$Id$")

static const char prefix_[] = "\0";

TAO_Protocol_Factory::TAO_Protocol_Factory (CORBA::ULong tag)
  : tag_ (tag)
{
}

TAO_Protocol_Factory::~TAO_Protocol_Factory (void)
{
}

CORBA::ULong
TAO_Protocol_Factory::tag (void) const
{
  return this->tag_;
}

int
TAO_Protocol_Factory::init (int /* argc */,
                            ACE_TCHAR * /* argv */ [])
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
TAO_Protocol_Factory::match_prefix (const ACE_CString & /* prefix */)
{
  return 0;
}

const char *
TAO_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_Protocol_Factory::options_delimiter (void) const
{
  return '\0';
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Protocol_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Protocol_Factory>

#endif

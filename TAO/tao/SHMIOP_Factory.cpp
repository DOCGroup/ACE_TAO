// $Id$

#include "tao/SHMIOP_Factory.h"
#include "tao/SHMIOP_Acceptor.h"
#include "tao/SHMIOP_Connector.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, SHMIOP_Factory, "$Id$")

static const char prefix_[] = "shmiop";

TAO_SHMIOP_Protocol_Factory::TAO_SHMIOP_Protocol_Factory (void)
  :  major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR)
{
}

TAO_SHMIOP_Protocol_Factory::~TAO_SHMIOP_Protocol_Factory (void)
{
}

int
TAO_SHMIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_SHMIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_SHMIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_SHMIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_SHMIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_SHMIOP_Protocol_Factory::init (int /* argc */,
                                 char* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_SHMIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_SHMIOP_Connector,
                  0);
  return connector;
}

int
TAO_SHMIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_SHMIOP_Protocol_Factory,
                       ASYS_TEXT ("SHMIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SHMIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_SHMIOP_Protocol_Factory)

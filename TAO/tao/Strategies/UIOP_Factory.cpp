// -*- C++ -*-
// $Id$

#include "UIOP_Factory.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Acceptor.h"
#include "UIOP_Connector.h"

ACE_RCSID(Strategies, UIOP_Factory, "$Id$")

static const char prefix_[] = "uiop";

TAO_UIOP_Protocol_Factory::TAO_UIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (TAO_TAG_UIOP_PROFILE),
     major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR)
{
}

TAO_UIOP_Protocol_Factory::~TAO_UIOP_Protocol_Factory (void)
{
}

int
TAO_UIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_UIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_UIOP_Protocol_Factory::options_delimiter (void) const
{
  return '|';
}

TAO_Acceptor *
TAO_UIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_UIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_UIOP_Protocol_Factory::init (int /* argc */,
                                 ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_UIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_UIOP_Connector,
                  0);

  return connector;
}

int
TAO_UIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 1;
}

ACE_STATIC_SVC_DEFINE (TAO_UIOP_Protocol_Factory,
                       ACE_TEXT ("UIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_UIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                          ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_Strategies, TAO_UIOP_Protocol_Factory)


#endif  /* TAO_HAS_UIOP == 1 */

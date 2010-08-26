// -*- C++ -*-
// $Id$

#include "tao/IIOP_Factory.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/IIOP_Acceptor.h"
#include "tao/IIOP_Connector.h"
#include "tao/IOPC.h"
#include "ace/OS_NS_strings.h"

static const char the_prefix[] = "iiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IIOP_Protocol_Factory::TAO_IIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP)
{
}

TAO_IIOP_Protocol_Factory::~TAO_IIOP_Protocol_Factory (void)
{
}

int
TAO_IIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix) == 0);
}

const char *
TAO_IIOP_Protocol_Factory::prefix (void) const
{
  return ::the_prefix;
}

char
TAO_IIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_IIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_IIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_IIOP_Protocol_Factory::init (int /* argc */,
                                 ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_IIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_IIOP_Connector,
                  0);
  return connector;
}

int
TAO_IIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_IIOP_Protocol_Factory,
                       ACE_TEXT ("IIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_IIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_IIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL


#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

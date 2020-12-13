// -*- C++ -*-
#include "IIOP_Factory.h"

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "IIOP_Acceptor.h"
#include "IIOP_Connector.h"
#include "IOPC.h"
#include "ace/OS_NS_strings.h"

static const char the_prefix[] = "iiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IIOP_Protocol_Factory::TAO_IIOP_Protocol_Factory ()
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP)
{
}

TAO_IIOP_Protocol_Factory::~TAO_IIOP_Protocol_Factory ()
{
}

int
TAO_IIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix) == 0);
}

const char *
TAO_IIOP_Protocol_Factory::prefix () const
{
  return ::the_prefix;
}

char
TAO_IIOP_Protocol_Factory::options_delimiter () const
{
  return '/';
}

TAO_Acceptor *
TAO_IIOP_Protocol_Factory::make_acceptor ()
{
  TAO_Acceptor *acceptor = nullptr;

  ACE_NEW_RETURN (acceptor,
                  TAO_IIOP_Acceptor,
                  nullptr);

  return acceptor;
}

int
TAO_IIOP_Protocol_Factory::init (int /* argc */,
                                 ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_IIOP_Protocol_Factory::make_connector ()
{
  TAO_Connector *connector = nullptr;

  ACE_NEW_RETURN (connector,
                  TAO_IIOP_Connector,
                  nullptr);
  return connector;
}

int
TAO_IIOP_Protocol_Factory::requires_explicit_endpoint () const
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

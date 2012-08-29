// $Id$

#include "tao/Strategies/COIOP_Factory.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/COIOP_Acceptor.h"
#include "tao/Strategies/COIOP_Connector.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_strings.h"

static const char the_prefix[] = "COIOP";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Protocol_Factory::TAO_COIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (TAO_TAG_COIOP_PROFILE)
{
}

TAO_COIOP_Protocol_Factory::~TAO_COIOP_Protocol_Factory (void)
{
}

int
TAO_COIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix) == 0);
}

const char *
TAO_COIOP_Protocol_Factory::prefix (void) const
{
  return ::the_prefix;
}

char
TAO_COIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_COIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_COIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_COIOP_Protocol_Factory::init (int /* argc */,
                                 ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_COIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_COIOP_Connector,
                  0);
  return connector;
}

int
TAO_COIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}


ACE_STATIC_SVC_DEFINE (TAO_COIOP_Protocol_Factory,
                       ACE_TEXT ("COIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_COIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_Strategies, TAO_COIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */

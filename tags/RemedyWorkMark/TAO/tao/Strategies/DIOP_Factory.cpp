// $Id$

#include "tao/Strategies/DIOP_Factory.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/DIOP_Acceptor.h"
#include "tao/Strategies/DIOP_Connector.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_strings.h"

static const char the_prefix[] = "diop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Protocol_Factory::TAO_DIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (TAO_TAG_DIOP_PROFILE)
{
}

TAO_DIOP_Protocol_Factory::~TAO_DIOP_Protocol_Factory (void)
{
}

int
TAO_DIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix) == 0);
}

const char *
TAO_DIOP_Protocol_Factory::prefix (void) const
{
  return ::the_prefix;
}

char
TAO_DIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_DIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_DIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_DIOP_Protocol_Factory::init (int /* argc */, ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_DIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_DIOP_Connector,
                  0);
  return connector;
}

int
TAO_DIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  // This switch is actually meant to distinguish between pluggable
  // protocols which are able to clean up their endpoints and such
  // that aren't. E.g. UIOP will leave files, it therefore returns 1,
  // IIOP cleans up its endpoint resources, which therefore return 0.
  //
  // DIOP does clean up endpoint resources, but as DIOP is only
  // suitable for special environments, e.g. it supports only one-ways,
  // it returns 1 for an other reason than resource clean-up.

  return 1;
}


ACE_STATIC_SVC_DEFINE (TAO_DIOP_Protocol_Factory,
                       ACE_TEXT ("DIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_DIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_Strategies, TAO_DIOP_Protocol_Factory)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

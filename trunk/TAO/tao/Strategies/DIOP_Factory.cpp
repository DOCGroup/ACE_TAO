// $Id$

#include "DIOP_Factory.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "DIOP_Acceptor.h"
#include "DIOP_Connector.h"

ACE_RCSID(TAO_Strategies, DIOP_Factory, "$Id$")

static const char prefix_[] = "diop";

TAO_DIOP_Protocol_Factory::TAO_DIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (TAO_TAG_UDP_PROFILE),
     major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR)
{
}

TAO_DIOP_Protocol_Factory::~TAO_DIOP_Protocol_Factory (void)
{
}

int
TAO_DIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_DIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
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
TAO_DIOP_Protocol_Factory::init (int /* argc */,
                                 char* /* argv */ [])
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

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include "SCIOP_Factory.h"
#include "SCIOP_Acceptor.h"
#include "SCIOP_Connector.h"

#if TAO_HAS_SCIOP == 1

#include "tao/IOPC.h"

ACE_RCSID (tao,
           SCIOP_Factory,
           "$Id$")

static const char prefix_[] = "sciop";

TAO_SCIOP_Protocol_Factory::TAO_SCIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (TAO_TAG_SCIOP_PROFILE),
     major_ (TAO_DEF_SCIOP_MAJOR),
     minor_ (TAO_DEF_SCIOP_MINOR)
{
}

TAO_SCIOP_Protocol_Factory::~TAO_SCIOP_Protocol_Factory (void)
{
}

int
TAO_SCIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_SCIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_SCIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_SCIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_SCIOP_Acceptor,
                  0);

  return acceptor;
}

int
TAO_SCIOP_Protocol_Factory::init (int /* argc */,
                                 ACE_TCHAR* /* argv */ [])
{
  return 0;
}

TAO_Connector *
TAO_SCIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_SCIOP_Connector,
                  0);
  return connector;
}

int
TAO_SCIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_SCIOP_Protocol_Factory,
                       ACE_TEXT ("SCIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SCIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_SCIOP_Protocol_Factory)

#endif /* TAO_HAS_SCIOP == 1 */

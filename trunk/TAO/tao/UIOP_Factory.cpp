// -*- C++ -*-
// $Id$

#include "tao/UIOP_Factory.h"

# if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Acceptor.h"
#include "tao/UIOP_Connector.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, UIOP_Factory, "$Id$")

static const char prefix_[] = "uiop";

TAO_UIOP_Protocol_Factory::TAO_UIOP_Protocol_Factory (void)
  :  major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR)
{
}

TAO_UIOP_Protocol_Factory::~TAO_UIOP_Protocol_Factory (void)
{
}

int
TAO_UIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_UIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
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
                                 char* /* argv */ [])
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

ACE_STATIC_SVC_DEFINE (TAO_UIOP_Protocol_Factory,
                       ASYS_TEXT ("UIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_UIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_UIOP_Protocol_Factory)


# endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */

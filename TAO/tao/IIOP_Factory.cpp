// $Id$
#include "tao/IIOP_Factory.h"
#include "tao/IIOP_Acceptor.h"
#include "tao/IIOP_Connector.h"
#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"

ACE_RCSID(tao, IIOP_Factory, "$Id$")

static const char prefix_[] = "iiop";

TAO_IIOP_Protocol_Factory::TAO_IIOP_Protocol_Factory (void)
  :  major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR),
     lite_flag_ (0)
{
}

TAO_IIOP_Protocol_Factory::~TAO_IIOP_Protocol_Factory (void)
{
  
}

int
TAO_IIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_IIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
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
                  TAO_IIOP_Acceptor (this->lite_flag_),
                  0);

  return acceptor;
}

int
TAO_IIOP_Protocol_Factory::init (int argc,
                                 char* argv[])
{
  if (argc > 0)
     {
       if (ACE_OS::strcmp (argv[0],
                           "GIOP_Lite") == 0)
         {
           this->lite_flag_ = 1;
         }
       else
         ACE_ERROR_RETURN ((LM_ERROR,
                            ASYS_TEXT ("(%N|%l)Wrong protocol type \n")),
                           -1);

     }
  return 0;
}

TAO_Connector *
TAO_IIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_IIOP_Connector (this->lite_flag_),
                  0);
  return connector;
}

int
TAO_IIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_IIOP_Protocol_Factory,
                       ASYS_TEXT ("IIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_IIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_IIOP_Protocol_Factory)



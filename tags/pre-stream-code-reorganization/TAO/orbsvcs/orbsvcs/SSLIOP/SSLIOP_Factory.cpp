// $Id$

#include "SSLIOP_Factory.h"
#include "SSLIOP_Acceptor.h"
#include "SSLIOP_Connector.h"
#include "SSL_Context.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, SSLIOP_Factory, "$Id$")

static const char prefix_[] = "iiop";

TAO_SSLIOP_Protocol_Factory::TAO_SSLIOP_Protocol_Factory (void)
  :  major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR),
     use_ssl_ (1)
{
}

TAO_SSLIOP_Protocol_Factory::~TAO_SSLIOP_Protocol_Factory (void)
{
}

int
TAO_SSLIOP_Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::prefix_) == 0);
}

const char *
TAO_SSLIOP_Protocol_Factory::prefix (void) const
{
  return ::prefix_;
}

char
TAO_SSLIOP_Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO_SSLIOP_Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO_SSLIOP_Acceptor (),
                  0);

  return acceptor;
}

int
TAO_SSLIOP_Protocol_Factory::init (int argc,
                                   char* argv[])
{
  for (int curarg = 0; curarg != argc; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                              "-SSLDisable") == 0)
        {
          this->use_ssl_ = 0;
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   "-SSLCertificate") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *lasts = 0;
              const char *type_name =
                ACE_OS::strtok_r (argv[curarg], ":", &lasts);
              const char *path =
                ACE_OS::strtok_r (0, ":", &lasts);
              int type = -1;
              if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
                {
                  type = SSL_FILETYPE_ASN1;
                }
              else if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
                {
                  type = SSL_FILETYPE_PEM;
                }
              ACE_SSL_Context::instance ()->certificate (path, type);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   "-SSLPrivateKey") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *lasts = 0;
              const char *type_name =
                ACE_OS::strtok_r (argv[curarg], ":", &lasts);
              const char *path =
                ACE_OS::strtok_r (0, ":", &lasts);
              int type = -1;
              if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
                {
                  type = SSL_FILETYPE_ASN1;
                }
              else if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
                {
                  type = SSL_FILETYPE_PEM;
                }
              ACE_SSL_Context::instance ()->private_key (path, type);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   "-SSLAuthenticate") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              int mode = SSL_VERIFY_NONE;
              if (ACE_OS::strcasecmp (argv[curarg], "NONE") == 0)
                {
                  mode = SSL_VERIFY_NONE;
                }
              else if (ACE_OS::strcasecmp (argv[curarg], "SERVER") == 0)
                {
                  mode = SSL_VERIFY_PEER;
                }
              else if (ACE_OS::strcasecmp (argv[curarg], "CLIENT") == 0
                       || ACE_OS::strcasecmp (argv[curarg],
                                              "SERVER_AND_CLIENT") == 0)
                {
                  mode = SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
                }
              ACE_SSL_Context::instance ()->default_verify_mode (mode);
            }
        }
    }

  return 0;
}

TAO_Connector *
TAO_SSLIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_SSLIOP_Connector (this->use_ssl_),
                  0);
  return connector;
}

int
TAO_SSLIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_SSLIOP_Protocol_Factory,
                       ASYS_TEXT ("SSLIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SSLIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)

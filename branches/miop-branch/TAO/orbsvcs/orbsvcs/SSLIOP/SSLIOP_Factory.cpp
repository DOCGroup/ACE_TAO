// $Id$

#include "SSLIOP_Factory.h"
#include "SSLIOP_Acceptor.h"
#include "SSLIOP_Connector.h"
#include "SSLIOP_ORBInitializer.h"

#include "orbsvcs/Security/Security_ORBInitializer.h"  // @todo:
                                                       // should go away

#include "ace/SSL/SSL_Context.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Factory,
           "$Id$")

static const char prefix_[] = "iiop";

TAO_SSLIOP_Protocol_Factory::TAO_SSLIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP),
     major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR),
     no_protection_ (0)
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
                  TAO_SSLIOP_Acceptor (this->no_protection_),
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
          ACE_ERROR ((LM_WARNING,
                      ACE_TEXT ("(%P|%t) \"-SSLDisable\" has been ")
                      ACE_TEXT ("superceded by the ")
                      ACE_TEXT ("\"-SSLNoProtection\" option.\n")));

          this->no_protection_ = 1;
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   "-SSLNoProtection") == 0)
        {
          this->no_protection_ = 1;
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

  if (this->register_orb_initializer () != 0)
    return -1;

  return 0;
}

int
TAO_SSLIOP_Protocol_Factory::register_orb_initializer (void)
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // @todo: This hard-coding should be fixed once SECIOP is
      // supported.
      // Register the Security ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_Security_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      PortableInterceptor::ORBInitializer_var initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register the SSLIOP ORB initializer.
      // PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_SSLIOP_ORBInitializer (this->no_protection_),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      //PortableInterceptor::ORBInitializer_var initializer = tmp;
      initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unable to register SSLIOP ORB "
                           "initializer.");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


TAO_Connector *
TAO_SSLIOP_Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO_SSLIOP_Connector (this->no_protection_),
                  0);
  return connector;
}

int
TAO_SSLIOP_Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

ACE_STATIC_SVC_DEFINE (TAO_SSLIOP_Protocol_Factory,
                       ACE_TEXT ("SSLIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SSLIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)

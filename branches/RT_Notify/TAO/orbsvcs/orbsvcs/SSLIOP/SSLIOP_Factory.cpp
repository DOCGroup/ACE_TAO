#include "SSLIOP_Factory.h"
#include "SSLIOP_Acceptor.h"
#include "SSLIOP_Connector.h"
#include "SSLIOP_ORBInitializer.h"

#include "orbsvcs/Security/Security_ORBInitializer.h"  // @todo:
                                                       // should go away

#include "tao/debug.h"

#include "ace/SSL/SSL_Context.h"


ACE_RCSID (TAO_SSLIOP,
           SSLIOP_Factory,
           "$Id$")


static const char prefix_[] = "iiop";

TAO_SSLIOP_Protocol_Factory::TAO_SSLIOP_Protocol_Factory (void)
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP),
     major_ (TAO_DEF_GIOP_MAJOR),
     minor_ (TAO_DEF_GIOP_MINOR),
     qop_ (Security::SecQOPIntegrityAndConfidentiality)
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
                  TAO_SSLIOP_Acceptor (this->qop_),
                  0);

  return acceptor;
}

int
TAO_SSLIOP_Protocol_Factory::init (int argc,
                                   char* argv[])
{
  char *certificate_path = 0;
  char *private_key_path = 0;
  char *dhparams_path = 0;

  int certificate_type = -1;
  int private_key_type = -1;
  int dhparams_type = -1;

  int prevdebug = -1;

  for (int curarg = 0; curarg != argc; ++curarg)
    {
      if ((ACE_OS::strcasecmp (argv[curarg],
                               "-verbose") == 0)
          || (ACE_OS::strcasecmp (argv[curarg],
                                  "-v") == 0))
        {
          if (TAO_debug_level == 0)
            {
              prevdebug = TAO_debug_level;
              TAO_debug_level = 1;
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                              "-SSLNoProtection") == 0)
        {
          // Enable the eNULL cipher.  Note that enabling the "eNULL"
          // cipher only disables encryption.  However, certificate
          // exchanges will still occur.
          if (::SSL_CTX_set_cipher_list (
                  ACE_SSL_Context::instance ()->context (),
                  "DEFAULT:eNULL") == 0)
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) Unable to set eNULL ")
                            ACE_TEXT ("SSL cipher in SSLIOP ")
                            ACE_TEXT ("factory.\n")));

              return -1;
            }

          // This does not disable secure invocations on the server
          // side.  It merely enables insecure ones.  On the client
          // side, secure invocations will be disabled unless
          // overridden by a SecurityLevel2::QOPPolicy in the object
          // reference.
          this->qop_ = Security::SecQOPNoProtection;
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
              certificate_path =
                ACE_OS::strtok_r (0, ":", &lasts);

              if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
                {
                  certificate_type = SSL_FILETYPE_ASN1;
                }
              else if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
                {
                  certificate_type = SSL_FILETYPE_PEM;
                }
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
              private_key_path =
                ACE_OS::strtok_r (0, ":", &lasts);

              if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
                {
                  private_key_type = SSL_FILETYPE_ASN1;
                }
              else if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
                {
                  private_key_type = SSL_FILETYPE_PEM;
                }
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
                  mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
                }

              ACE_SSL_Context::instance ()->default_verify_mode (mode);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   "-SSLDHparams") == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              char *lasts = 0;
              const char *type_name =
                ACE_OS::strtok_r (argv[curarg], ":", &lasts);
              dhparams_path = ACE_OS::strtok_r (0, ":", &lasts);

              if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
                {
                  dhparams_type = SSL_FILETYPE_ASN1;
                }
              else if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
                {
                  dhparams_type = SSL_FILETYPE_PEM;
                }
            }
        }

    }

  // Load in the DH params.  If there was a file explicitly specified,
  // then we do that here, otherwise we load them in from the cert file.
  // Note that we only do this on the server side, I think so we might
  // need to defer this 'til later in the acceptor or something...
  if (dhparams_path == 0)
    {
      // If the user didn't explicitly specify a DH parameters file, we
      // also might find it concatenated in the certificate file.
      // So, we set the dhparams to that if it wasn't explicitly set.
      dhparams_path = certificate_path;
      dhparams_type = certificate_type;
    }

  if (dhparams_path != 0)
    {
      if (ACE_SSL_Context::instance ()->dh_params (dhparams_path,
                                                   dhparams_type) != 0)
        {
          if (dhparams_path != certificate_path)
            {
              // We only want to fail catastrophically if the user specified
              // a dh parameter file and we were unable to actually find it
              // and load from it.
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) SSLIOP_Factory: ")
                            ACE_TEXT ("unable to set ")
                            ACE_TEXT ("DH parameters <%s>"),
                            dhparams_path));
              return -1;
            }
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_INFO,
                            ACE_TEXT ("(%P|%t) SSLIOP_Factory: ")
                            ACE_TEXT ("No DH parameters found in ")
                            ACE_TEXT ("certificate <%s>; either none ")
                            ACE_TEXT ("are needed (RSA) or badness will ensue later.\n"),
                            dhparams_path));
            }
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("Diffie-Hellman params ")
                        ACE_TEXT ("from %s\n"),
                        dhparams_path));
        }
    }

  // The certificate must be set before the private key since the
  // ACE_SSL_Context attempts to check the private key for
  // consistency.  That check requires the certificate to be available
  // in the underlying SSL_CTX.
  if (certificate_path != 0)
    {
      if (ACE_SSL_Context::instance ()->certificate (certificate_path,
                                                    certificate_type) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) Unable to set ")
                        ACE_TEXT ("SSL certificate <%s> ")
                        ACE_TEXT ("in SSLIOP factory.\n"),
                        certificate_path));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("SSL certificate ")
                        ACE_TEXT ("from %s\n"),
                        certificate_path));
        }
    }

  if (private_key_path != 0)
    {
      if (ACE_SSL_Context::instance ()->private_key (private_key_path,
                                                     private_key_type) != 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) Unable to set ")
                          ACE_TEXT ("SSL private key ")
                          ACE_TEXT ("<%s> in SSLIOP factory.\n"),
                          private_key_path));
            }

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("Private Key ")
                        ACE_TEXT ("from %s\n"),
                        private_key_path));
        }
    }

  if (this->register_orb_initializer () != 0)
    return -1;

  if (prevdebug != -1)
    TAO_debug_level = prevdebug;

  return 0;
}

int
TAO_SSLIOP_Protocol_Factory::register_orb_initializer (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
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
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register the SSLIOP ORB initializer.
      // PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_SSLIOP_ORBInitializer (this->qop_),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_TRY_CHECK;

      //PortableInterceptor::ORBInitializer_var initializer = tmp;
      initializer = tmp;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
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
                  TAO_SSLIOP_Connector (this->qop_),
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

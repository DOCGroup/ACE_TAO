// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_Factory.h"
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connector.h"
#include "orbsvcs/SSLIOP/SSLIOP_ORBInitializer.h"
#include "ace/OS_NS_strings.h"

#include "orbsvcs/Security/Security_ORBInitializer.h"  /// @todo should go away

#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/DLL_Resident_ORB_Initializer.h"

#include "ace/SSL/sslconf.h"
#include "ace/SSL/SSL_Context.h"

// An SSL session id seed value. Needs not be too unique, just somewhat
// different. See the OpenSSL manual
static const unsigned char session_id_context_[] =
  "$Id$";

// Protocol name prefix
static const char * const the_prefix[] = {"iiop", "ssliop"};

// An OS-dependent path separator character
static ACE_TCHAR const TAO_PATH_SEPARATOR_STRING[] =
#if defined(ACE_WIN32)
  ACE_TEXT (";");
#else
  ACE_TEXT (":");
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    static const long ACCEPT_TIMEOUT = 10;  // Default accept timeout
                                            // in seconds.
  }
}

TAO::SSLIOP::Protocol_Factory::Protocol_Factory (void)
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP),
     qop_ (::Security::SecQOPIntegrityAndConfidentiality),
     timeout_ (TAO::SSLIOP::ACCEPT_TIMEOUT)
{
}

TAO::SSLIOP::Protocol_Factory::~Protocol_Factory (void)
{
}

int
TAO::SSLIOP::Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix[0]) == 0)
     || (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix[1]) == 0);
}

const char *
TAO::SSLIOP::Protocol_Factory::prefix (void) const
{
   // Note: This method doesn't seem to be used anywhere. Moreover,
   // keeping it may make things more confusing - a Factory can
   // well be handling multiple protocol prefixes, not just one!
   // Shouldn't it be deprecated?
  return ::the_prefix[0];
}

char
TAO::SSLIOP::Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

TAO_Acceptor *
TAO::SSLIOP::Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO::SSLIOP::Acceptor (this->qop_,
                                         this->timeout_),
                  0);

  return acceptor;
}


// Parses a X509 path. Beware: This function modifies
// the buffer pointed to by arg!
int
TAO::SSLIOP::Protocol_Factory::parse_x509_file (char *arg, char **path)
{
  ACE_ASSERT (arg != 0);
  ACE_ASSERT (path != 0);

  char *lst = 0;
  const char *type_name = ACE_OS::strtok_r (arg, ":", &lst);
  *path = ACE_OS::strtok_r (0, "", &lst);

  if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
      return SSL_FILETYPE_ASN1;

  if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
      return SSL_FILETYPE_PEM;

  return -1;
}


int
TAO::SSLIOP::Protocol_Factory::init (int argc, ACE_TCHAR* argv[])
{
  char *certificate_path = 0;
  char *private_key_path = 0;
  char *dhparams_path = 0;
  char *ca_file = 0;
  CORBA::String_var ca_dir;
  ACE_TCHAR *rand_path = 0;

  int certificate_type = -1;
  int private_key_type = -1;
  int dhparams_type = -1;

  int prevdebug = -1;

  CSIIOP::AssociationOptions csiv2_target_supports =
    CSIIOP::Integrity | CSIIOP::Confidentiality;
  CSIIOP::AssociationOptions csiv2_target_requires =
    CSIIOP::Integrity | CSIIOP::Confidentiality;

  // Force the Singleton instance to be initialized/instantiated.
  // Some SSLIOP option combinations below will result in the
  // Singleton instance never being initialized.  In that case,
  // problems may occur later on due to lack of initialization of the
  // underlying SSL library (e.g. OpenSSL), which occurs when an
  // ACE_SSL_Context is instantiated.

  // This directive processing initializes ACE_SSL_Context as well
  // as registers ACE_SSL for correct cleanup.
  ACE_Service_Config::process_directive (
    ACE_STATIC_SERVICE_DIRECTIVE ("ACE_SSL_Initializer", ""));

  // The code is cleaner this way anyway.
  ACE_SSL_Context * ssl_ctx = ACE_SSL_Context::instance ();

  size_t session_id_len =
    (sizeof session_id_context_ >= SSL_MAX_SSL_SESSION_ID_LENGTH)
      ? SSL_MAX_SSL_SESSION_ID_LENGTH
      : sizeof session_id_context_;

  // Note that this function returns 1, if the operation succeded.
  // See SSL_CTX_set_session_id_context(3)
  if( 1 != ::SSL_CTX_set_session_id_context (ssl_ctx->context(),
                                             session_id_context_,
                                             session_id_len))
  {
    if (TAO_debug_level > 0)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) Unable to set the session id ")
                  ACE_TEXT ("context to \'%C\'\n"), session_id_context_));

    return -1;
  }

  for (int curarg = 0; curarg != argc; ++curarg)
    {
      if ((ACE_OS::strcasecmp (argv[curarg],
                               ACE_TEXT("-verbose")) == 0)
          || (ACE_OS::strcasecmp (argv[curarg],
                                  ACE_TEXT("-v")) == 0))
        {
          if (TAO_debug_level == 0)
            {
              prevdebug = TAO_debug_level;
              TAO_debug_level = 1;
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLNoProtection")) == 0)
        {
          // Enable the eNULL cipher.  Note that enabling the "eNULL"
          // cipher only disables encryption.  However, certificate
          // exchanges will still occur.
          if (::SSL_CTX_set_cipher_list (ssl_ctx->context (),
                                         "ALL:eNULL") == 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) Unable to set eNULL ")
                          ACE_TEXT ("SSL cipher in SSLIOP ")
                          ACE_TEXT ("factory.\n")));

              return -1;
            }

          // This does not disable secure invocations on the server
          // side.  It merely enables insecure ones.  On the client
          // side, secure invocations will be disabled unless
          // overridden by a SecurityLevel2::QOPPolicy in the object
          // reference.
          this->qop_ = ::Security::SecQOPNoProtection;

          ACE_SET_BITS (csiv2_target_supports,
                        CSIIOP::NoProtection);

          ACE_CLR_BITS (csiv2_target_requires,
                        CSIIOP::Confidentiality);
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCertificate")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              certificate_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), &certificate_path);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLPrivateKey")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              private_key_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), &private_key_path);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLAuthenticate")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              int mode = SSL_VERIFY_NONE;
              if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT("NONE")) == 0)
                {
                  mode = SSL_VERIFY_NONE;
                }
              else if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT("SERVER")) == 0)
                {
                  mode = SSL_VERIFY_PEER;

                  ACE_SET_BITS (csiv2_target_supports,
                                CSIIOP::EstablishTrustInTarget
                                | CSIIOP::EstablishTrustInClient);
                }
              else if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT("CLIENT")) == 0
                       || ACE_OS::strcasecmp (argv[curarg],
                                              ACE_TEXT("SERVER_AND_CLIENT")) == 0)
                {
                  mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;

                  ACE_SET_BITS (csiv2_target_supports,
                                CSIIOP::EstablishTrustInTarget
                                | CSIIOP::EstablishTrustInClient);

                  ACE_SET_BITS (csiv2_target_requires,
                                CSIIOP::EstablishTrustInClient);
                }

              ssl_ctx->default_verify_mode (mode);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLAcceptTimeout")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              float timeout = 0;

              if (sscanf (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), "%f", &timeout) != 1
                  || timeout < 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "ERROR: Invalid -SSLAcceptTimeout "
                                   "value: %s.\n",
                                   argv[curarg]),
                                  -1);
              else
                this->timeout_.set (timeout);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLDHparams")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              dhparams_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), &dhparams_path);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCAfile")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              (void) parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), &ca_file);
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCApath")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ca_dir = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(argv[curarg]));
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLrand")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              rand_path = argv[curarg];
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLServerCipherOrder")) == 0)
        {
          ::SSL_CTX_set_options (ssl_ctx->context (),
                                 SSL_OP_CIPHER_SERVER_PREFERENCE);
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCipherList")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              if (::SSL_CTX_set_cipher_list (ssl_ctx->context (),
                                             argv[curarg]) == 0)
                {
                  ACE_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) Unable to set cipher ")
                              ACE_TEXT ("list in SSLIOP ")
                              ACE_TEXT ("factory.\n")));

                  return -1;
                }
            }
        }

    }

  // Load some (more) entropy from the user specified sources
  // in addition to what's pointed to by ACE_SSL_RAND_FILE_ENV
  if (rand_path != 0)
  {
    short errors = 0;
    ACE_TCHAR *file_name = 0;
    const ACE_TCHAR *path = ACE_OS::strtok_r (rand_path,
                                              TAO_PATH_SEPARATOR_STRING,
                                              &file_name);
    while ( path != 0)
    {
      if( -1 == ssl_ctx->seed_file (ACE_TEXT_ALWAYS_CHAR(path), -1))
      {
        ++errors;
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) Failed to load ")
                    ACE_TEXT ("more entropy from <%s>: %m\n"), path));
      }
      else
      {
        if (TAO_debug_level > 0)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Loaded ")
                      ACE_TEXT ("more entropy from <%s>\n"), path));
      }

      path = ACE_OS::strtok_r (0, TAO_PATH_SEPARATOR_STRING, &file_name);
    }

    if (errors > 0)
      return -1;
  }

  // Load any trusted certificates explicitely rather than relying on
  // previously set SSL_CERT_FILE and/or SSL_CERT_PATH environment variable
  if (ca_file != 0 || ca_dir.in () != 0)
    {
      if (ssl_ctx->load_trusted_ca (ca_file, ca_dir.in ()) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) Unable to load ")
                      ACE_TEXT ("CA certs from %C%C%C\n"),
                      ((ca_file != 0) ? ca_file : "a file pointed to by "
                                                  ACE_SSL_CERT_FILE_ENV
                                                  " env var (if any)"),
                      ACE_TEXT (" and "),
                      ((ca_dir.in () != 0) ?
                          ca_dir.in () : "a directory pointed to by "
                                         ACE_SSL_CERT_DIR_ENV
                                         " env var (if any)")));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("TAO (%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("Trusted Certificates from %C%C%C\n"),
                        ((ca_file != 0) ? ca_file : "a file pointed to by "
                                                    ACE_SSL_CERT_FILE_ENV
                                                    " env var (if any)"),
                        ACE_TEXT (" and "),
                        ((ca_dir.in () != 0) ?
                            ca_dir.in () : "a directory pointed to by "
                                           ACE_SSL_CERT_DIR_ENV
                                           " env var (if any)")));
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
      if (ssl_ctx->dh_params (dhparams_path,
                              dhparams_type) != 0)
        {
          if (dhparams_path != certificate_path)
            {
              // We only want to fail catastrophically if the user specified
              // a dh parameter file and we were unable to actually find it
              // and load from it.
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t) SSLIOP_Factory: ")
                          ACE_TEXT ("unable to set ")
                          ACE_TEXT ("DH parameters <%C>\n"),
                          dhparams_path));
              return -1;
            }
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_INFO,
                            ACE_TEXT ("(%P|%t) SSLIOP_Factory: ")
                            ACE_TEXT ("No DH parameters found in ")
                            ACE_TEXT ("certificate <%C>; either none ")
                            ACE_TEXT ("are needed (RSA) or problems ")
                            ACE_TEXT ("will ensue later.\n"),
                            dhparams_path));
            }
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("(%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("Diffie-Hellman params ")
                        ACE_TEXT ("from %C\n"),
                        dhparams_path));
        }
    }

  // The certificate must be set before the private key since the
  // ACE_SSL_Context attempts to check the private key for
  // consistency.  That check requires the certificate to be available
  // in the underlying SSL_CTX.
  if (certificate_path != 0)
    {
      if (ssl_ctx->certificate (certificate_path,
                                certificate_type) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) Unable to set ")
                      ACE_TEXT ("SSL certificate <%C> ")
                      ACE_TEXT ("in SSLIOP factory.\n"),
                      certificate_path));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("TAO (%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("SSL certificate ")
                        ACE_TEXT ("from %C\n"),
                        certificate_path));
        }
    }

  if (private_key_path != 0)
    {
      if (ssl_ctx->private_key (private_key_path, private_key_type) != 0)
        {

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) Unable to set ")
                      ACE_TEXT ("SSL private key ")
                      ACE_TEXT ("<%C> in SSLIOP factory.\n"),
                      private_key_path));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_INFO,
                        ACE_TEXT ("TAO (%P|%t) SSLIOP loaded ")
                        ACE_TEXT ("Private Key ")
                        ACE_TEXT ("from <%C>\n"),
                        private_key_path));
        }
    }

  if (this->register_orb_initializer (csiv2_target_supports,
                                      csiv2_target_requires) != 0)
    return -1;

  if (prevdebug != -1)
    TAO_debug_level = prevdebug;

  return 0;
}

int
TAO::SSLIOP::Protocol_Factory::register_orb_initializer (
  CSIIOP::AssociationOptions csiv2_target_supports,
  CSIIOP::AssociationOptions csiv2_target_requires)
{
  try
    {
      // @todo: This hard-coding should be fixed once SECIOP is
      // supported.
      // Register the Security ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO::Security::ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var initializer = tmp;

  PortableInterceptor::ORBInitializer_ptr temp_dll_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

  ACE_NEW_THROW_EX (temp_dll_initializer,
        PortableInterceptor::DLL_Resident_ORB_Initializer(
  initializer.in (),
  ACE_TEXT ("TAO_Security")),
  CORBA::NO_MEMORY (
  CORBA::SystemException::_tao_minor_code (
  TAO::VMCID,
  ENOMEM),
  CORBA::COMPLETED_NO));

  PortableInterceptor::ORBInitializer_var dll_initializer
  = temp_dll_initializer;

  PortableInterceptor::register_orb_initializer (dll_initializer.in ());


      // Register the SSLIOP ORB initializer.
      ACE_NEW_THROW_EX (tmp,
                        TAO::SSLIOP::ORBInitializer (this->qop_,
                                                     csiv2_target_supports,
                                                     csiv2_target_requires),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      initializer = tmp;

  temp_dll_initializer = PortableInterceptor::ORBInitializer::_nil ();

  ACE_NEW_THROW_EX (temp_dll_initializer,
        PortableInterceptor::DLL_Resident_ORB_Initializer(
  initializer.in (),
  ACE_TEXT ("TAO_SSLIOP")),
  CORBA::NO_MEMORY (
  CORBA::SystemException::_tao_minor_code (
  TAO::VMCID,
  ENOMEM),
  CORBA::COMPLETED_NO));

  dll_initializer = temp_dll_initializer;

  PortableInterceptor::register_orb_initializer (dll_initializer.in ());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unable to register SSLIOP ORB initializer.");
      return -1;
    }

  return 0;
}


TAO_Connector *
TAO::SSLIOP::Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO::SSLIOP::Connector (this->qop_),
                  0);
  return connector;
}

int
TAO::SSLIOP::Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_SSLIOP_Protocol_Factory,
                       ACE_TEXT ("SSLIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_SSLIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)

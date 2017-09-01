#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/SSLIOP/SSLIOP_Factory.h"
#include "orbsvcs/SSLIOP/SSLIOP_Acceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connector.h"
#include "orbsvcs/SSLIOP/SSLIOP_ORBInitializer.h"
#include "ace/OS_NS_strings.h"
#include "ace/Read_Buffer.h"
#include "ace/Malloc_Base.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#if defined (ACE_WIN32)
# include <conio.h>
#elif defined (ACE_HAS_TERMIOS)
# include "ace/os_include/os_termios.h"
#endif /* ACE_WIN32 */

#include "orbsvcs/Security/Security_ORBInitializer.h"  /// @todo should go away

#include "tao/debug.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/DLL_Resident_ORB_Initializer.h"

#include "ace/SSL/sslconf.h"
#include "ace/SSL/SSL_Context.h"

// An SSL session id seed value. Needs not be too unique, just somewhat
// different. See the OpenSSL manual
static const unsigned char session_id_context_[] =
  "";

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
namespace
{
  size_t
  secret_input (char *buf, size_t max)
  {
    size_t len = 0;
    char c = '\0';
#if defined (ACE_WIN32)
    if (!ACE_OS::isatty (ACE_STDIN))
      {
        len = ACE_OS::fread (buf, 1, max, stdin);
        buf[len] = 0;
        return len;
      }
#elif defined (ACE_HAS_TERMIOS)
    struct termios old_tio, new_tio;

    if (ACE_OS::isatty (ACE_STDIN))
      {
        ::tcgetattr (ACE_STDIN, &old_tio);
        new_tio = old_tio;
        new_tio.c_lflag &= (~ICANON & ~ECHO);
        ::tcsetattr (ACE_STDIN, TCSANOW, &new_tio);
      }
#endif /* ACE_WIN32 */

    for (len = 0; len < max; )
      {
#if defined (ACE_WIN32)
        c = ::_getch ();
#else
        int ci = ::getchar ();
        if (ci == EOF)
          {
            break;
          }
        c = (char)ci;
#endif /* ACE_WIN32 */
        if (c >= ' ' && c <= '~')
          {
            buf[len++] = c;
            ACE_OS::printf ("%c", '*');
          }
        else if (len > 0 && (c == 8 || c == 127))
          {
            buf[--len] = 0;
            ACE_OS::printf ("\b \b");
          }
        else if (c == '\n' || c == '\r' || c == '\0')
          {
            break;
          }
      }
    buf[len] = 0;

#if !defined (ACE_WIN32) && defined (ACE_HAS_TERMIOS)
    if (ACE_OS::isatty (ACE_STDIN))
      {
        /* restore the former settings */
        ::tcsetattr (ACE_STDIN, TCSANOW, &old_tio);
      }
#endif /* !ACE_WIN32 */
    return len;
  }
}

namespace TAO
{
  namespace SSLIOP
  {
    static const long ACCEPT_TIMEOUT = 10;  // Default accept timeout
                                            // in seconds.
  }
}

ACE_CString TAO::SSLIOP::Protocol_Factory::pem_passwd_;
const ACE_CString key_prompt("prompt:");
const ACE_CString key_file("file:");
const ACE_CString key_env("env:");

int
TAO::SSLIOP::Protocol_Factory::pem_passwd_cb (char *buf, int size, int , void *the_passwd)
{
  const char *passwd_str = reinterpret_cast<const char *>(the_passwd);
  int len = 0;
  if (the_passwd != 0)
    {
      len = ACE_Utils::truncate_cast<int> (ACE_OS::strlen (passwd_str));
    }
  else
    {
      if (pem_passwd_.length() == 0)
        {
          return 0;
        }
      if (pem_passwd_.find (key_prompt) == 0)
        {
          size_t ofs = key_prompt.length();
          // do prompt
          const char *prompt = pem_passwd_.length() == ofs ?
            "Enter password" : pem_passwd_.c_str() + ofs;
          ACE_OS::printf ("%s: ",prompt);
          pem_passwd_.resize (size);
          pem_passwd_.clear ();
          len = secret_input (buf, size);
          ACE_OS::printf ("\n");
          pem_passwd_ = buf;
          return len;
       }
      else if (pem_passwd_.find (key_file) == 0)
        {
          size_t ofs = key_file.length();
          const char *fname = pem_passwd_.c_str() + ofs;
          // do file
          FILE* file = ACE_OS::fopen (fname,ACE_TEXT("r"));

          if (file == 0)
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("TAO (%P|%t) - SSLIOP_Factory::pem_passwd_cb ")
                                ACE_TEXT ("cannot open file: %C\n"),
                                fname));
              pem_passwd_ = "";
            }
          else
            {
              ACE_Read_Buffer reader (file, true);
              char* string = reader.read ();
              if (string != 0)
                {
                  pem_passwd_ = string;
                  reader.alloc ()->free (string);
                }
              else
                {
                  if (TAO_debug_level > 0)
                    ORBSVCS_DEBUG ((LM_DEBUG,
                                    ACE_TEXT ("TAO (%P|%t) - SSLIOP_Factory::pem_passwd_cb ")
                                    ACE_TEXT ("cannot read file: %C\n"),
                                    fname));
                  pem_passwd_ = "";
                }
              ACE_OS::fclose (file);
            }
        }
      else if (pem_passwd_.find (key_env) == 0)
        {
          size_t ofs = key_env.length();
          // do env
          const char *env = pem_passwd_.length() == ofs ?
            "TAO_PEM_PASSWORD" : pem_passwd_.c_str() + ofs;
          char *pwd = ACE_OS::getenv (env);
          if (pwd != 0)
            {
              pem_passwd_ = pwd;
            }
          else
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("TAO (%P|%t) - SSLIOP_Factory::pem_passwd_cb ")
                                ACE_TEXT ("invalid env: %C\n"),
                                env));
              pem_passwd_ = "";
            }
        }

      len = ACE_Utils::truncate_cast<size_t> (pem_passwd_.length());
      if (len >= size)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("TAO (%P|%t) - SSLIOP_Factory::pem_passwd_cb truncating ")
                            ACE_TEXT ("supplied password from len %d to %d\n"),
                            len, size - 1));
          len = size - 1;
          pem_passwd_ = pem_passwd_.substr(0,len);
        }
      passwd_str = pem_passwd_.c_str ();
      ::SSL_CTX_set_default_passwd_cb_userdata (ACE_SSL_Context::instance ()->context(),
                                                (void *) passwd_str);
    }

  ACE_OS::strncpy (buf, pem_passwd_.c_str(), len);
  buf[len] = 0;
  return len;
}

TAO::SSLIOP::Protocol_Factory::Protocol_Factory (void)
  :  TAO_Protocol_Factory (IOP::TAG_INTERNET_IOP),
     qop_ (::Security::SecQOPIntegrityAndConfidentiality),
     timeout_ (TAO::SSLIOP::ACCEPT_TIMEOUT),
     check_host_ (false)
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
                                         this->timeout_,
                                         this->check_host_),
                  0);

  return acceptor;
}


// Parses a X509 path. Beware: This function modifies
// the buffer pointed to by arg!
int
TAO::SSLIOP::Protocol_Factory::parse_x509_file (char *arg, char *&path)
{
  char *lst = 0;
  const char *type_name = ACE_OS::strtok_r (arg, ":", &lst);
  path = CORBA::string_dup (ACE_OS::strtok_r (0, "", &lst));

  if (ACE_OS::strcasecmp (type_name, "ASN1") == 0)
      return SSL_FILETYPE_ASN1;

  if (ACE_OS::strcasecmp (type_name, "PEM") == 0)
      return SSL_FILETYPE_PEM;

  return -1;
}

int
TAO::SSLIOP::Protocol_Factory::init (int argc, ACE_TCHAR* argv[])
{
  CORBA::String_var certificate_path;
  CORBA::String_var private_key_path;
  CORBA::String_var dhparams_path;
  CORBA::String_var ca_file;
  CORBA::String_var ca_dir;
  ACE_TCHAR *rand_path = 0;

  int certificate_type = -1;
  int private_key_type = -1;
  int dhparams_type = -1;

  int prevdebug = -1;

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
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - Unable to set the session id ")
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
              ORBSVCS_DEBUG ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - Unable to set eNULL ")
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
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCertificate")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              certificate_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]),
                                                  certificate_path.out());
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLPrivateKey")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              private_key_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]),
                                                  private_key_path.out ());
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
                }
              else if (ACE_OS::strcasecmp (argv[curarg], ACE_TEXT("CLIENT")) == 0
                       || ACE_OS::strcasecmp (argv[curarg],
                                              ACE_TEXT("SERVER_AND_CLIENT")) == 0)
                {
                  mode = SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT;
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
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
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
              dhparams_type = parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]),
                                               dhparams_path.out());
            }
        }

      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCAfile")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              (void) parse_x509_file (ACE_TEXT_ALWAYS_CHAR(argv[curarg]), ca_file.out());
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

#if !defined (__Lynx__)
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
                                             ACE_TEXT_ALWAYS_CHAR(argv[curarg])) == 0)
                {
                  ORBSVCS_DEBUG ((LM_ERROR,
                              ACE_TEXT ("TAO (%P|%t) - Unable to set cipher ")
                              ACE_TEXT ("list in SSLIOP ")
                              ACE_TEXT ("factory.\n")));

                  return -1;
                }
            }
        }
#endif
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLVersionList")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ssl_ctx->filter_versions (ACE_TEXT_ALWAYS_CHAR(argv[curarg]));
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLPassword")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              pem_passwd_ = ACE_TEXT_ALWAYS_CHAR(argv[curarg]);
            }
        }
      else if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-SSLCheckHost")) == 0)
        {
          this->check_host_ = true;
        }

    }

  if (pem_passwd_.length() > 0)
    {
      ::SSL_CTX_set_default_passwd_cb (ssl_ctx->context(), pem_passwd_cb);
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
        ORBSVCS_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - Failed to load ")
                    ACE_TEXT ("more entropy from <%s>: %m\n"), path));
      }
      else
      {
        if (TAO_debug_level > 0)
          ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Loaded ")
                      ACE_TEXT ("more entropy from <%s>\n"), path));
      }

      path = ACE_OS::strtok_r (0, TAO_PATH_SEPARATOR_STRING, &file_name);
    }

    if (errors > 0)
      return -1;
  }

  // Load any trusted certificates explicitely rather than relying on
  // previously set SSL_CERT_FILE and/or SSL_CERT_PATH environment variable
  if (ca_file.in () != 0 || ca_dir.in () != 0)
    {
      if (ssl_ctx->load_trusted_ca (ca_file.in (), ca_dir.in ()) != 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - Unable to load ")
                          ACE_TEXT ("CA certs from %C%C%C\n"),
                          ((ca_file.in () != 0) ? ca_file.in () : "a file pointed to by "
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
            ORBSVCS_DEBUG ((LM_INFO,
                            ACE_TEXT ("TAO (%P|%t) - SSLIOP loaded ")
                            ACE_TEXT ("Trusted Certificates from %C%C%C\n"),
                            ((ca_file.in () != 0) ? ca_file.in () : "a file pointed to by "
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
  if (dhparams_path.in() == 0)
    {
      // If the user didn't explicitly specify a DH parameters file, we
      // also might find it concatenated in the certificate file.
      // So, we set the dhparams to that if it wasn't explicitly set.
      dhparams_path = certificate_path;
      dhparams_type = certificate_type;
    }

  if (dhparams_path.in() != 0)
    {
      if (ssl_ctx->dh_params (dhparams_path.in(),
                              dhparams_type) != 0)
        {
          if (ACE_OS::strcmp (dhparams_path.in(), certificate_path.in()))
            {
              // We only want to fail catastrophically if the user specified
              // a dh parameter file and we were unable to actually find it
              // and load from it.
              ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT ("(%P|%t) - SSLIOP_Factory: ")
                              ACE_TEXT ("unable to set ")
                              ACE_TEXT ("DH parameters <%C>\n"),
                              dhparams_path.in () ));
              return -1;
            }
          else
            {
              if (TAO_debug_level > 0)
                ORBSVCS_DEBUG ((LM_INFO,
                                ACE_TEXT ("(%P|%t) - SSLIOP_Factory: ")
                                ACE_TEXT ("No DH parameters found in ")
                                ACE_TEXT ("certificate <%C>; either none ")
                                ACE_TEXT ("are needed (RSA) or problems ")
                                ACE_TEXT ("will ensue later.\n"),
                                dhparams_path.in ()));
            }
        }
      else
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_INFO,
                            ACE_TEXT ("(%P|%t) - SSLIOP loaded ")
                            ACE_TEXT ("Diffie-Hellman params ")
                            ACE_TEXT ("from %C\n"),
                            dhparams_path.in ()));
        }
    }

  // The certificate must be set before the private key since the
  // ACE_SSL_Context attempts to check the private key for
  // consistency.  That check requires the certificate to be available
  // in the underlying SSL_CTX.
  if (certificate_path.in() != 0)
    {
      if (ssl_ctx->certificate (certificate_path.in(),
                                certificate_type) != 0)
        {
          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - Unable to set ")
                          ACE_TEXT ("SSL certificate <%C> ")
                          ACE_TEXT ("in SSLIOP factory.\n"),
                          certificate_path.in()));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_INFO,
                            ACE_TEXT ("TAO (%P|%t) - SSLIOP loaded ")
                            ACE_TEXT ("SSL certificate ")
                            ACE_TEXT ("from %C\n"),
                            certificate_path.in()));
        }
    }

  if (private_key_path.in() != 0)
    {
      if (ssl_ctx->private_key (private_key_path.in(), private_key_type) != 0)
        {

          ORBSVCS_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - Unable to set ")
                          ACE_TEXT ("SSL private key ")
                          ACE_TEXT ("<%C> in SSLIOP factory.\n"),
                          private_key_path.in ()));

          return -1;
        }
      else
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_INFO,
                            ACE_TEXT ("TAO (%P|%t) - SSLIOP loaded ")
                            ACE_TEXT ("Private Key ")
                            ACE_TEXT ("from <%C>\n"),
                            private_key_path.in ()));
        }
    }

  if (this->register_orb_initializer () != 0)
    return -1;

  if (prevdebug != -1)
    TAO_debug_level = prevdebug;

  return 0;
}

int
TAO::SSLIOP::Protocol_Factory::register_orb_initializer (void)
{
  try
    {
      // @todo: This hard-coding should be fixed once SECIOP is
      // supported.
      // Register the Security ORB initializer.
      PortableInterceptor::ORBInitializer_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO::Security::ORBInitializer,
                        CORBA::NO_MEMORY
                        (CORBA::SystemException::_tao_minor_code
                         (TAO::VMCID, ENOMEM), CORBA::COMPLETED_NO));
      PortableInterceptor::ORBInitializer_var initializer = tmp;

      PortableInterceptor::ORBInitializer_ptr tmp_dll =
        PortableInterceptor::ORBInitializer::_nil ();
      ACE_NEW_THROW_EX (tmp_dll,
                        PortableInterceptor::DLL_Resident_ORB_Initializer
                        (initializer.in (), ACE_TEXT ("TAO_Security")),
                        CORBA::NO_MEMORY
                        (CORBA::SystemException::_tao_minor_code
                         (TAO::VMCID, ENOMEM), CORBA::COMPLETED_NO));

      PortableInterceptor::ORBInitializer_var dll_initializer = tmp_dll;
      PortableInterceptor::register_orb_initializer (dll_initializer.in ());
      // Register the SSLIOP ORB initializer.
      ACE_NEW_THROW_EX (tmp,
                        TAO::SSLIOP::ORBInitializer (this->qop_),
                        CORBA::NO_MEMORY
                        (CORBA::SystemException::_tao_minor_code
                         (TAO::VMCID, ENOMEM), CORBA::COMPLETED_NO));

      initializer = tmp;
      tmp_dll = PortableInterceptor::ORBInitializer::_nil ();
      ACE_NEW_THROW_EX (tmp_dll,
                        PortableInterceptor::DLL_Resident_ORB_Initializer
                        (initializer.in (), ACE_TEXT ("TAO_SSLIOP")),
                        CORBA::NO_MEMORY
                        (CORBA::SystemException::_tao_minor_code
                         (TAO::VMCID, ENOMEM), CORBA::COMPLETED_NO));
      dll_initializer = tmp_dll;
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
  TAO::SSLIOP::Connector *connector = 0;

  ACE_NEW_RETURN (connector,
                  TAO::SSLIOP::Connector (this->qop_, this->check_host_),
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

// $Id$

#include "orbsvcs/HTIOP/HTIOP_Factory.h"
#include "orbsvcs/HTIOP/HTIOP_Acceptor.h"
#include "orbsvcs/HTIOP/HTIOP_Connector.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"

#include "ace/HTBP/HTBP_Environment.h"
#include "tao/IOPC.h"

static const char the_prefix[] = "htiop";

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Protocol_Factory::Protocol_Factory (void)
  :  TAO_Protocol_Factory (OCI_TAG_HTIOP_PROFILE),
     ht_env_ (0),
     inside_ (-1)
{
}

TAO::HTIOP::Protocol_Factory::~Protocol_Factory (void)
{
  delete this->ht_env_;
}

int
TAO::HTIOP::Protocol_Factory::match_prefix (const ACE_CString &prefix)
{
  // Check for the proper prefix for this protocol.
  return (ACE_OS::strcasecmp (prefix.c_str (), ::the_prefix) == 0);
}

const char *
TAO::HTIOP::Protocol_Factory::prefix (void) const
{
  return ::the_prefix;
}

char
TAO::HTIOP::Protocol_Factory::options_delimiter (void) const
{
  return '/';
}

int
TAO::HTIOP::Protocol_Factory::init (int argc,
                                  ACE_TCHAR* argv[])
{
  const ACE_TCHAR * config_file = 0;
  const ACE_TCHAR * persist_file = 0;
  unsigned proxy_port = 0;
  const ACE_TCHAR * proxy_host = 0;

  ACE_stat statbuf;
  int use_registry = 0;

  for (int i = 0; i < argc; i++)
    {
      if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-config")) == 0)
        {
          if (++i < argc)
            if (ACE_OS::stat (argv[i],&statbuf) != -1)
              config_file = argv[i];
        }
      else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-env_persist")) == 0)
        {
          if (++i < argc)
            if (ACE_OS::stat (argv[i],&statbuf) != -1)
              persist_file = argv[i];
        }
      else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-win32_reg")) == 0)
        {
          use_registry = 1;
        }
      else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-inside")) == 0)
        {
          if (++i < argc)
            this->inside_ = ACE_OS::atoi (ACE_TEXT_ALWAYS_CHAR(argv[i]));
        }
      else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-proxy_port")) == 0)
        {
          if (++i < argc)
            proxy_port = static_cast<unsigned>
              (ACE_OS::atoi (ACE_TEXT_ALWAYS_CHAR(argv[i])));
        }
      else if (ACE_OS::strcasecmp(argv[i], ACE_TEXT("-proxy_host")) == 0)
        {
          if (++i < argc)
            if (ACE_OS::stat (argv[i],&statbuf) != -1)
              proxy_host = argv[i];
        }
    }

  ACE_NEW_RETURN (this->ht_env_,
                  ACE::HTBP::Environment(0,
                                       use_registry,
                                       persist_file),
                  -1);

  if (config_file != 0)
    this->ht_env_->import_config (config_file);
  else
    {
      if (proxy_port != 0)
        this->ht_env_->set_proxy_port (proxy_port);
      if (proxy_host != 0)
        this->ht_env_->set_proxy_host (proxy_host);
    }
  return 0;
}

TAO_Acceptor *
TAO::HTIOP::Protocol_Factory::make_acceptor (void)
{
  TAO_Acceptor *acceptor = 0;

  ACE_NEW_RETURN (acceptor,
                  TAO::HTIOP::Acceptor (this->ht_env_,
                                        this->inside_),
                  0);
  return acceptor;
}

TAO_Connector *
TAO::HTIOP::Protocol_Factory::make_connector (void)
{
  TAO_Connector *connector = 0;
  ACE_NEW_RETURN (connector,
                  TAO::HTIOP::Connector (this->ht_env_),
                  0);
  return connector;
}

int
TAO::HTIOP::Protocol_Factory::requires_explicit_endpoint (void) const
{
  return 0;
}


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_HTIOP_Protocol_Factory,
                       ACE_TEXT ("HTIOP_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_HTIOP_Protocol_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)


// Since the actual class instantiated by the factory is scoped inside a
// namespace, this macro is used in place of ACE_FACTORY_DEFINE, athough
// the declaration macro is the same. Note that the second argument must
// match the name class name used in the ACE_FACTORY_DECLARE, and the
// third argument is the fully scoped class to be instantiated.
ACE_FACTORY_NAMESPACE_DEFINE (HTIOP,
                              TAO_HTIOP_Protocol_Factory,
                              TAO::HTIOP::Protocol_Factory)

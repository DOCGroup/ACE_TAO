// $Id$

#include "HTBP_Environment.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE::HTBP::Environment::Environment (ACE_Configuration *config,
                                     int using_registry,
                                     const ACE_TCHAR *persistent_file)
  : config_ (config),
    imp_exp_ (0),
    own_config_ (config != 0)
{
  initialize (using_registry,
              persistent_file);
}

ACE::HTBP::Environment::~Environment ()
{
  if (!own_config_)
    this->clear();
  else
    delete this->config_;

  delete this->imp_exp_;
}

void
ACE::HTBP::Environment::clear ()
{
  if (this->config_)
    this->config_->remove_section (config_->root_section (),
                                   ACE_TEXT("htbp"),
                                   1);
}

int
ACE::HTBP::Environment::initialize (int use_registry,
                                    const ACE_TCHAR *persistent_file)
{
  if (this->config_ == 0)
    {
      int result = -1;
      if (use_registry)
        result = this->open_registry_config();
      if (result == -1)
        result = this->open_persistent_config (persistent_file);
      if (result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT("(%P|%t) ACE::HTBP::Environment")
                             ACE_TEXT("::initialize ")
                             ACE_TEXT("Open Config failed")),
                            -1);
        }
    }


  ACE_NEW_RETURN (this->imp_exp_,
                  ACE_Ini_ImpExp (*this->config_),
                  -1);

  if (this->config_->open_section (config_->root_section (),
                                   ACE_TEXT("htbp"), 1,
                                   this->htbp_key_) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) ACE::HTBP::Environment::initialize ")
                       ACE_TEXT("Open HTBP Section failed")),
                      -1);
  return 0;
}

int
ACE::HTBP::Environment::open_registry_config ()
{
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
  HKEY root =
    ACE_Configuration_Win32Registry::resolve_key
    (HKEY_LOCAL_MACHINE,ACE_TEXT("Software\\HTBP\\Environment"));

  ACE_NEW_RETURN (this->config_,
                  ACE_Configuration_Win32Registry (root),
                  -1);
  this->own_config_ = true;
  return 0;
#else
  errno = ENOTSUP;
  return -1;
#endif /* ACE_WIN32 && !ACE_LACKS_WIN32_REGISTRY */
}

int
ACE::HTBP::Environment::open_persistent_config (const ACE_TCHAR *persistent_file)
{
  ACE_Configuration_Heap *heap;
  ACE_NEW_RETURN (heap,
                  ACE_Configuration_Heap,
                  -1);
  // do this before trying to open so it isn't leaked if the open fails.
  this->config_ = heap;
  this->own_config_ = true;
  if (persistent_file == 0)
    heap->open();
  else
    if (heap->open (persistent_file) != 0)
      ACE_ERROR_RETURN (( LM_ERROR,
                          ACE_TEXT ("(%P|%t) ACE::HTBP::Environment::")
                          ACE_TEXT ("open_config: %p\n"),
                          persistent_file),
                        -1 );
  return 0;
}

int
ACE::HTBP::Environment::get_htid_url (ACE_TString &htid_url) const
{
  return this->config_->get_string_value (this->htbp_key_,
                                          ACE_TEXT("htid_url"),
                                          htid_url);
}

int
ACE::HTBP::Environment::set_htid_url (const ACE_TCHAR *htid_generator_url)
{
  return this->config_->set_string_value (this->htbp_key_,
                                          ACE_TEXT("htid_url"),
                                          htid_generator_url);
}


int
ACE::HTBP::Environment::get_htid_via_proxy (int &via_proxy) const
{
  return this->config_->get_integer_value (this->htbp_key_,
                                           ACE_TEXT("htid_via_proxy"),
                                           (u_int &)via_proxy);
}

int
ACE::HTBP::Environment::set_htid_via_proxy (int via_proxy)
{
  return this->config_->set_integer_value (this->htbp_key_,
                                           ACE_TEXT("htid_via_proxy"),
                                           (u_int)via_proxy);
}

int
ACE::HTBP::Environment::get_proxy_host (ACE_TString &proxy_host) const
{
  return this->config_->get_string_value (this->htbp_key_,
                                          ACE_TEXT("proxy_host"),
                                          proxy_host);
}

int
ACE::HTBP::Environment::set_proxy_host (const ACE_TCHAR *proxy_host)
{
  return this->config_->set_string_value (this->htbp_key_,
                                          ACE_TEXT("proxy_host"),
                                          proxy_host);
}

int
ACE::HTBP::Environment::get_proxy_port (unsigned int &proxy_port) const
{
  int result = this->config_->get_integer_value (this->htbp_key_,
                                                 ACE_TEXT("proxy_port"),
                                                 proxy_port);
  if (result != 0)
    {
      ACE_TString port_str;
      result = this->config_->get_string_value (this->htbp_key_,
                                                ACE_TEXT("proxy_port"),
                                                port_str);
      if (result == 0)
        proxy_port = ACE_OS::strtol(port_str.c_str(),0,10);
    }
  return result;
}

int
ACE::HTBP::Environment::set_proxy_port (unsigned int proxy_port)
{
  return this->config_->set_integer_value (this->htbp_key_,
                                           ACE_TEXT("proxy_port"),
                                           proxy_port);
}

int
ACE::HTBP::Environment::import_config (const ACE_TCHAR *filename)
{
  return this->imp_exp_->import_config (filename);
}

int
ACE::HTBP::Environment::export_config (const ACE_TCHAR *filename)
{
  return this->imp_exp_->export_config (filename);
}

ACE_END_VERSIONED_NAMESPACE_DECL

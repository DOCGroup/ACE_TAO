// -*- C++ -*-
#ifndef ACE_HTBP_ENVIRONMENT_H
#define ACE_HTBP_ENVIRONMENT_H
#include /**/ "ace/pre.h"

#include "HTBP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"
#include "ace/Singleton.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTBP
  {
    /**
     * @class Environment
     *
     * @brief
     *
     * This class manages the storage of configuration data, either in a
     * flat file for Unix systems, or in the Windows registry
     */
    class HTBP_Export Environment
    {
    public:
      /// Constructor
      Environment (ACE_Configuration *config = nullptr,
                   bool using_registry = false,
                   ACE_TCHAR const* persistent_file = nullptr);

      /// Destructor
      ~Environment ();

      Environment (Environment const&) = delete;
      Environment& operator= (Environment const&) = delete;
      Environment (Environment&&) = delete;
      Environment& operator= (Environment&&) = delete;

      /// removes all htbp related values from the configuration
      void clear ();

      /// Accessors to HTID URL
      /// Returns 0 on success
      /// {@
      int get_htid_url (ACE_TString &htid_url) const;
      int set_htid_url (ACE_TCHAR const* htid_generator_url);
      /// @}

      /// Accessors to HTID via proxy - If value is true, HTID request goes
      /// through configured proxy otherwise it uses a direct connection.
      /// Returns 0 on success
      /// {@
      int get_htid_via_proxy (int &htid_via_proxy) const;
      int set_htid_via_proxy (int htid_via_proxy);
      /// @}

      /// Accessors to Proxy Address
      /// Returns 0 on success
      /// {@
      int get_proxy_host (ACE_TString &proxy_host) const;
      int set_proxy_host (ACE_TCHAR const* proxy_host);
      /// @}

      /// Returns 0 on success
      /// {@
      int get_proxy_port (unsigned int &proxy_port) const;
      int set_proxy_port (unsigned int proxy_port);
      /// @}

      /// Import the configuration from the filename
      int import_config (ACE_TCHAR const* filename);

      /// Export the configuration to the filename
      int export_config (ACE_TCHAR const* filename);

    private:
      /// Open config, set sections etc
      int initialize (bool use_registry,
                      ACE_TCHAR const* persistent_file);

      /// Open an ACE_Configuration of the appropriate type.
      int open_registry_config ();
      int open_persistent_config (ACE_TCHAR const* persistent_file);

      /// Our Proxy and HTID generator URL database
      ACE_Configuration *config_;

      /// Flat section of HTBP related items
      ACE_Configuration_Section_Key htbp_key_;

      /// the wrapper used to import or export initialization values
      ACE_Ini_ImpExp *imp_exp_;

      /// We created the config instance, so we must clean it up
      bool own_config_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_ENVIRONMENT_H */

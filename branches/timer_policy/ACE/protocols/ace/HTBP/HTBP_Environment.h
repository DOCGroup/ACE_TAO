// -*- C++ -*-
// $Id$

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
      Environment (ACE_Configuration *config = 0,
                   int using_registry = 0,
                   const ACE_TCHAR *persistent_file = 0);

      /// Destructor
      ~Environment ();

      /// removes all htbp related values from the configuration
      void clear (void);

      /// Accessors to HTID URL
      /// Returns 0 on success
      /// {@
      int get_htid_url (ACE_TString &htid_url) const;
      int set_htid_url (const ACE_TCHAR *htid_generator_url);
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
      int set_proxy_host (const ACE_TCHAR *proxy_host);
      /// @}

      /// Returns 0 on success
      /// {@
      int get_proxy_port (unsigned int &proxy_port) const;
      int set_proxy_port (unsigned int proxy_port);
      /// @}

      /// Import the configuration from the filename
      int import_config (const ACE_TCHAR *filename);

      /// Export the configuration to the filename
      int export_config (const ACE_TCHAR *filename);

    private:
      /// Open config, set sections etc
      int initialize (int use_registry,
                      const ACE_TCHAR *persistent_file);

      /// Open an ACE_Configuration of the appropriate type.
      int open_registry_config ();
      int open_persistent_config (const ACE_TCHAR *persistent_file);

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

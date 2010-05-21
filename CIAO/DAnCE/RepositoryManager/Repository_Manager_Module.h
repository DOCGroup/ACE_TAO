// -*- C++ -*-

//=============================================================================
/**
 * @file Repository_Manager_Module.h
 *
 * $Id$
 *
 * @brief  To start RepositoryManager by starter
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODE_MANAGER_MODULE_H
#define NODE_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector_T.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNamingC.h"
#include "DAnCE/DAnCE_Utility.h"

/**
 * @class Repository_Manager_Module
 *
 * This class runs the node manager instance
 */
class DAnCE_RepositoryManager_Module
{
 public:
  /// Constructor.
  DAnCE_RepositoryManager_Module (void);

  /// Destructor.
  ~DAnCE_RepositoryManager_Module (void);

  /// Overload the base class method to create a new instance
  /// of a DAnCE_RepositoryManager_Module object.
  CORBA::Object_ptr init (CORBA::ORB_ptr orb,
                          int argc,
                          ACE_TCHAR *argv []);

 private:
  struct SOptions
  {
    const ACE_TCHAR * domain_nc_;
    const ACE_TCHAR * ior_file_;
    size_t threads_;
    const ACE_TCHAR * package_dir_;
    const ACE_TCHAR * name_;
    const ACE_TCHAR * server_address_;

    bool spawn_http_;
    const ACE_TCHAR *http_port_;
    const ACE_TCHAR *http_threads_;
    const ACE_TCHAR *http_threading_;
    const ACE_TCHAR *http_io_;
    const ACE_TCHAR *http_caching_;

    SOptions() :
      domain_nc_ (0),
      ior_file_ (0),
      threads_ (1),
      package_dir_ (ACE_TEXT("RepositoryManager_Packages")),
      server_address_ (0),
      spawn_http_ (false),
      http_port_ (ACE_TEXT("5432")),
      http_threads_ (ACE_TEXT("1")),
      http_threading_ (ACE_TEXT("POOL")),
      http_io_ (ACE_TEXT("SYNCH")),
      http_caching_ (ACE_TEXT("NO_CACHE"))
    {
    }
  };

  const char * usage (void);

  bool parse_args (int argc, ACE_TCHAR *argv []);

  void create_poas (void);

  void spawn_http (void);

  CORBA::ORB_var orb_;
  CosNaming::NamingContext_var domain_nc_;

  SOptions options_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var rm_poa_;
};

#include /**/ "ace/post.h"

#endif  /* NODE_MANAGER_MODULE_H */

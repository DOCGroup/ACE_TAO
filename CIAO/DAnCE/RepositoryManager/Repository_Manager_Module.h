// -*- C++ -*-

//=============================================================================
/**
 * @file Repository_Manager_Module.h
 *
 * $Id$
 *
 * @Brief  To start RepositoryManager by starter
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODE_MANAGER_MODULE_H
#define NODE_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "DAnCE_RepositoryManager_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Vector_T.h"
#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "tao/ORB.h"
#include "tao/PortableServer/POAC.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/Object_Loader.h"
#include "Deployment/DAnCE_Loader.h"
#include "DAnCE/Deployment/Deployment_common.h"

namespace DAnCE
{
  class RepositoryManagerDaemon_i;
}

/**
 * @class Repository_Manager_Module
 *
 * @brief The shared object that is instantiated when the node manager
 *        module/library is dynamically loaded.
 *
 * This class runs the node manager instance
 */
class DAnCE_RepositoryManager_Module_Export DAnCE_RepositoryManager_Module
  : public DAnCE::DAnCE_Object_Loader
{
 public:
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
          http_port_ ("5432"),
          http_threads_ ("1"),
          http_threading_ ("POOL"),
          http_io_ ("SYNCH"),
          http_caching_ ("NO_CACHE")
        {
        }
      };

  /// Constructor.
  DAnCE_RepositoryManager_Module (void);

  /// Destructor.
  ~DAnCE_RepositoryManager_Module (void);

  /// Overload the base class method to create a new instance
  /// of a DAnCE_RepositoryManager_Module object.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  virtual const ACE_TCHAR * usage (void);

  virtual bool parse_args (int argc, ACE_TCHAR *argv []);

 private:
  void create_poas (void);
  
  void spawn_http (void);
  
  /// Here we store the servants.
  typedef ACE_Map_Manager < ACE_CString,
                            DAnCE::RepositoryManagerDaemon_i *,
                            ACE_Null_Mutex > Servant_Map;
  
  Servant_Map rm_map_;

  CORBA::ORB_var orb_;
  CosNaming::NamingContext_var domain_nc_;

  SOptions options_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var rm_poa_;
};

ACE_FACTORY_DECLARE (DAnCE_RepositoryManager_Module, DAnCE_RepositoryManager_Module)

#include /**/ "ace/post.h"

#endif  /* NODE_MANAGER_MODULE_H */

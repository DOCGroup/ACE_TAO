// -*- C++ -*-

//=============================================================================
/**
 * @file Target_Manager_Module.h
 *
 * $Id$
 *
 * @brief  To start TargetManager by starter
 *
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

#include "ace/Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "tao/ORB.h"
#include "tao/PortableServer/POAC.h"
#include "orbsvcs/CosNamingC.h"
#include "TargetManager_Impl.h"

/**
 * @class Target_Manager_Module
 *
 * This class runs the node manager instance
 */
class DAnCE_TargetManager_Module
{
 public:
  /// Constructor.
  DAnCE_TargetManager_Module (void);

  /// Destructor.
  ~DAnCE_TargetManager_Module (void);

  CORBA::Object_ptr init (CORBA::ORB_ptr orb,
                          int argc,
                          ACE_TCHAR *argv []);

 private:
  const char * usage (void);

  bool parse_args (int argc, ACE_TCHAR *argv []);

  struct SOptions
  {
    const ACE_TCHAR * domain_nc_;
    const ACE_TCHAR * ior_file_;
    const ACE_TCHAR * domain_descr_;
    const ACE_TCHAR * name_;

    SOptions() :
      domain_nc_ (0),
      ior_file_ (0),
      domain_descr_ (0),
      name_ (0)
    {
    }
  };

  void create_poas (void);

  /// Here we store the servants.
  typedef ACE_Map_Manager < ACE_CString,
                            DAnCE::TargetManagerDaemon_i *,
                            ACE_Null_Mutex > Servant_Map;

  Servant_Map rm_map_;

  CORBA::ORB_var orb_;
  CosNaming::NamingContext_var domain_nc_;

  SOptions options_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var rm_poa_;
};

#include /**/ "ace/post.h"

#endif  /* NODE_MANAGER_MODULE_H */

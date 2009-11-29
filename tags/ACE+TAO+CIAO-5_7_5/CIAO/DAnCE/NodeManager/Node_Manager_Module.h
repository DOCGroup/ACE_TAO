// -*- C++ -*-

//=============================================================================
/**
 * @file Node_Manager_Module.h
 *
 * $Id$
 *
 * @brief  To start NodeManager by starter
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef NODE_MANAGER_MODULE_H
#define NODE_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "DAnCE_NodeManager_Module_Export.h"

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
  class NodeManager_Impl;
}

/**
 * @class Node_Manager_Module
 *
 * @brief The shared object that is instantiated when the node manager
 *        module/library is dynamically loaded.
 *
 * This class runs the node manager instance
 */
class DAnCE_NodeManager_Module_Export DAnCE_NodeManager_Module
  : public DAnCE::DAnCE_Object_Loader
{
 public:
      struct SOptions
      {
        bool process_ns_;
        const ACE_TCHAR* process_ns_file_;
        bool create_plan_ns_;
        const ACE_TCHAR* create_plan_ns_ior_;
        bool rebind_plan_ns_;
        const ACE_TCHAR* rebind_plan_ns_ior_;
        bool port_indirection_;
        ACE_Vector<ACE_TString> node_managers_;
        bool ignore_failure_;
        const ACE_TCHAR * cs_path_;
        unsigned int timeout_;
        CORBA::Boolean standalone_;
        const ACE_TCHAR * server_args_;
        const ACE_TCHAR * domain_nc_;
        const ACE_TCHAR * instance_nc_;

        SOptions();
      };

  /// Constructor.
  DAnCE_NodeManager_Module (void);

  /// Destructor.
  ~DAnCE_NodeManager_Module (void);

  /// Overload the base class method to create a new instance
  /// of a DAnCE_NodeManager_Module object.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  virtual const char * usage (void);

  virtual bool parse_args (int argc, ACE_TCHAR *argv []);

 private:
  void register_value_factories (void);
  void create_poas (void);
  void create_nm_properties (DAnCE::PROPERTY_MAP &props);

  /// Here we store the servants.
  typedef ACE_Map_Manager < ACE_TString,
                            DAnCE::NodeManager_Impl *,
                            ACE_Null_Mutex > Servant_Map;
  Servant_Map nm_map_;
  CORBA::ORB_var orb_;
  CosNaming::NamingContext_var domain_nc_;

  SOptions options_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var nm_poa_;
};

ACE_FACTORY_DECLARE (DAnCE_NodeManager_Module, DAnCE_NodeManager_Module)

#include /**/ "ace/post.h"

#endif  /* NODE_MANAGER_MODULE_H */

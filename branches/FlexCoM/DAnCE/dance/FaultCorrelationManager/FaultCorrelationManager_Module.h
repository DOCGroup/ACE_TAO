// -*- C++ -*-

//=============================================================================
/**
 * @file FaultCorrelationManager_Module.h
 *
 * $Id$
 *
 * @author Friedhelm Wolf <fwolf@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef FAULTCORRELATIONMANAGER_MODULE_H
#define FAULTCORRELATIONMANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "FaultCorrelationManager_Module_export.h"

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
#include "dance/Deployment/DAnCE_Loader.h"
#include "dance/Deployment/Deployment_common.h"

namespace DAnCE {
  class FaultCorrelationManager_Impl;
}

/**
 * @class FaultCorrelationManager_Module
 *
 * @brief The shared object that is instantiated when the fault
 *        correlation manager module/library is dynamically loaded.
 *
 * This class runs the fault correlation manager instance
 */
class FaultCorrelationManager_Module_Export FaultCorrelationManager_Module
  : public DAnCE::DAnCE_Object_Loader
{
 public:
      struct SOptions
      {
        bool process_ns_;
        const char* process_ns_file_;
        bool create_plan_ns_;
        const char* create_plan_ns_ior_;
        const char * domain_nc_;
        const char * instance_nc_;
        const char * exec_mgr_ior_;
        const char * rep_mgr_ior_;
        const char * ior_file_;
        
        SOptions()
            : process_ns_ (false),
              process_ns_file_ (0),
              create_plan_ns_ (false),
              create_plan_ns_ior_ (0),
              domain_nc_ (0),
              instance_nc_ (0),
              exec_mgr_ior_ ("file://EM.ior"),
              rep_mgr_ior_ ("file://RM.ior"),
              ior_file_ ("FCM.ior")
        {
        }
      };

  /// Constructor.
  FaultCorrelationManager_Module (void);

  /// Destructor.
  ~FaultCorrelationManager_Module (void);

  /// Overload the base class method to create a new instance
  /// of a FaultCorrelationManager_Module object.
  virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                           int argc,
                                           ACE_TCHAR *argv []);

  virtual const char * usage (void);

  virtual bool parse_args (int argc, ACE_TCHAR *argv []);

 private:
  void register_value_factories (void);
  void create_poas (void);
  void create_fcm_properties (DAnCE::PROPERTY_MAP &props);

  CORBA::ORB_var orb_;
  CosNaming::NamingContext_var domain_nc_;

  SOptions options_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var fcm_poa_;

  //  FaultCorrelation_Task task_;
};

ACE_FACTORY_DECLARE (FaultCorrelationManager_Module, FaultCorrelationManager_Module)

#include /**/ "ace/post.h"

#endif  /* FAULTCORRELATIONMANAGER_MODULE_H */

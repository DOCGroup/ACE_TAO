// -*- C++ -*-

//=============================================================================
/**
 * @file ExecutionManager_Module.h
 *
 * $Id$
 *
 * @Brief
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 */
//=============================================================================

#ifndef EXECUTION_MANAGER_MODULE_H
#define EXECUTION_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "DAnCE_ExecutionManager_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Vector_T.h"
#include "tao/Object_Loader.h"

namespace DAnCE
{
  class ExecutionManager_Impl;
}

/**
 * @class Execution_Manager_Module
 *
 * @brief The shared object that is instantiated when the execution manager
 *        module/library is dynamically loaded.
 *
 * This class runs the execution manager instance
 */
class DAnCE_ExecutionManager_Module_Export DAnCE_ExecutionManager_Module
      : public TAO_Object_Loader
  {
  public:
    struct SOptions
      {
        const char* exec_mgr_file_;
        bool process_ns_;
        const char* process_ns_file_;
        bool create_plan_ns_;
        const char* create_plan_ns_ior_;
        bool rebind_plan_ns_;
        const char* rebind_plan_ns_ior_;
        bool port_indirection_;
        ACE_Vector<ACE_CString> node_managers_;
        bool ignore_failure_;
        const char *node_map_;
        const char *domain_nc_;
        
        SOptions()
            : exec_mgr_file_ (0),
              process_ns_ (false),
              process_ns_file_ (0),
              create_plan_ns_ (false),
              create_plan_ns_ior_ (0),
              rebind_plan_ns_ (false),
              rebind_plan_ns_ior_ (0),
              ignore_failure_ (false),
              node_map_(0),
              domain_nc_ (0)
        {
        }
    };

    /// Constructor.
    DAnCE_ExecutionManager_Module (void);

    /// Destructor.
    ~DAnCE_ExecutionManager_Module (void);

    /// Overload the base class method to create a new instance
    /// of a DAnCE_NodeManager_Module object.
    virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                             int argc,
                                             ACE_TCHAR *argv []);

    virtual bool parse_args (int argc, ACE_TCHAR *argv []);

    SOptions options_;
  private:
    /// Storage for ExecutionManager servant.
    DAnCE::ExecutionManager_Impl * em_impl_;
  };

ACE_FACTORY_DECLARE (DAnCE_ExecutionManager_Module, DAnCE_ExecutionManager_Module)

#include /**/ "ace/post.h"

#endif  /* EXECUTION_MANAGER_MODULE_H */


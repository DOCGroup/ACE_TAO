// -*- C++ -*-

//=============================================================================
/**
 * @file ExecutionManager_Module.h
 *
 * $Id$
 *
 * @brief
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 */
//=============================================================================

#ifndef EXECUTION_MANAGER_MODULE_H
#define EXECUTION_MANAGER_MODULE_H

#include /**/ "ace/pre.h"

#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{
  class ExecutionManager_Impl;
}

/**
 * @class Execution_Manager_Module
 *
 * This class implements the execution manager instance
 */
class DAnCE_ExecutionManager_Module
  {
  public:
    /// Constructor.
    DAnCE_ExecutionManager_Module (void);

    /// Destructor.
    ~DAnCE_ExecutionManager_Module (void);

    /// Setup our infrastructure
    CORBA::Object_ptr init (CORBA::ORB_ptr orb,
                            int argc,
                            ACE_TCHAR *argv []);

  private:
    bool parse_args (int argc, ACE_TCHAR *argv []);

    struct SOptions
      {
        const ACE_TCHAR* exec_mgr_file_;
        bool process_ns_;
        const ACE_TCHAR* process_ns_file_;
        bool create_plan_ns_;
        const ACE_TCHAR* create_plan_ns_ior_;
        bool rebind_plan_ns_;
        const ACE_TCHAR* rebind_plan_ns_ior_;
        bool port_indirection_;
        ACE_Vector<ACE_TString> node_managers_;
        bool ignore_failure_;
        const ACE_TCHAR *node_map_;
        const ACE_TCHAR *domain_nc_;
        const ACE_TCHAR *cdd_;

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
              domain_nc_ (0),
              cdd_ (0)
        {
        }
    };

    SOptions options_;

    /// Storage for ExecutionManager servant.
    DAnCE::ExecutionManager_Impl * em_impl_;
  };

#include /**/ "ace/post.h"

#endif  /* EXECUTION_MANAGER_MODULE_H */


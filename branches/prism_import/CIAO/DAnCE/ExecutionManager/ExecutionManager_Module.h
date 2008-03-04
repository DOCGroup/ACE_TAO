// -*- C++ -*-

//=============================================================================
/**
 * @file Execution_Manager_Module.h
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
    /// Constructor.
    DAnCE_ExecutionManager_Module (void);

    /// Destructor.
    ~DAnCE_ExecutionManager_Module (void);

    /// Overload the base class method to create a new instance
    /// of a DAnCE_NodeManager_Module object.
    virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                             int argc,
                                             ACE_TCHAR *argv [])
    ACE_THROW_SPEC ( (CORBA::SystemException));

  private:
    /// Storage for ExecutionManager servant.
    DAnCE::ExecutionManager_Impl * em_impl_;
  };

ACE_FACTORY_DECLARE (DAnCE_ExecutionManager_Module, DAnCE_ExecutionManager_Module)

#include /**/ "ace/post.h"

#endif  /* EXECUTION_MANAGER_MODULE_H */


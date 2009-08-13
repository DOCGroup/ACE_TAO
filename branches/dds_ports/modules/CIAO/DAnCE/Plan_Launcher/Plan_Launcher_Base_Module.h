// -*- C++ -*-

//=============================================================================
/**
 * @file Plan_Launcher_Base_Module.h
 *
 * $Id$
 *
 * @Brief  Uses to run PlanLauncher from starter
 *
 * @author Vinzenz Tornow <vt@prismtech.com>
 */
//=============================================================================

#ifndef PLAN_LAUNCHER_MODULE_H
#define PLAN_LAUNCHER_MODULE_H

#include /**/ "ace/pre.h"

#include "Plan_Launcher_Base_Module_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "tao/Object_Loader.h"


/**
 * @class Plan_Launcher_Module
 *
 * @brief The shared object that is instantiated when the plan launcher
 *        module/library is dynamically loaded.
 *
 * This class runs the plan launcher instance
 */
class DAnCE_Plan_Launcher_Base_Module_Export DAnCE_Plan_Launcher_Base_Module
      : public TAO_Object_Loader
  {
  public:
    /// Constructor.
    DAnCE_Plan_Launcher_Base_Module (void);

    /// Overload the base class method to create a new instance
    /// of a DAnCE_NodeManager_Module object.
    virtual CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                             int argc,
                                             ACE_TCHAR *argv []);

  private:
  };

ACE_FACTORY_DECLARE (DAnCE_Plan_Launcher_Base_Module, DAnCE_Plan_Launcher_Base_Module)

#include /**/ "ace/post.h"

#endif  /* PLAN_LAUNCHER_MODULE_H */

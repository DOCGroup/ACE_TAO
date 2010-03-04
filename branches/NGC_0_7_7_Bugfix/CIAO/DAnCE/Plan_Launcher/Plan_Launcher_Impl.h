/**
 * $Id$
 * @file Plan_Launcher_Impl.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * Contains the Plan_Launcher_i class, which can be used by
 * applications to launch component assemblies.
 */
#ifndef PLAN_LAUNCHER_IMPL_H
#define PLAN_LAUNCHER_IMPL_H

#include "Plan_Launcher_Base_Impl.h"

/**
* @class Plan_Launcher_Impl
* @brief This class launches and manages deployment plans.
*/
class Plan_Launcher_Impl : public Plan_Launcher_Base_Impl
  {
  public:
    Plan_Launcher_Impl (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
        : Plan_Launcher_Base_Impl (orb, argc, argv) {};

    virtual ~Plan_Launcher_Impl(void);
    
    void execute(void);

    static ::Deployment::DeploymentPlan* load_xml_plan (
      const ACE_TCHAR *deployment_plan_uri);

  protected:
    virtual void stop_plan();
  }; // class Plan_Launcher_Impl

#endif /* PLAN_LAUNCHER_IMPL_H */

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

#include "Plan_Launcher_Impl_Export.h"
#include "Plan_Launcher_Base_Impl.h"

namespace DAnCE
  {
  namespace Plan_Launcher
    {

    /**
    * @class Plan_Launcher_Impl
    * @brief This class launches and manages deployment plans.
    */
    class Plan_Launcher_Impl_Export Plan_Launcher_Impl : public Plan_Launcher_Base_Impl
      {
      public:
        Plan_Launcher_Impl (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
            : Plan_Launcher_Base_Impl (orb, argc, argv) {};

        virtual ~Plan_Launcher_Impl () {};

        void execute();
        static ::Deployment::DeploymentPlan* load_xml_plan (const char *deployment_plan_uri)
        ACE_THROW_SPEC((Deployment_Failure));

      protected:
        virtual void stop_plan();
      }; // class Plan_Launcher_Impl

  } // Plan_Launcher
} // DAnCE


#endif /* PLAN_LAUNCHER_IMPL_H */

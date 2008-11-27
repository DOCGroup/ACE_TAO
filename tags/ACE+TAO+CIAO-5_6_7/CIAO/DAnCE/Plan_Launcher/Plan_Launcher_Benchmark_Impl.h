/**
 * $Id$
 * @file Plan_Launcher_Benchmark_Impl.h
 * @author Gan Deng <dengg@dre.vanderbilt.edu>
 *
 * A subclass of the Plan_Launcher_i class, which overrides the method
 * called lauch_plan() to collect benchmarking results..
 */
#ifndef PLAN_LAUNCHER_BENCHMARK_IMPL_H
#define PLAN_LAUNCHER_BENCHMARK_IMPL_H

#include "Plan_Launcher_Impl.h"

namespace CIAO
{
  namespace Plan_Launcher
  {
    /**
     * @class Plan_Launcher_Benchmark_i
     * @brief This class is a subclass of Plan_Launcher_i which is solely for
     * benchmarking purpose.
     */
    class Plan_Launcher_Impl_Export Plan_Launcher_Benchmark_i 
      : public Plan_Launcher_i
    {
    public:
      
      virtual ~Plan_Launcher_Benchmark_i  ();

      /**
       * @brief Launch a plan, given an IDL deployment plan
       * @param plan A valid IDL deployment plan
       * @returns a string containing the UUID of the plan. Null indicates failure.
       */
      virtual const char * launch_plan (const ::Deployment::DeploymentPlan &plan);
      
    protected:

    };

  }
}

#endif /* PLAN_LAUNCHER_BENCHMARK_IMPL_H */

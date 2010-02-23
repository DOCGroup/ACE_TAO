// $Id$

/**
 * @file Split_Plan_impl.h
 *
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef DANCE_SPLIT_PLAN_H
#define DANCE_SPLIT_PLAN_H

#include "ace/Map_Manager.h"
#include "ace/SStringfwd.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Split_Plan/DAnCE_Split_Plan_export.h"

namespace DAnCE
{
  class DAnCE_Split_Plan_Export Split_Plan
  {
  public:
    typedef ACE_Map_Manager<ACE_CString, Deployment::DeploymentPlan, ACE_Null_Mutex> TNodePlans;

    Split_Plan (const Deployment::DeploymentPlan &plan,
		TNodePlans &sub_plans);

    void split_plan ();
    
  private:
    const Deployment::DeploymentPlan &plan_;
    TNodePlans &sub_plans_;
  };
}
#endif

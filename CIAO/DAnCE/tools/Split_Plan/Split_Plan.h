// $Id$

/**
 * @file Split_Plan.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_SPLIT_PLAN_H
#define DANCE_SPLIT_PLAN_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Map_Manager.h"
#include "ace/SStringfwd.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Split_Plan/DAnCE_Split_Plan_export.h"
#include "Sub_UUID_Generator.h"

namespace DAnCE
{
  template <class SPLITTER, class UUIDGEN = Unique_Sub_UUID_Generator>
  class Split_Plan
  {
  public:
    typedef SPLITTER                        TPlanSplitter;
    typedef typename TPlanSplitter::KEY     TSubPlanKey;
    typedef typename TPlanSplitter::FILTER  TSplitFilter;
    typedef UUIDGEN                         TSubUUIDGen;
    
    typedef ACE_Map_Manager<TSubPlanKey,
                            Deployment::DeploymentPlan,
                            ACE_Null_Mutex> TSubPlans;
    typedef typename TSubPlans::iterator TSubPlanIterator;

    Split_Plan ();

    void split_plan (const Deployment::DeploymentPlan &plan,
                     const TSplitFilter &filter = TSplitFilter ());

    TSubPlans & plans ();

  private:
    bool find_sub_plan (const TPlanSplitter &plan_splitter,
                        CORBA::ULong instance,
                        TSubPlanKey &sub_plan_key,
                        Deployment::DeploymentPlan &sub_plan);

    
    TSubPlans sub_plans_;
  };
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Split_Plan.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Split_Plan.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* DANCE_SPLIT_PLAN_H */

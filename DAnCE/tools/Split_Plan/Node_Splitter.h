// $Id$

/**
 * @file Node_Splitter.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_NODE_SPLITTER_H
#define DANCE_NODE_SPLITTER_H

#include "ace/SStringfwd.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Split_Plan/DAnCE_Split_Plan_export.h"

namespace DAnCE
{
  class DAnCE_Split_Plan_Export Node_Splitter
  {
  public:
    typedef ACE_CString       KEY;
    typedef ACE_CString       FILTER;

    Node_Splitter (const Deployment::DeploymentPlan &plan);

    bool    match_instance (CORBA::ULong instance,
                            const FILTER &filter) const;

    bool    match_sub_plan (CORBA::ULong instance,
                            const KEY &sub_plan_key) const;

    void    prepare_sub_plan (CORBA::ULong instance,
                              Deployment::DeploymentPlan &sub_plan,
                              KEY &sub_plan_key);

    void    finalize_sub_plan (Deployment::DeploymentPlan &sub_plan,
                               KEY &sub_plan_key);

  private:
    const Deployment::DeploymentPlan &plan_;
  };
}
#endif

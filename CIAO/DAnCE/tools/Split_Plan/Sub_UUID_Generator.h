// $Id$

/**
 * @file Sub_UUID_Generator.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef DANCE_SUB_UUID_GENERATOR_H
#define DANCE_SUB_UUID_GENERATOR_H

#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Split_Plan/DAnCE_Split_Plan_export.h"

namespace DAnCE
{
  class DAnCE_Split_Plan_Export Copy_UUID_Generator
  {
  public:
    Copy_UUID_Generator ();

    void      generate_sub_uuid (const Deployment::DeploymentPlan &parent_plan,
                                 Deployment::DeploymentPlan &sub_plan);
  };

  class DAnCE_Split_Plan_Export Unique_Sub_UUID_Generator
  {
  public:
    Unique_Sub_UUID_Generator ();

    void      generate_sub_uuid (const Deployment::DeploymentPlan &parent_plan,
                                 Deployment::DeploymentPlan &sub_plan);
  };

}
#endif

// $Id$

/**
 * @file Convert_Plan_Impl.h
 *
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */

#ifndef DANCE_CONVERT_PLAN_H
#define DANCE_CONVERT_PLAN_H

#include "Convert_Plan/DAnCE_Convert_Plan_export.h"
#include "Deployment/Deployment_DeploymentPlanC.h"

namespace DAnCE
{
  class DAnCE_Convert_Plan_Export Convert_Plan
  {
  public:
    static Deployment::DeploymentPlan * read_xml_plan (const char *filename);
    static Deployment::DeploymentPlan * read_cdr_plan (const char *filename);
    static bool write_cdr_plan (const char *filename, Deployment::DeploymentPlan &plan);
  };
}

#endif

/**
 * @file CIAO_Handler_Common.h
 * @author William R. Otte
 *
 * Operations common to all CIAO handlers.
 * $Id$
 */
#ifndef CIAO_HANDLER_COMMON_H
#define CIAO_HANDLER_COMMON_H

#include <map>
#include <string>

#include "ccm/CCM_StandardConfiguratorC.h"
#include "dance/Deployment/Deployment_DeploymentPlanC.h"
#include "dance/DAnCE_Utility.h"

namespace CIAO
{
  class Deployment_Common
  {
  public:
    // Temporary hack until Martin's artifact installation work can
    // be implemented.
    static const char * get_implementation (const char *name,
                                            const ::Deployment::DeploymentPlan &plan);

    static void create_attribute_configuration (const ::Deployment::Properties &props,
                                                ::Components::ConfigValues & values);
    struct Instance_Info
    {
      std::string name;
      ::DAnCE::Utility::PMAP_PTR pmap;
    };

    typedef std::map < std::string, Instance_Info > INSTANCES;

  };
}

#endif

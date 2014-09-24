// $Id$
/**
 * @file Plugin_Conf.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Handles installation of plugins from a configuration file.
 */

#ifndef PLUGIN_CONF_H
#define PLUGIN_CONF_H

#include "dance/Deployment/Deployment_DeploymentPlanC.h"
#include "dance/LocalityManager/Handler/DAnCE_Locality_Handler_export.h"

namespace DAnCE
{
  class DAnCE_Locality_Handler_Export Plugin_Configurator
  {
  public:
    void load_from_text_file (const ACE_TCHAR *file);

  private:
    void deploy_plan (::Deployment::DeploymentPlan &plan);

    void create_entry (const char *artifact,
                       const char *entrypoint,
                       const char *type,
                       int open_mode,
                       ::Deployment::DeploymentPlan &plan,
                       CORBA::ULong pos);
  };
}
#endif

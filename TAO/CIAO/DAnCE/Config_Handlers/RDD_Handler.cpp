// $Id$
#include "RDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
#include "Any_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    RDD_Handler::resource_deployment_descr (
        const ResourceDeploymentDescription &src,
        ::Deployment::ResourceDeploymentDescription &dest)
    {
      dest.requirementName =
        src.requirementName ().c_str ();

      dest.resourceName=
        src.resourceName ().c_str ();

      Any_Handler::extract_into_any (src.resourceValue (),
                                     dest.resourceValue);

    }

  }

}

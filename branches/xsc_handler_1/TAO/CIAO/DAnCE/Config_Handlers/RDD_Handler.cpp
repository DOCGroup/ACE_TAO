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

    ResourceDeploymentDescription
    RDD_Handler::resource_deployment_descr (
      const ::Deployment::ResourceDeploymentDescription &src)
    {
      XMLSchema::string< char > reqname ((src.requirementName));
      XMLSchema::string< char > resname ((src.resourceName));
      Any resval (Any_Handler::get_any (src.resourceValue));

      ResourceDeploymentDescription rdd (reqname,resname,resval);
      return rdd;
    }


  }

}

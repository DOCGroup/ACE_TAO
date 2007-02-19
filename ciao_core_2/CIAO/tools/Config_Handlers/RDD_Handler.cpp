// $Id$

#include "RDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Any_Handler.h"
#include "ciao/CIAO_common.h"
#include "Property_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    RDD_Handler::resource_deployment_descr (
                                            const ResourceDeploymentDescription &src,
                                            ::Deployment::ResourceDeploymentDescription &dest)
    {
      CIAO_TRACE("RDD_Handler::resource_deployment_descr");

      dest.requirementName =
        src.requirementName ().c_str ();

      dest.resourceName=
        src.resourceName ().c_str ();

      std::for_each (src.begin_property (),
                     src.end_property (),
                     Property_Functor (dest.property));

    }

    ResourceDeploymentDescription
    RDD_Handler::resource_deployment_descr (
                                            const ::Deployment::ResourceDeploymentDescription &src)
    {
      CIAO_TRACE("RDD_Handler::resource_deployment_descr - reverse");

      XMLSchema::string< char > reqname ((src.requirementName));
      XMLSchema::string< char > resname ((src.resourceName.in ()));

      ResourceDeploymentDescription rdd (reqname,resname);

      for (CORBA::ULong i = 0;
           i != src.property.length ();
           ++i)
        {
          rdd.add_property (Property_Handler::get_property (src.property[i]));
        }

      return rdd;
    }


  }

}

// $Id$

#include "PCD_Handler.h"
#include "Req_Handler.h"
#include "CEPE_Handler.h"
#include "PSPE_Handler.h"
#include "ERE_Handler.h"
#include "CRDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  namespace Config_Handlers
  {

    PCD_Handler::PCD_Handler (void)
    {
    }

    PCD_Handler::~PCD_Handler (void)
    {
    }

    ///This method takes a <Deployment::PlanConnectionDescription>
    ///and maps the values from the passed in XSC
    ///PlanConnectionDescription to its members.
    void PCD_Handler::handle_PlanConnectionDescription (const PlanConnectionDescription& desc,
                                                     Deployment::PlanConnectionDescription& toconfig)
    {
      DANCE_TRACE("PCD_Handler::get_PlanConnectionDescription");

      toconfig.name =( CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ())));

      //Source is mapped to a string in the schema and a sequence
      //in the IDL. We just assign the source string from the xml
      //to the first position in the IDL type's sequence. We
      //make sure that the source is present before lengthening
      //the sequence.
      if (desc.source_p ())
        {
          // Only one.....
          toconfig.source.length (1);
          toconfig.source[0] = ACE_TEXT_ALWAYS_CHAR ( desc.source ().c_str ());
        }

      toconfig.deployRequirement.length (desc.count_deployRequirement ());
      std::for_each (desc.begin_deployRequirement (),
                     desc.end_deployRequirement (),
                     Requirement_Functor (toconfig.deployRequirement));


      //Create the ComponentExternalPortEndpoint handler.
      CEPE_Handler::external_port_endpoints (desc,
                                             toconfig.externalEndpoint);

      //Configure the PlanSubcomponentPortEndpoint's.
      PSPE_Handler::sub_component_port_endpoints (desc,
                                                  toconfig.internalEndpoint);

      //Configure the ExternalReferenceEndpoint's.
      ERE_Handler::external_ref_endpoints (desc,
                                           toconfig.externalReference);

      //Configure the resource value.
      CRDD_Handler crddhandler;
      CORBA::ULong pos = 0;
      toconfig.deployedResource.length (desc.count_deployedResource ());
      for(PlanConnectionDescription::deployedResource_const_iterator res =
            desc.begin_deployedResource();
          res != desc.end_deployedResource();
          res++)
        {
          crddhandler.get_ConnectionResourceDeploymentDescription (toconfig.deployedResource[pos++],
                                                                   *(*res));
        }

    }

    PlanConnectionDescription
    PCD_Handler::get_PlanConnectionDescription (const Deployment::PlanConnectionDescription &src)
    {
      DANCE_TRACE("PCD_Handler::get_PlanConnectionDescription");

      XMLSchema::string< ACE_TCHAR > name (ACE_TEXT_CHAR_TO_TCHAR (src.name));

      PlanConnectionDescription pcd(name);

      //Get the source if it exists
      if(src.source.length() != 0)
        {
          XMLSchema::string< ACE_TCHAR > source(ACE_TEXT_CHAR_TO_TCHAR (src.source[0]));
          pcd.source(source);
        }

      //Get any externalEndpoint(s) and store them
      size_t total = src.externalEndpoint.length();
      for(size_t i = 0; i < total; i++)
        {
          //pcd.add_externalEndpoint(CEPE_Handler::external_port_endpoint(src.externalEndpoint[i]));
        }

      //Get any externalReference(s) and store them
      total = src.externalReference.length();
      for(size_t j = 0; j < total; j++)
        {
          //pcd.add_externalReference(ERE_Handler::external_ref_endpoint(src.externalReference[j]));
        }

      //Get any internalEndpoint(s) and store them
      total = src.internalEndpoint.length();
      for(size_t k = 0; k < total; k++)
        {
          //pcd.add_internalEndpoint(PSPE_Handler::sub_component_port_endpoint(src.internalEndpoint[k]));
        }

      //Get any deployedResource(s) and store them
      total = src.deployedResource.length();
      for(size_t l = 0; l < total; l++)
        {
          //pcd.add_deployedResource(CRDD_Handler::connection_resource_depl_desc(src.deployedResource[l]));
        }

      //Get any deployRequirement(s) and store them
      total = src.deployRequirement.length();
      for(size_t m = 0; m < total; m++)
        {
          //pcd.add_deployRequirement(Req_Handler::get_requirement(src.deployRequirement[m]));
        }

      return pcd;
    }
  }
}

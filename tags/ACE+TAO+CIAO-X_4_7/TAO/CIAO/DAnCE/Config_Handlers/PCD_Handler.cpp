// $Id$

#include "PCD_Handler.h"
#include "Req_Handler.h"
#include "CEPE_Handler.h"
#include "PSPE_Handler.h"
#include "ERE_Handler.h"
#include "CRDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
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
    void PCD_Handler::get_PlanConnectionDescription (
                Deployment::PlanConnectionDescription& toconfig,
                PlanConnectionDescription& desc)
    {
      toconfig.name = CORBA::string_dup (desc.name ().c_str ());

      //Source is mapped to a string in the schema and a sequence
      //in the IDL. We just assign the source string from the xml
      //to the first position in the IDL type's sequence. We
      //make sure that the source is present before lengthening
      //the sequence.
      if (desc.source_p ())
        {
          toconfig.source.length (toconfig.source.length () + 1);
          toconfig.source[toconfig.source.length () - 1] =
            CORBA::string_dup (desc.source ().c_str ());
        }


      if (desc.deployRequirement_p ())
        {
#if 0
          // @@ MAJO:
          //Create the handler for the requirements.
          Requirement_Handler reqhandler;

          //Increase the sequence length and delgate
          //the Requirement to the Req_Handler.
          toconfig.deployRequirement.length (
            toconfig.deployRequirement.length () + 1);
          reqhandler.get_Requirement (
            toconfig.deployRequirement[toconfig.deployRequirement.length () - 1],
            desc.deployRequirement ());
#endif /*if 0*/
        }

      //Create the ComponentExternalPortEndpoint handler.
      CEPE_Handler cepehandler;

      //Iterate through and configure each port in the
      //externalEndpoint sequence.
      for (PlanConnectionDescription::externalEndpoint_iterator
           port (desc.begin_externalEndpoint ());
           port != desc.end_externalEndpoint ();
           ++port)
        {
          toconfig.externalEndpoint.length (
            toconfig.externalEndpoint.length () + 1);

          cepehandler.get_ComponentExternalPortEndpoint (
            toconfig.externalEndpoint [toconfig.externalEndpoint.length () - 1],
            *port);
        }

      //Configure the PlanSubcomponentPortEndpoint's.
      PSPE_Handler pspehandler;

      for (PlanConnectionDescription::internalEndpoint_iterator
           ipoint (desc.begin_internalEndpoint ());
           ipoint != desc.end_internalEndpoint ();
           ++ipoint)
        {
          toconfig.internalEndpoint.length (
            toconfig.internalEndpoint.length () + 1);

          pspehandler.get_PlanSubcomponentPortEndpoint (
            toconfig.internalEndpoint [toconfig.internalEndpoint.length () - 1],
            *ipoint);
        }

      //Configure the ExternalReferenceEndpoint's.
      ERE_Handler erehandler;

      for (PlanConnectionDescription::externalReference_iterator
           ipoint (desc.begin_externalReference ());
           ipoint != desc.end_externalReference ();
           ++ipoint)
        {
          toconfig.externalReference.length (
            toconfig.externalReference.length () + 1);

          erehandler.get_ExternalReferenceEndpoint (
            toconfig.externalReference [toconfig.externalReference.length () - 1],
            *ipoint);
        }

      //Configure the resource value.
      if (desc.deployedResource_p ())
        {
          CRDD_Handler crddhandler;

          toconfig.deployedResource.length (
            toconfig.deployedResource.length () + 1);

          crddhandler.get_ConnectionResourceDeploymentDescription (
            toconfig.deployedResource[toconfig.deployedResource.length () - 1],
            desc.deployedResource ());
        }

    }

  }
}

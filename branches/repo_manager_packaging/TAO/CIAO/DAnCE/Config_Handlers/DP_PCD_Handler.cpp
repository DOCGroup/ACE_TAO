// $Id$

#include "DP_PCD_Handler.h"
#include "CEPE_Handler.h"
#include "PSPE_Handler.h"
#include "ERE_Handler.h"
#include "CRDD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "cdp.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    DP_PCD_Handler::plan_connection_descrs (
        const DeploymentPlan &src,
        Deployment::PlanConnectionDescriptions& dest)
    {
      DeploymentPlan::connection_const_iterator cci_e =
        src.end_connection ();

      for (DeploymentPlan::connection_const_iterator cci_b =
             src.begin_connection ();
           cci_b != cci_e;
           ++cci_b)
        {
          CORBA::ULong len =
            dest.length ();

          dest.length (len + 1);

          bool retval =
            DP_PCD_Handler::plan_connection_descr (
              *(cci_b),
              dest[len]);

          if (!retval)
            return retval;
        }

      return true;
    }

    bool
    DP_PCD_Handler::plan_connection_descr (
        const PlanConnectionDescription &src,
        Deployment::PlanConnectionDescription &dest)
    {
      dest.name =
        src.name ().c_str ();

      if (src.source_p ())
        {
          // There will be only one as per the schema
          dest.source.length (1);
          dest.source[0] =
            src.source ().c_str ();
        }


      if (!CEPE_Handler::external_port_endpoints (src,
                                                  dest.externalEndpoint))
        return false;

      if (!ERE_Handler::external_ref_endpoints (src,
                                                dest.externalReference))
        return false;

      if (!PSPE_Handler::sub_component_port_endpoints (src,
                                                       dest.internalEndpoint))
        return false;

#if 0
      // @@MAJO: Need to figure how to use this.
      if (desc.deployedResource_p ())
        {
          CRDD_Handler crddhandler;

          toconfig.deployedResource.length (
            toconfig.deployedResource.length () + 1);

          crddhandler.get_ConnectionResourceDeploymentDescription (
            toconfig.deployedResource[toconfig.deployedResource.length () - 1],
            desc.deployedResource ());
        }

      if (desc.deployRequirement_p ())
        {

          // @@ MAJO: Not sure how to use this
          //Create the handler for the requirements.
          Requirement_Handler reqhandler;

          //Increase the sequence length and delgate
          //the Requirement to the Req_Handler.
          toconfig.deployRequirement.length (
            toconfig.deployRequirement.length () + 1);
          reqhandler.get_Requirement (
            toconfig.deployRequirement[toconfig.deployRequirement.length () - 1],
            desc.deployRequirement ());


          return 0;
        }
#endif /*if 0*/

      return true;
    }
  }
}

// $Id$

#include "PSPE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    bool
    PSPE_Handler::sub_component_port_endpoints (
        const PlanConnectionDescription &src,
        ::Deployment::PlanSubcomponentPortEndpoints &dest)
    {
      PlanConnectionDescription::internalEndpoint_const_iterator iei_e =
        src.end_internalEndpoint ();
      for (PlanConnectionDescription::internalEndpoint_const_iterator iei_b =
             src.begin_internalEndpoint ();
           iei_b != iei_e;
           ++iei_b)
        {
          CORBA::ULong len =
            dest.length ();

          dest.length (len + 1);

          (void) PSPE_Handler::sub_component_port_endpoint (
              (*iei_b),
              dest[len]);
        }
      return true;
    }

    void
    PSPE_Handler::sub_component_port_endpoint (
        const PlanSubcomponentPortEndpoint &src,
        ::Deployment::PlanSubcomponentPortEndpoint &dest)
    {
      dest.portName =
        src.portName ().c_str ();

      if (src.provider_p ())
        {
          dest.provider = !(src.provider ().empty ());
        }
      else
        {
          dest.provider = 0;
        }
    }
  }
}

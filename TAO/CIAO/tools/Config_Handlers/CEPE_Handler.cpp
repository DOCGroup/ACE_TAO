// $Id$

#include "CEPE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    void
    CEPE_Handler::external_port_endpoints (
        const PlanConnectionDescription &src,
        ::Deployment::ComponentExternalPortEndpoints &dest)
    {
      PlanConnectionDescription::externalEndpoint_const_iterator eeci_e =
        src.end_externalEndpoint ();

      CORBA::ULong pos = 0;
      dest.length (src.count_externalEndpoint ());
      for (PlanConnectionDescription::externalEndpoint_const_iterator eeci_b =
             src.begin_externalEndpoint ();
           eeci_b != eeci_e;
           ++eeci_b)
        {
          CEPE_Handler::external_port_endpoint ((*eeci_b),
                                                dest[pos++]);
        }
    }

    void
    CEPE_Handler::external_port_endpoint (
        const ComponentExternalPortEndpoint &src,
        ::Deployment::ComponentExternalPortEndpoint &dest)
    {
      dest.portName = src.portName ().c_str ();
    }
    
    ComponentExternalPortEndpoint
    CEPE_Handler::external_port_endpoint (
      const ::Deployment::ComponentExternalPortEndpoint &src)
    {
      //MAJO Unfinished
      XMLSchema::string< char > portname ((src.portName));
      ComponentExternalPortEndpoint cepe (portname);
      return cepe;
    }
  }
}

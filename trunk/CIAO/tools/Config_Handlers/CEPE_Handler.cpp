// $Id$

#include "CEPE_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
namespace CIAO
{
  namespace Config_Handlers
  {
    void
    CEPE_Handler::external_port_endpoints (
                                           const PlanConnectionDescription &src,
                                           ::Deployment::ComponentExternalPortEndpoints &dest)
    {
      CIAO_TRACE("CEOE_Handler::external_port_endpoints");
      PlanConnectionDescription::externalEndpoint_const_iterator eeci_e =
        src.end_externalEndpoint ();

      CORBA::ULong pos = 0;
      dest.length (src.count_externalEndpoint ());
      for (PlanConnectionDescription::externalEndpoint_const_iterator eeci_b =
             src.begin_externalEndpoint ();
           eeci_b != eeci_e;
           ++eeci_b)
        {
          CEPE_Handler::handle_external_port_endpoint (*(*eeci_b),
                                                       dest[pos++]);
        }
    }

    void
    CEPE_Handler::handle_external_port_endpoint (
                                          const ComponentExternalPortEndpoint &src,
                                          ::Deployment::ComponentExternalPortEndpoint &dest)
    {
      CIAO_TRACE("CEOE_Handler::external_port_endpoint");
      dest.portName = src.portName ().c_str ();
    }

    ComponentExternalPortEndpoint
    CEPE_Handler::external_port_endpoint (
                                          const ::Deployment::ComponentExternalPortEndpoint &src)
    {
      CIAO_TRACE("CEOE_Handler::external_port_endpoint - reverse");
      //MAJO Unfinished
      XMLSchema::string< char > portname ((src.portName));
      return ComponentExternalPortEndpoint  (portname);
    }
  }
}

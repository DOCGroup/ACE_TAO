// $Id$
#include "CPD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Deployment_DataC.h"
namespace CIAO
{
  namespace Config_Handlers
  {
    void
    CPD_Handler::component_port_description (
      const ComponentPortDescription& desc,
      Deployment::ComponentPortDescription& toconfig)
    {
      toconfig.name =
        desc.name ().c_str ();

      toconfig.specificType =
        desc.specificType ().c_str ();

      if (!desc.supportedType ().empty ())
        {
          toconfig.supportedType.length (1);
          toconfig.supportedType[0] =
            desc.supportedType ().c_str ();
        }

      toconfig.provider = !desc.provider ().empty ();
      toconfig.exclusiveProvider = !desc.exclusiveProvider ().empty ();
      toconfig.exclusiveUser = !desc.exclusiveUser ().empty ();
      toconfig.optional =  !desc.optional ().empty ();
    }
  }
}

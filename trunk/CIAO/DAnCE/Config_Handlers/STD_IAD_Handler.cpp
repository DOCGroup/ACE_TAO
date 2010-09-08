// $Id$
#include "STD_IAD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Packaging_DataC.h"
#include "Property_Handler.h"

#include "iad.hpp"

namespace DAnCE
{
  namespace Config_Handlers
  {
    bool
    STD_IAD_Handler::impl_artifact_descr (
      const ImplementationArtifactDescription &desc,
      ::Deployment::ImplementationArtifactDescription &toconfig)
    {
      toconfig.label =
        desc.label ().c_str ();

      toconfig.UUID =
        desc.UUID ().c_str ();

      for (ImplementationArtifactDescription::location_const_iterator iter =
             desc.begin_location ();
           iter != desc.end_location ();
           iter++)
      {
        CORBA::ULong len =
          toconfig.location.length ();
        toconfig.location.length (len + 1);
        toconfig.location [len] = (*iter).c_str ();
      }

      if (desc.execParameter_p ())
      {
        Property p = desc.execParameter ();
        Deployment::Property idl_p;
        Property_Handler::get_property (p, idl_p);
        toconfig.execParameter.length (1);
        toconfig.execParameter [0] = idl_p;
      }
     return true;
    }
  }
}

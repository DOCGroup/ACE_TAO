//$Id$

#include "SatisfierProperty_Handler.h"
#include "Any_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"
#include "Utils/Exceptions.h"
namespace CIAO
{
  namespace Config_Handlers
  {

    SatisfierProperty_Handler::SatisfierProperty_Handler (void)
    {
    }

    SatisfierProperty_Handler::~SatisfierProperty_Handler (void)
    {
    }

    void
    SatisfierProperty_Handler::handle_sat_property (
                                                 const SatisfierProperty& desc,
                                                 Deployment::SatisfierProperty& toconfig)
    {
      CIAO_TRACE("SatisfierProperty_Handler::get_sat_property");

      toconfig.name = desc.name ().c_str ();

      switch (desc.kind ().integral ())
        {
        case SatisfierPropertyKind::Quantity_l:
          toconfig.kind = Deployment::Quantity;
          break;

        case SatisfierPropertyKind::Capacity_l:
          toconfig.kind = Deployment::Capacity;
          break;

        case SatisfierPropertyKind::Minimum_l:
          toconfig.kind = Deployment::Minimum;
          break;

        case SatisfierPropertyKind::Maximum_l:
          toconfig.kind = Deployment::Maximum;
          break;

          //        case SatisfierPropertyKind::Attribute_l:
          //toconfig.kind = Deployment::_Attribute;
          //break;

        case SatisfierPropertyKind::Selection_l:
          toconfig.kind = Deployment::Selection;
          break;
          
        default:
          ACE_ERROR ((LM_ERROR, "Unknown SatisfierPropertyKind\n"));
          throw 1;
          
        }

      toconfig.dynamic = desc.dynamic ();

      Any_Handler::extract_into_any (desc.value (),
                                     toconfig.value);
    }

    SatisfierProperty
    SatisfierProperty_Handler::get_sat_property (
                                                 const Deployment::SatisfierProperty& src)
    {
      CIAO_TRACE("SatisfierProperty_Handler::get_sat_property - reverse");

      ::XMLSchema::string< ACE_TCHAR > name ((src.name));
      SatisfierPropertyKind::Value kind;

      const SatisfierPropertyKind *spk = 0;

      switch (src.kind)
        {
        case Deployment::Quantity:
          spk = &SatisfierPropertyKind::Quantity;
          break;

        case Deployment::Capacity:
          spk = &SatisfierPropertyKind::Capacity;
          break;

        case Deployment::Minimum:
          spk = &SatisfierPropertyKind::Minimum;
          break;

        case Deployment::Maximum:
          spk = &SatisfierPropertyKind::Maximum;
          break;

          //        case Deployment::_Attribute:
          //kind = SatisfierPropertyKind::Attribute_l;
          //break;

        case Deployment::Selection:
          kind = SatisfierPropertyKind::Selection_l;
          break;

        default:
          throw Plan_Error ("Invalid SatisfierPropertyKind");
        }

      bool dynamic = src.dynamic;

      Any value (Any_Handler::get_any (src.value));

      return SatisfierProperty (name, *spk, dynamic, value);
    }

  }
}

// $Id$
#include "Package_Handlers/IAD_Handler.h"
#include "Package_Handlers/NIA_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "Property_Handler.h"
#include "Req_Handler.h"
#include "Utils/Functors.h"
#include "Utils/Exceptions.h"
#include "XML_Typedefs.h"

#include "Deployment.hpp"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    namespace Packaging
    {

      void
      IAD_Handler::impl_artifact_descr (const ImplementationArtifactDescription &desc,
                                        ::Deployment::ImplementationArtifactDescription &toconfig)
      {
        DANCE_TRACE ("IAD_Handler::impl_artifact_descr");

        const ImplementationArtifactDescription *iad = 0;
        std::auto_ptr <ImplementationArtifactDescription> safe_iad;

        if (desc.href_p ())
          {
            safe_iad.reset (IAD_Handler::resolve_iad (desc.href ().c_str ()));
            iad = safe_iad.get ();
          }
        else
          iad = &desc;


        if (iad->label_p ())
          toconfig.label = ACE_TEXT_ALWAYS_CHAR ( iad->label ().c_str ());

        if (iad->UUID_p ())
          toconfig.UUID = ACE_TEXT_ALWAYS_CHAR (iad->UUID ().c_str ());

        toconfig.location.length (iad->count_location ());
        std::for_each (iad->begin_location (),
                       iad->end_location (),
                       String_Seq_Functor (toconfig.location));

        toconfig.dependsOn.length (iad->count_dependsOn ());
        SEQ_HAND_GCC_BUG_WORKAROUND (NIA_Handler::handle_nia,
                                     (*desc.begin_dependsOn ()),
                                     toconfig.dependsOn);
        std::for_each (iad->begin_dependsOn (),
                       iad->end_dependsOn (),
                       NIA_Functor (toconfig.dependsOn));

        toconfig.execParameter.length (iad->count_execParameter ());
        std::for_each (iad->begin_execParameter (),
                       iad->end_execParameter (),
                       Property_Functor (toconfig.execParameter));

        toconfig.infoProperty.length (iad->count_infoProperty ());
        std::for_each (iad->begin_infoProperty (),
                       iad->end_infoProperty (),
                       Property_Functor (toconfig.infoProperty));

        toconfig.deployRequirement.length (iad->count_deployRequirement ());
        std::for_each (iad->begin_deployRequirement (),
                       iad->end_deployRequirement (),
                       Requirement_Functor (toconfig.deployRequirement ));
      }

      ImplementationArtifactDescription
      IAD_Handler::impl_artifact_descr (const ::Deployment::ImplementationArtifactDescription &src)
      {
        DANCE_TRACE ("IAD_Handler::impl_artifact_descr - reverse");
        ImplementationArtifactDescription retval;

        retval.label (ACE_TEXT_CHAR_TO_TCHAR (src.label.in ()));
        retval.UUID (ACE_TEXT_CHAR_TO_TCHAR  (src.UUID.in ()));
#if 0
        for (CORBA::ULong i = 0; i < src.location.length (); ++i)
          retval.add_location (src.location[i].in ());

        for (CORBA::ULong i = 0; i < src.dependsOn.length (); ++i)
          retval.add_dependsOn (NIA_Handler::get_nia (src.dependsOn[i]));

        for (CORBA::ULong i = 0; i < src.execParameter.length (); ++i)
          retval.add_execParameter
            (Property_Handler::get_property (src.execParameter[i]));

        for (CORBA::ULong i = 0; i < src.infoProperty.length (); ++i)
          retval.add_infoProperty
            (Property_Handler::get_property ( src.infoProperty[i]));

        for (CORBA::ULong i = 0; i < src.deployRequirement.length (); ++i)
          retval.add_deployRequirement
            (Req_Handler::get_requirement (src.deployRequirement[i]));
#endif
        return retval;
      }

      ImplementationArtifactDescription *  IAD_Handler::resolve_iad (const ACE_TCHAR *uri)
      {
        DANCE_TRACE ("IAD_Handler::resolve_iad");

        xercesc::DOMDocument *dom = XML_Helper::XML_HELPER.create_dom (uri);

        if (!dom)
          throw Parse_Error (ACE_TEXT ("Unable to create DOM for IAD"));

        try {
          return new ImplementationArtifactDescription
            (reader::implementationArtifactDescription (dom));
        }
        catch (...) {
          throw Parse_Error (ACE_TEXT ("Unable to create XSC structure for IAD"));
        }
      }

    }
  }
}

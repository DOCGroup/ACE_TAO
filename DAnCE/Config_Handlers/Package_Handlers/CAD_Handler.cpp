// $Id$

#include "tao/AnyTypeCode/TypeCode.h"
#include "Package_Handlers/CAD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "cid.hpp"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "Req_Handler.h"
#include "CEPE_Handler.h"
#include "ERE_Handler.h"
#include "Utils/Exceptions.h"

#include "Package_Handlers/SID_Handler.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    namespace Packaging
    {
      struct Packaging_Handlers_Export SPE_Handler
      {
    static void handle_spe (const SubcomponentPortEndpoint &desc,
                ::Deployment::SubcomponentPortEndpoint &toconfig)
    {
      DANCE_TRACE("SPE_Handler::get_spe");

      toconfig.portName = ACE_TEXT_ALWAYS_CHAR ( desc.portName ().c_str ());

      ACE_TString str (desc.instance ().idref ().id ().c_str ());
      CORBA::ULong pos (0);

      SID_Handler::IDREF.find_ref (str, pos);
      toconfig.instanceRef = pos;
    }

    static SubcomponentPortEndpoint
    get_spe (const ::Deployment::SubcomponentPortEndpoint &src)
    {
      DANCE_TRACE("SPE_Handler::get_spe - reverse");
      ACE_TString str;
      SID_Handler::IDREF.find_ref (src.instanceRef, str);

      IdRef xid;
      xid.idref (str.c_str ());

      return SubcomponentPortEndpoint (ACE_TEXT_CHAR_TO_TCHAR (src.portName.in ()),
                                       xid);
    }
      };

      typedef Sequence_Handler < SubcomponentPortEndpoint,
                 ::Deployment::SubcomponentPortEndpoints,
                 ::Deployment::SubcomponentPortEndpoint,
                 SPE_Handler::handle_spe > SPE_Functor;

      struct Packaging_Handlers_Export ACD_Handler
      {
    static void handle_acd (const AssemblyConnectionDescription &desc,
                ::Deployment::AssemblyConnectionDescription &toconfig)
    {
      DANCE_TRACE("ACD_Handler::get_acd");

      toconfig.name = ACE_TEXT_ALWAYS_CHAR ( desc.name ().c_str ());

      toconfig.deployRequirement.length (desc.count_deployRequirement ());
      std::for_each (desc.begin_deployRequirement (),
             desc.end_deployRequirement (),
             Requirement_Functor (toconfig.deployRequirement));

      toconfig.internalEndpoint.length (desc.count_internalEndpoint ());
      SEQ_HAND_GCC_BUG_WORKAROUND (SPE_Handler::handle_spe,
                                   (*desc.begin_internalEndpoint ()),
                                   toconfig.internalEndpoint);
      std::for_each (desc.begin_internalEndpoint (),
             desc.end_internalEndpoint (),
             SPE_Functor (toconfig.internalEndpoint));

      toconfig.externalEndpoint.length (desc.count_externalEndpoint ());
      std::for_each (desc.begin_externalEndpoint (),
                     desc.end_externalEndpoint (),
                     CEPE_Functor (toconfig.externalEndpoint));

      toconfig.externalReference.length (desc.count_externalReference ());
      std::for_each (desc.begin_externalReference (),
             desc.end_externalReference (),
             ERE_Functor (toconfig.externalReference));

    }

    static AssemblyConnectionDescription
    get_acd (const ::Deployment::AssemblyConnectionDescription &src)
    {
      DANCE_TRACE("ACD_Handler::get_acd - reverse");

      AssemblyConnectionDescription retval (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()));
#if 0
      for (CORBA::ULong i = 0; i < src.deployRequirement.length (); ++i)
        retval.add_deployRequirement (Req_Handler::get_requirement (src.deployRequirement[i]));

      for (CORBA::ULong i = 0; i < src.internalEndpoint.length (); ++i)
        retval.add_internalEndpoint
          (SPE_Handler::get_spe (src.internalEndpoint[i]));

      for (CORBA::ULong i = 0; i < src.externalEndpoint.length (); ++i)
        retval.add_externalEndpoint
          (CEPE_Handler::external_port_endpoint (src.externalEndpoint[i]));

      for (CORBA::ULong i = 0; i < src.externalReference.length (); ++i)
        retval.add_externalReference
          (ERE_Handler::external_ref_endpoint (src.externalReference[i]));
#endif
      return retval;
    }
      };

      typedef Sequence_Handler < AssemblyConnectionDescription,
                 ::Deployment::AssemblyConnectionDescriptions,
                 ::Deployment::AssemblyConnectionDescription,
                 ACD_Handler::handle_acd > ACD_Functor;


      struct Packaging_Handlers_Export SPR_Handler
      {
    static void handle_spr (const SubcomponentPropertyReference &desc,
                ::Deployment::SubcomponentPropertyReference &toconfig)
    {
      DANCE_TRACE("SPR_Handler::get_spr");

      toconfig.propertyName = ACE_TEXT_ALWAYS_CHAR ( desc.propertyName ().c_str ());

      ACE_TString str (desc.instance ().idref ().id ().c_str ());
      CORBA::ULong pos (0);

      SID_Handler::IDREF.find_ref (str, pos);
      toconfig.instanceRef = pos;

    }

    static SubcomponentPropertyReference
    get_spr (const ::Deployment::SubcomponentPropertyReference &src)
    {
      DANCE_TRACE("SPR_HAndler::get_spr - reverse");
      ACE_TString str;
      SID_Handler::IDREF.find_ref (src.instanceRef, str);

      IdRef xid;
      xid.idref (str.c_str ());
      return SubcomponentPropertyReference (ACE_TEXT_CHAR_TO_TCHAR (src.propertyName.in ()),
                                            xid);
    }

      };

      typedef Sequence_Handler < SubcomponentPropertyReference,
                 ::Deployment::SubcomponentPropertyReferences,
                 ::Deployment::SubcomponentPropertyReference,
                 SPR_Handler::handle_spr > SPR_Functor;

      struct Packaging_Handlers_Export APM_Handler
      {
    static void handle_apm (const AssemblyPropertyMapping &desc,
                ::Deployment::AssemblyPropertyMapping &toconfig)
    {
      DANCE_TRACE("APM_Handler::get_apm");

      toconfig.name = ACE_TEXT_ALWAYS_CHAR ( desc.name ().c_str ());
      toconfig.externalName = ACE_TEXT_ALWAYS_CHAR ( desc.externalName ().c_str ());

      toconfig.delegatesTo.length (desc.count_delegatesTo ());
      SEQ_HAND_GCC_BUG_WORKAROUND (SPR_Handler::handle_spr,
                                   (*desc.begin_delegatesTo ()),
                                   toconfig.delegatesTo);
      std::for_each (desc.begin_delegatesTo (),
             desc.end_delegatesTo (),
             SPR_Functor (toconfig.delegatesTo));
    }

    static AssemblyPropertyMapping
    get_apm (const ::Deployment::AssemblyPropertyMapping &src)
    {
      DANCE_TRACE("APM_Handler::get_apm - reverse");

      AssemblyPropertyMapping retval (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()),
                                      ACE_TEXT_CHAR_TO_TCHAR (src.externalName.in ()));
#if 0
      for (CORBA::ULong i = 0; i < src.delegatesTo.length (); ++i)
        retval.add_delegatesTo (SPR_Handler::get_spr (src.delegatesTo[i]));
#endif
      return retval;
    }
      };

      typedef Sequence_Handler < AssemblyPropertyMapping,
                 ::Deployment::AssemblyPropertyMappings,
                 ::Deployment::AssemblyPropertyMapping,
                 APM_Handler::handle_apm > APM_Functor;

      void
      CAD_Handler::component_assem_descr (const ComponentAssemblyDescription &desc,
                                          ::Deployment::ComponentAssemblyDescription &toconfig)
      {
        DANCE_TRACE("CAD_Handler::component_assem_descr");

        toconfig.instance.length (desc.count_instance ());
        std::for_each (desc.begin_instance (),
                       desc.end_instance (),
                       SID_Functor (toconfig.instance));

        /* @@ MAJO: Implement Locality */

        toconfig.connection.length (desc.count_connection ());
        SEQ_HAND_GCC_BUG_WORKAROUND (ACD_Handler::handle_acd,
                                     (*desc.begin_connection ()),
                                     toconfig.connection);
        std::for_each (desc.begin_connection (),
                       desc.end_connection (),
                       ACD_Functor (toconfig.connection));

        toconfig.externalProperty.length (desc.count_externalProperty ());
        SEQ_HAND_GCC_BUG_WORKAROUND (APM_Handler::handle_apm,
                                     (*desc.begin_externalProperty ()),
                                     toconfig.externalProperty);
        std::for_each (desc.begin_externalProperty (),
                       desc.end_externalProperty (),
                       APM_Functor (toconfig.externalProperty));
      }

      ComponentAssemblyDescription
      CAD_Handler::component_assem_descr (const ::Deployment::ComponentAssemblyDescription &/*src*/)
      {
        DANCE_TRACE("CAD_Handler::component_assem_descr - reverse");

        ComponentAssemblyDescription retval;
#if 0
        for (CORBA::ULong i = 0; i < src.instance.length (); ++i)
          retval.add_instance
            (SID_Handler::sub_comp_inst_descr (src.instance[i]));

        for (CORBA::ULong i = 0; i < src.connection.length (); ++i)
          retval.add_connection (ACD_Handler::get_acd (src.connection[i]));

        for (CORBA::ULong i = 0; i < src.externalProperty.length (); ++i)
          retval.add_externalProperty (APM_Handler::get_apm (src.externalProperty[i]));
#endif
        return retval;

      }

      ComponentAssemblyDescription *
      CAD_Handler::resolve_cad (const char *)
      {
        /*      xercesc::DOMDocument *dom =
                this->xml_helper_->create_dom (uri);

                if (!dom)
                throw Parse_Error ("Unable to create DOM for CAD");

                try {
                return new ComponentAssemblyDescription
                (componentAssemblyDescription (dom));
                }
                catch (...) {
                throw Parse_Error ("Unable to create XSC structure for CAD");
        */
        return 0;
      }
    }
  }
}


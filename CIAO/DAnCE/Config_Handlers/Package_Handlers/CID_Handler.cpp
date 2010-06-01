// $Id$
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "Package_Handlers/CID_Handler.h"
#include "Package_Handlers/CPD_Handler.h"
#include "Package_Handlers/IAD_Handler.h"
#include "Package_Handlers/CAD_Handler.h"
#include "Package_Handlers/Comp_Intf_Descr_Handler.h"
#include "Package_Handlers/NIA_Handler.h"
#include "SatisfierProperty_Handler.h"
#include "Req_Handler.h"

#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "XML_Typedefs.h"
#include "Utils/Exceptions.h"

#include "Deployment.hpp"
#include "Property_Handler.h"


namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Packaging
    {

      IDREF_Base<CORBA::ULong> CID_Handler::IDREF;

      struct Capability_Handler
      {
    static void handle_capability (const Capability &desc,
                       ::Deployment::Capability &toconfig)
    {
      DANCE_TRACE ("Capability_Handler::get_capability");

      toconfig.name = ACE_TEXT_ALWAYS_CHAR ( desc.name ().c_str ());

      toconfig.resourceType.length (desc.count_resourceType ());
      std::for_each (desc.begin_resourceType (),
             desc.end_resourceType (),
             String_Seq_Functor (toconfig.resourceType));

      toconfig.property.length (desc.count_property ());
      std::for_each (desc.begin_property (),
             desc.end_property (),
             SatisfierProperty_Functor (toconfig.property));
    }

    static Capability get_capability (const ::Deployment::Capability &src)
    {
      DANCE_TRACE ("Capability_Handler::get_capability - reverse");

      Capability retval (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()));
#if 0
      for (CORBA::ULong i = 0; i < src.resourceType.length (); ++i)
        retval.add_resourceType (src.resourceType[i].in ());

      for (CORBA::ULong i = 0; i < src.property.length (); ++i)
        retval.add_property (SatisfierProperty_Handler::get_sat_property (src.property[i]));
#endif
      return retval;
    }
      };

      typedef Sequence_Handler < Capability,
                 ::Deployment::Capabilities,
                 ::Deployment::Capability,
                 Capability_Handler::handle_capability > Capability_Functor;

      struct IR_Handler
      {
    static void handle_ir (const ImplementationRequirement &desc,
                   ::Deployment::ImplementationRequirement &toconfig)
    {
      DANCE_TRACE ("IR_Handler::get_ir");

      if (desc.resourceUsage_p ())
        {
          switch (desc.resourceUsage ().integral ())
        {
        case ResourceUsageKind::None_l:
          toconfig.resourceUsage = Deployment::None;
          break;

        case ResourceUsageKind::InstanceUsesResource_l:
          toconfig.resourceUsage = Deployment::InstanceUsesResource;
          break;

        case ResourceUsageKind::ResourceUsesInstance_l:
          toconfig.resourceUsage = Deployment::ResourceUsesInstance;
          break;

        case ResourceUsageKind::PortUsesResource_l:
          toconfig.resourceUsage = Deployment::PortUsesResource;
          break;

        case ResourceUsageKind::ResourceUsesPort_l:
          toconfig.resourceUsage = Deployment::ResourceUsesPort;
          break;

        default:
          throw Config_Error (desc.name (),
                              ACE_TEXT ("Unknown ResourceUsageKind."));
          break;
        }
        }

      if (desc.resourcePort_p ())
        toconfig.resourcePort = ACE_TEXT_ALWAYS_CHAR ( desc.resourcePort ().c_str ());

      if (desc.componentPort_p  ())
        toconfig.componentPort = ACE_TEXT_ALWAYS_CHAR ( desc.componentPort ().c_str ());

      toconfig.resourceType = ACE_TEXT_ALWAYS_CHAR ( desc.resourceType ().c_str ());
      toconfig.name = ACE_TEXT_ALWAYS_CHAR ( desc.name ().c_str ());

      std::for_each (desc.begin_property (),
             desc.end_property (),
             Property_Functor (toconfig.property ));
    }

    static ImplementationRequirement
    get_ir (const ::Deployment::ImplementationRequirement &src)
    {
      DANCE_TRACE ("IR_Handler::get_ir - reverse");

      ImplementationRequirement retval (ACE_TEXT_CHAR_TO_TCHAR (src.resourceType.in ()),
                                        ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()));

      if (src.resourceUsage.length () == 1)
        switch (src.resourceUsage[0])
          {
          case Deployment::None:
        retval.resourceUsage (ResourceUsageKind::None);
        break;

          case Deployment::InstanceUsesResource:
        retval.resourceUsage (ResourceUsageKind::InstanceUsesResource);
        break;

          case Deployment::ResourceUsesInstance:
        retval.resourceUsage (ResourceUsageKind::ResourceUsesInstance);
        break;

          case Deployment::PortUsesResource:
        retval.resourceUsage (ResourceUsageKind::PortUsesResource);
        break;

          case Deployment::ResourceUsesPort:
        retval.resourceUsage (ResourceUsageKind::ResourceUsesPort);
        break;

          default:
            throw Config_Error (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()),
                            ACE_TEXT ("Unknown ResourceUsageKind."));
        break;
          }

      retval.resourcePort (ACE_TEXT_CHAR_TO_TCHAR (src.resourcePort.in ()));

      retval.componentPort (ACE_TEXT_CHAR_TO_TCHAR (src.componentPort.in ()));
#if 0
      for (CORBA::ULong i = 0; i < src.property.length (); ++i)
        retval.add_property (Property_Handler::get_property (src.property[i]));
#endif
      return retval;
    }
      };

      typedef Sequence_Handler < ImplementationRequirement,
                 ::Deployment::ImplementationRequirements,
                 ::Deployment::ImplementationRequirement,
                 IR_Handler::handle_ir > IR_Functor;

      struct MID_Handler
      {
    static void handle_mid (const MonolithicImplementationDescription &desc,
                ::Deployment::MonolithicImplementationDescription &toconfig)
    {
      DANCE_TRACE ("MID_Handler::get_mid");

      toconfig.nodeExecParameter.length (desc.count_nodeExecParameter ());
      std::for_each (desc.begin_nodeExecParameter (),
             desc.end_nodeExecParameter (),
             Property_Functor (toconfig.nodeExecParameter));

      toconfig.componentExecParameter.length (desc.count_componentExecParameter ());
      std::for_each (desc.begin_componentExecParameter (),
             desc.end_componentExecParameter (),
             Property_Functor (toconfig.componentExecParameter));

      toconfig.deployRequirement.length (desc.count_deployRequirement ());
      while (0)
        {
          IR_Handler::handle_ir (*(*desc.begin_deployRequirement ()),
                                 toconfig.deployRequirement[0]);
        }
      std::for_each (desc.begin_deployRequirement (),
             desc.end_deployRequirement (),
             IR_Functor (toconfig.deployRequirement));

      toconfig.primaryArtifact.length (desc.count_primaryArtifact ());
      SEQ_HAND_GCC_BUG_WORKAROUND (NIA_Handler::handle_nia,
                                   (*desc.begin_primaryArtifact ()),
                                   toconfig.primaryArtifact);
      std::for_each (desc.begin_primaryArtifact (),
             desc.end_primaryArtifact (),
             NIA_Functor (toconfig.primaryArtifact));
    }

    static MonolithicImplementationDescription
    get_mid (const ::Deployment::MonolithicImplementationDescription &/*src*/)
    {
      DANCE_TRACE ("MID_Handler::get_mid - reverse");

      MonolithicImplementationDescription retval;
#if 0
      for (CORBA::ULong i = 0; i < src.nodeExecParameter.length (); ++i)
        retval.add_nodeExecParameter
          (Property_Handler::get_property (src.nodeExecParameter[i]));

      for (CORBA::ULong i = 0; i < src.componentExecParameter.length (); ++i)
        retval.add_componentExecParameter
          (Property_Handler::get_property (src.componentExecParameter[i]));

      for (CORBA::ULong i = 0; i < src.deployRequirement.length (); ++i)
        retval.add_deployRequirement
          (IR_Handler::get_ir (src.deployRequirement[i]));

      for (CORBA::ULong i = 0; i < src.primaryArtifact.length (); ++i)
        retval.add_primaryArtifact
          (NIA_Handler::get_nia (src.primaryArtifact[i]));
#endif
      return retval;
    }

      };

      typedef Sequence_Handler < MonolithicImplementationDescription,
                 ::Deployment::MonolithicImplementationDescriptions,
                 ::Deployment::MonolithicImplementationDescription,
                 MID_Handler::handle_mid > MID_Functor;



      void
      CID_Handler::component_impl_descr (
                                         const ComponentImplementationDescription &desc,
                                         ::Deployment::ComponentImplementationDescription &toconfig)
      {
        DANCE_TRACE ("CID_Handler::component_impl_descr");

        const ComponentImplementationDescription *cid = 0;
        std::auto_ptr <ComponentImplementationDescription> xsc_cid;

        if (desc.href_p ())
          {
            xsc_cid.reset (CID_Handler::resolve_cid ((desc.href ().c_str ())));
            cid = xsc_cid.get ();
          }
        else
          cid = &desc;

        if (cid->label_p ())
          toconfig.label = ACE_TEXT_ALWAYS_CHAR ( cid->label ().c_str ());

        if (cid->UUID_p ())
          toconfig.UUID = ACE_TEXT_ALWAYS_CHAR ( cid->UUID ().c_str ());

        if (cid->implements_p ())
          { // MAJO: We should be able to assume this exists, fix broken interpeters..
            Comp_Intf_Descr_Handler::comp_intf_descr (cid->implements (),
                                                      toconfig.implements);
          }

        if (cid->assemblyImpl_p ())
          {
            toconfig.assemblyImpl.length (1);
            CAD_Handler::component_assem_descr (cid->assemblyImpl (),
                                                toconfig.assemblyImpl[0]);
          }
        else if (cid->monolithicImpl_p ())
          {
            toconfig.monolithicImpl.length (1);
            MID_Handler::handle_mid (cid->monolithicImpl (),
                     toconfig.monolithicImpl[0]);
          }
        else
          throw Plan_Error (ACE_TEXT ("ComponentImplementationDescription must have either assemblyImpl or monolithicImpl"));

        // configProperty
        toconfig.configProperty.length (cid->count_configProperty ());
        std::for_each (cid->begin_configProperty (),
                       cid->end_configProperty (),
                       Property_Functor (toconfig.configProperty));

        // capability
        toconfig.capability.length (cid->count_capability ());
        SEQ_HAND_GCC_BUG_WORKAROUND (Capability_Handler::handle_capability,
                                     (*cid->begin_capability ()),
                                     toconfig.capability);
        std::for_each (cid->begin_capability (),
                       cid->end_capability (),
                       Capability_Functor (toconfig.capability));

        // dependsOn
        toconfig.dependsOn.length (cid->count_dependsOn ());
        CORBA::ULong pos = 0;
        for (ComponentImplementationDescription::dependsOn_const_iterator i = cid->begin_dependsOn ();
             i != cid->end_dependsOn ();
             ++i)
          toconfig.dependsOn[pos++].requiredType = ACE_TEXT_ALWAYS_CHAR ( (*i)->requiredType ().c_str ());

        // infoProperty
        toconfig.infoProperty.length (cid->count_infoProperty ());
        std::for_each (cid->begin_infoProperty (),
                       cid->end_infoProperty (),
                       Property_Functor (toconfig.infoProperty));

      }

      ComponentImplementationDescription
      CID_Handler::component_impl_descr (const Deployment::ComponentImplementationDescription& src)
      {
        DANCE_TRACE ("CID_Handler::component_impl_descr - reverse");

        ComponentImplementationDescription retval;

        retval.label (ACE_TEXT_CHAR_TO_TCHAR (src.label.in ()));
        retval.UUID (ACE_TEXT_CHAR_TO_TCHAR (src.UUID.in ()));

        {
          retval.implements (Comp_Intf_Descr_Handler::comp_intf_descr (src.implements));
        }

        if (src.assemblyImpl.length () == 1)
          {
            retval.assemblyImpl (CAD_Handler::component_assem_descr (src.assemblyImpl[0]));
          }
        else if (src.monolithicImpl.length () == 1)
          {
            retval.monolithicImpl (MID_Handler::get_mid (src.monolithicImpl[0]));
          }
        else
          DANCE_ERROR (1, (LM_WARNING, "Warning: ComponentImplementationDescription lacks "
                         "either a required assemblyImpl or monolithicImpl, or has too many"));
#if 0
        for (CORBA::ULong i = 0; i < src.configProperty.length (); ++i)
          {
            retval.add_configProperty (
                                       Property_Handler::get_property (src.configProperty[i]));
          }

        for (CORBA::ULong i = 0; i < src.dependsOn.length (); ++i)
          {
            retval.add_dependsOn (ImplementationDependency (src.dependsOn[i].requiredType.in ()));
          }

        for (CORBA::ULong i = 0; i < src.infoProperty.length (); ++i)
          {
            retval.add_infoProperty (
                                     Property_Handler::get_property (src.infoProperty[i]));
          }
#endif
        return retval;
      }

      ComponentImplementationDescription *
      CID_Handler::resolve_cid (const ACE_TCHAR *uri)
      {
        DANCE_TRACE ("CID_Handler::resolve_cid");

        xercesc::DOMDocument *dom = XML_Helper::XML_HELPER.create_dom (uri);

        if (!dom)
          throw Parse_Error (ACE_TEXT ("Unable to create DOM for CID"));

        try {
          return new ComponentImplementationDescription
            (reader::componentImplementationDescription (dom));
        }
        catch (...) {
          throw Parse_Error (ACE_TEXT ("Unable to create XSC structure for CID"));
        }
      }
    }

  }
}

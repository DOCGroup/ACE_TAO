// $Id$
#include "tao/AnyTypeCode/TypeCode.h"
#include "Comp_Intf_Descr_Handler.h"
#include "XML_Typedefs.h"
#include "Utils/Exceptions.h"
#include "Utils/Functors.h"
#include "DataType_Handler.h"
#include "Property_Handler.h"
#include "Deployment.hpp"
#include "Package_Handlers/CPD_Handler.h"
#include "Config_Handlers/CPD_Handler.h"
#include "DAnCE/Logger/Log_Macros.h"
namespace DAnCE
{
  namespace Config_Handlers
  {
    namespace Packaging
    {
        struct Comp_Prop_Handler
        {
          static void handle_cpd (const ComponentPropertyDescription &desc,
                               ::Deployment::ComponentPropertyDescription &toconfig)
          {
            DANCE_TRACE ("Comp_Prop_Handler::get_cpd");

            toconfig.name = ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ());
            DataType_Handler::data_type (desc.type (),
                                         toconfig.type);
          }

          static ComponentPropertyDescription
          get_cpd (const ::Deployment::ComponentPropertyDescription &src)
          {
            DANCE_TRACE ("Comp_Prop_Handler::get_cpd - reverse");

            return ComponentPropertyDescription (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()),
                                                 DataType_Handler::data_type (src.type));
          }

        };

        typedef Sequence_Handler < ComponentPropertyDescription,
                                   ::Deployment::ComponentPropertyDescriptions,
                                   ::Deployment::ComponentPropertyDescription,
                                   Comp_Prop_Handler::handle_cpd > Comp_Prop_Functor;


      void
      Comp_Intf_Descr_Handler::comp_intf_descr (
                                                const ComponentInterfaceDescription &desc,
                                                ::Deployment::ComponentInterfaceDescription &toconfig)
      {
        DANCE_TRACE ("Comp_Intf_Descr_Handler::comp_intf_descr");

        const ComponentInterfaceDescription *cid = 0;
        std::auto_ptr <ComponentInterfaceDescription> safe_cid;

        if (desc.href_p ())
          {
            safe_cid.reset (Comp_Intf_Descr_Handler::resolve_cid (desc.href ().c_str ()));
            cid = safe_cid.get ();
          }
        else
          cid = &desc;

        if (cid->label_p ())
          toconfig.label = ACE_TEXT_ALWAYS_CHAR (cid->label ().c_str ());

        if (cid->UUID_p ())
          toconfig.UUID = ACE_TEXT_ALWAYS_CHAR (cid->UUID ().c_str ());

        // MAJO: SpecificType should be required.
        if (cid->specificType_p ())
          toconfig.specificType = ACE_TEXT_ALWAYS_CHAR (cid->specificType ().c_str ());

        toconfig.supportedType.length (cid->count_supportedType ());
        std::for_each (cid->begin_supportedType (),
                       cid->end_supportedType (),
                       String_Seq_Functor (toconfig.supportedType));

        toconfig.idlFile.length (cid->count_idlFile ());
        std::for_each (cid->begin_idlFile (),
                       cid->end_idlFile (),
                       String_Seq_Functor (toconfig.idlFile));

        toconfig.configProperty.length (cid->count_configProperty ());
        std::for_each (cid->begin_configProperty (),
                       cid->end_configProperty (),
                       Property_Functor (toconfig.configProperty));

        toconfig.port.length (cid->count_port ());
        std::for_each (cid->begin_port (),
                       cid->end_port (),
                       DAnCE::Config_Handlers::CPD_Functor (toconfig.port));

        toconfig.property.length (cid->count_property ());
        SEQ_HAND_GCC_BUG_WORKAROUND (Comp_Prop_Handler::handle_cpd,
                                     (*cid->begin_property ()),
                                     toconfig.property);
        std::for_each (cid->begin_property (),
                       cid->end_property (),
                       Comp_Prop_Functor (toconfig.property));

        toconfig.infoProperty.length (cid->count_infoProperty ());
        std::for_each (cid->begin_infoProperty (),
                       cid->end_infoProperty (),
                       Property_Functor (toconfig.infoProperty));
      }


      ComponentInterfaceDescription
      Comp_Intf_Descr_Handler::comp_intf_descr (
                                                const ::Deployment::ComponentInterfaceDescription &src)
      {
        DANCE_TRACE ("Comp_Intf_Descr_Handler::comp_intf_descr - reverse");
        ComponentInterfaceDescription retval;

        retval.label (ACE_TEXT_CHAR_TO_TCHAR (src.label.in ()));
        retval.UUID (ACE_TEXT_CHAR_TO_TCHAR  (src.UUID.in ()));
        retval.specificType (ACE_TEXT_CHAR_TO_TCHAR  (src.specificType.in ()));
#if 0
        for (CORBA::ULong i = 0; i < src.supportedType.length (); ++i)
          retval.add_supportedType (src.supportedType[i].in ());

        for (CORBA::ULong i = 0; i < src.idlFile.length (); ++i)
          retval.add_idlFile (src.idlFile[i].in ());

        for (CORBA::ULong i = 0; i < src.configProperty.length (); ++i)
          retval.add_configProperty (
                                     Property_Handler::get_property (src.configProperty[i]));

        for (CORBA::ULong i = 0; i < src.port.length (); ++i)
          retval.add_port
            (DAnCE::Config_Handlers::CPD_Handler::component_port_description (src.port[i]));

        for (CORBA::ULong i = 0; i < src.property.length (); ++i)
          retval.add_property (Comp_Prop_Handler::get_cpd (src.property[i]));

        for (CORBA::ULong i = 0; i < src.infoProperty.length (); ++i)
          retval.add_infoProperty (
                                   Property_Handler::get_property (src.infoProperty[i]));
#endif
        return retval;
      }

      ComponentInterfaceDescription *
      Comp_Intf_Descr_Handler::resolve_cid (const ACE_TCHAR *uri)
      {
        DANCE_TRACE ("Comp_Intf_Descr_Handler::resolve_cid");

        xercesc::DOMDocument *dom = XML_Helper::XML_HELPER.create_dom (uri);

        if (!dom)
          throw Parse_Error (ACE_TEXT ("Unable to create DOM for Component Interface Description."));

        try {
          return new ComponentInterfaceDescription
            (reader::componentInterfaceDescription (dom));
        }
        catch (...) {
          throw Parse_Error (ACE_TEXT ("Unable to create XSC structure for Component Interface Description"));
        }
      }
    }
  }}

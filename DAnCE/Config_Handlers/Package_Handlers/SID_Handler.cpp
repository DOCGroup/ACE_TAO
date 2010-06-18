// $Id$
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "Package_Handlers/SID_Handler.h"
#include "cid.hpp"
#include "DAnCE/Deployment/DeploymentC.h"
#include "Package_Handlers/CPD_Handler.h"
#include "Package_Handlers/PCD_Handler.h"
#include "Package_Handlers/Comp_Intf_Descr_Handler.h"
#include "Req_Handler.h"
#include "Property_Handler.h"

#include "ace/UUID.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Packaging
    {
      IDREF_Base<CORBA::ULong> SID_Handler::IDREF;

      void
      SID_Handler::handle_sub_comp_inst_descr (const SubcomponentInstantiationDescription &desc,
                                         ::Deployment::SubcomponentInstantiationDescription &toconfig)
      {
        DANCE_TRACE ("SID_Handler::sub_comp_inst_descr");
        toconfig.name = ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ());

        if (desc.basePackage_p ())
          {
            toconfig.basePackage.length (1);
            CPD_Handler::handle_component_package_descr (desc.basePackage (),
                                                  toconfig.basePackage[0]);
          }
        else if (desc.specializedConfig_p ())
          {
            toconfig.specializedConfig.length (1);
            PCD_Handler::package_config (desc.specializedConfig (),
                                         toconfig.specializedConfig[0]);
          }
        else if (desc.importedPackage_p ())
          {
            toconfig.importedPackage.length (1);
            toconfig.importedPackage[0].location.length (desc.importedPackage ().count_location ());

            std::for_each (desc.importedPackage ().begin_location (),
                           desc.importedPackage ().end_location (),
                           String_Seq_Functor (toconfig.importedPackage[0].location));
          }
        else if (desc.referencedPackage_p ())
          {
            toconfig.referencedPackage.length (1);

            if (desc.referencedPackage ().requiredUUID_p ())
              toconfig.referencedPackage[0].requiredUUID =
                ACE_TEXT_ALWAYS_CHAR (desc.referencedPackage ().requiredUUID ().c_str ());

            if (desc.referencedPackage ().requiredName_p ())
              toconfig.referencedPackage[0].requiredName =
                ACE_TEXT_ALWAYS_CHAR (desc.referencedPackage ().requiredName ().c_str ());

            Comp_Intf_Descr_Handler::comp_intf_descr (desc.referencedPackage ().requiredType (),
                                                      toconfig.referencedPackage[0].requiredType);

          }

        toconfig.selectRequirement.length (desc.count_selectRequirement ());
        std::for_each (desc.begin_selectRequirement (),
                       desc.end_selectRequirement (),
                       Requirement_Functor (toconfig.selectRequirement));

        toconfig.configProperty.length (desc.count_configProperty ());
        std::for_each (desc.begin_configProperty (),
                       desc.end_configProperty (),
                       Property_Functor (toconfig.configProperty));

        if (desc.id_p ())
          {
            ACE_TString str (desc.id ().c_str ());
            SID_Handler::IDREF.bind_next_available (str);
          }
        else
          DANCE_ERROR (1, (LM_WARNING, ACE_TEXT ("Warning: SID With Name %s has no ID\n"),
                           desc.name ().c_str ()));

      }

      SubcomponentInstantiationDescription
      SID_Handler::sub_comp_inst_descr (const Deployment::SubcomponentInstantiationDescription &src)
      {
        DANCE_TRACE ("SID_Handler::sub_comp_inst_descr - reverse");
        SubcomponentInstantiationDescription retval (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()));

        if (src.basePackage.length () == 1)
          retval.basePackage
            (CPD_Handler::component_package_descr (src.basePackage[0]));
        else if (src.specializedConfig.length () == 1)
          retval.specializedConfig
            (PCD_Handler::package_config (src.specializedConfig[0]));
        else if (src.importedPackage.length () == 1)
          {
            ComponentPackageImport ci;
#if 0
            for (CORBA::ULong i = 0; i < src.importedPackage[0].location.length (); ++i)
              ci.add_location (src.importedPackage[0].location[i].in ());
#endif
            retval.importedPackage (ci);
          }
        else if (src.referencedPackage.length () == 1)
          {
            ComponentPackageReference cpr (Comp_Intf_Descr_Handler::comp_intf_descr (src.referencedPackage[0].requiredType));
            cpr.requiredUUID (ACE_TEXT_CHAR_TO_TCHAR (src.referencedPackage[0].requiredUUID.in ()));
            cpr.requiredName (ACE_TEXT_CHAR_TO_TCHAR (src.referencedPackage[0].requiredName.in ()));

            retval.referencedPackage (cpr);
          }
#if 0
        for (CORBA::ULong i = 0; i < src.selectRequirement.length (); ++i)
          retval.add_selectRequirement
            (Req_Handler::get_requirement (src.selectRequirement[i]));

        for (CORBA::ULong i = 0; i < src.configProperty.length (); ++i)
          retval.add_configProperty
            (Property_Handler::get_property (src.configProperty[i]));
#endif
        // @@MAJO This is not a good way of binding reverse IDREFS.
        std::auto_ptr <ACE_Utils::UUID> safe_uuid (
          ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID ());
        ACE_TString uuid ( ACE_TEXT_CHAR_TO_TCHAR (safe_uuid->to_string ()->c_str ()));

        //        ACE_ERROR ((LM_ERROR, "*** Binding to %s\n",
        //           uuid.c_str ()));

        SID_Handler::IDREF.bind_next_available (uuid);

        return retval;
      }

    }
  }
}


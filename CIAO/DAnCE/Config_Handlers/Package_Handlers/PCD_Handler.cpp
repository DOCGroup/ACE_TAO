// $Id$
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "XML_Typedefs.h"
#include "XML/XercesString.h"
#include "Utils/Exceptions.h"
#include "Package_Handlers/PCD_Handler.h"
#include "Package_Handlers/CPD_Handler.h"
#include "toplevel.hpp"
#include "Deployment.hpp"
#include "Property_Handler.h"
#include "Req_Handler.h"

#include <memory>

using DAnCE::XML::XStr;

namespace DAnCE
{
  namespace Config_Handlers
  {
    namespace Packaging
    {
      void
      PCD_Handler::package_config (const ACE_TCHAR *uri,
                                   ::Deployment::PackageConfiguration &toconfig)
      {
        XERCES_CPP_NAMESPACE::DOMDocument *dom = XML_Helper::XML_HELPER.create_dom (uri);

        if (dom == 0)
          {
            std::basic_string<ACE_TCHAR> error (ACE_TEXT ("Unable to open file: "));
            error += uri;
            throw Parse_Error (error);
          }

        XStr root = dom->getDocumentElement ()->getTagName ();

        if (root == XStr (ACE_TEXT ("Deployment:topLevelPackageDescription")))
          {
            TopLevelPackageDescription tpd;

            tpd = reader::topLevelPackageDescription (dom);

            PCD_Handler::package_config (*(*tpd.begin_package ()),
                                         toconfig);
          }
        else if (root == XStr (ACE_TEXT ("Deployment:packageConfiguration")))
          {
            PackageConfiguration pcd;
            pcd = reader::packageConfiguration (dom);
            PCD_Handler::package_config (pcd, toconfig);
          }
        else
          {
            std::string error ("Invliad file passed to package_config, has base ");

            char *croot = xercesc::XMLString::transcode (root);

            error += croot;

            delete [] croot;

            throw Plan_Error (ACE_TEXT ("Invalid file passed to package_config, had base"));
          }
      }

      void
      PCD_Handler::package_config (const PackageConfiguration &desc,
                                   ::Deployment::PackageConfiguration &toconfig)
      {
        DANCE_TRACE ("PCD_Handler::package_config");
        std::auto_ptr < PackageConfiguration > xsc_pcd;

        const PackageConfiguration *pcd;

        if (desc.contentLocation_p ())
          {
            // Take ownership of the resolved pcd
            xsc_pcd.reset (PCD_Handler::resolve_package_config (desc.contentLocation ().c_str ()));
            pcd = xsc_pcd.get ();
          }
        else
          pcd = &desc;

        if (pcd->label_p ())
          toconfig.label =
            ACE_TEXT_ALWAYS_CHAR (pcd->label ().c_str ());

        if (pcd->UUID_p ())
          toconfig.UUID =
            ACE_TEXT_ALWAYS_CHAR (pcd->UUID ().c_str ());

        if (pcd->basePackage_p ())
          {
            toconfig.basePackage.length (1);
            CPD_Handler::handle_component_package_descr (pcd->basePackage (),
                                                         toconfig.basePackage [0]);
          }

        // @@ MAJO: Support other elements present here.

        toconfig.configProperty.length (desc.count_configProperty ());
        std::for_each (desc.begin_configProperty (),
                       desc.end_configProperty (),
                       Property_Functor (toconfig.configProperty));

        toconfig.selectRequirement.length (desc.count_selectRequirement ());
        std::for_each (desc.begin_selectRequirement (),
                       desc.end_selectRequirement (),
                       Requirement_Functor (toconfig.selectRequirement));

      }

      PackageConfiguration
      PCD_Handler::package_config (const Deployment::PackageConfiguration& src)
      {
        DANCE_TRACE ("PCD_Handler::package_config - reverse");
        PackageConfiguration pcd = PackageConfiguration ();

        if (src.label.in () != 0)
          pcd.label (ACE_TEXT_CHAR_TO_TCHAR (src.label.in ()));

        if (src.UUID.in () != 0)
          pcd.UUID (ACE_TEXT_CHAR_TO_TCHAR (src.UUID.in ()));

        if (src.basePackage.length () > 0)
          {
            pcd.basePackage (CPD_Handler::component_package_descr (src.basePackage[0]));
          }
#if 0
        // @@ MAJO: Support other elements present here.
        for (CORBA::ULong i = 0; i < src.configProperty.length (); ++i)
          pcd.add_configProperty (Property_Handler::get_property (src.configProperty[i]));

        for (CORBA::ULong i = 0; i < src.selectRequirement.length (); ++i)
          pcd.add_selectRequirement (Req_Handler::get_requirement (src.selectRequirement[i]));
#endif
        return pcd;

      }

      PackageConfiguration * PCD_Handler::resolve_package_config (const ACE_TCHAR *uri)
      {
        xercesc::DOMDocument* dom =
          XML_Helper::XML_HELPER.create_dom (uri);

        if (!dom)
          throw Parse_Error (ACE_TEXT ("Unable to create DOM for PackageConfiguration"));

        try {
          return new PackageConfiguration (reader::packageConfiguration (dom));
        }
        catch (...) {
          throw Parse_Error (ACE_TEXT ("Unable to create XSC structure for PackageConfiguration"));
        }

      }


    }
  }
}

// $Id$
#include "STD_CPD_Handler.h"
#include "STD_CID_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "Deployment.hpp"
#include "Utils/XML_Typedefs.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    bool
    STD_CPD_Handler::component_package_descr (
      const ComponentPackageDescription &desc,
      ::Deployment::ComponentPackageDescription &toconfig)
    {
      toconfig.label =
        desc.label ().c_str ();

      toconfig.UUID =
        desc.UUID ().c_str ();

      PackagedComponentImplementation pci = desc.implementation ();
      Deployment::PackagedComponentImplementation idl_pci;
      idl_pci.name =
        pci.name ().c_str ();

      if (XML_Helper::XML_HELPER.is_initialized ())
        return false;

      // parse the .cid (ComponentImplementationDescription) file
      // that <referencedImplementation> links to
      xercesc::DOMDocument* dom =
        XML_Helper::XML_HELPER.create_dom 
        ((pci.referencedImplementation ()).href ().c_str ()); // here location indicates .pcd file

      if (!dom)
        return false;

      ComponentImplementationDescription cid =
        componentImplementationDescription (dom);

      Deployment::ComponentImplementationDescription idl_cid;

      STD_CID_Handler cid_handler;

      cid_handler.component_impl_descr (cid,
                                        idl_cid);

      idl_pci.referencedImplementation = idl_cid;

      toconfig.implementation.length (1);
      toconfig.implementation [0] = idl_pci;

      return true;
    }
  }
}

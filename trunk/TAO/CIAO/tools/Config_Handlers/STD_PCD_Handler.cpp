// $Id$
#include "STD_PCD_Handler.h"
#include "STD_CPD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "ciao/Packaging_DataC.h"
#include "Deployment.hpp"


namespace CIAO
{
  namespace Config_Handlers
  {

    bool
    STD_PCD_Handler::package_config (
      const PackageConfiguration &desc,
      ::Deployment::PackageConfiguration &toconfig)
    {
      toconfig.label =
        desc.label ().c_str ();

      toconfig.UUID =
        desc.UUID ().c_str ();

      if (!this->xml_helper_->is_initialized ())
        return false;

      // parse the .cpd (ComponentPackageDescription) file
      // that <basePackage> links to
      xercesc::DOMDocument* dom =
        this->xml_helper_->create_dom ((desc.basePackage()).href ().c_str ());

      if (!dom)
        return false;

      ComponentPackageDescription cpc =
        componentPackageDescription (dom);

      Deployment::ComponentPackageDescription idl_cpc;

      STD_CPD_Handler cpd_handler (this->xml_helper_);

      cpd_handler.component_package_descr (cpc,
                                           idl_cpc);

      toconfig.basePackage.length (1);
      toconfig.basePackage [0] = idl_cpc;

      return true;
    }
  }
}

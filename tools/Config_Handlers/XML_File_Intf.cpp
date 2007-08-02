// $Id$

#include "XML_File_Intf.h"
#include "Utils/XML_Helper.h"
#include "Deployment.hpp"
#include "DP_Handler.h"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "ciao/CIAO_common.h"


namespace CIAO
{
  namespace Config_Handlers
  {
    XML_File_Intf::XML_File_Intf (const char *file)
    {
      if (!this->read_process_file (file))
        throw;
    }

    bool
    XML_File_Intf::read_process_file (const char *file)
    {
      CIAO_TRACE("XML_File_Intf::read_process_file");

      XML_Helper helper;

      if (!helper.is_initialized ())
        return false;

      XERCES_CPP_NAMESPACE::DOMDocument *dom =
        helper.create_dom (file);

      if (!dom)
        return false;

      DeploymentPlan dp =
        deploymentPlan (dom);


      DP_Handler dp_handler (dp);

      this->idl_dp_.reset (dp_handler.plan ());

      if (this->idl_dp_.get ())
        return true;

      return false;
    }

    ::Deployment::DeploymentPlan const *
    XML_File_Intf::get_plan (void) const
    {
      CIAO_TRACE("get_plan");
      return this->idl_dp_.get ();
    }

    ::Deployment::DeploymentPlan *
    XML_File_Intf::get_plan (void)
    {
      CIAO_TRACE("XML_File_Intf::get_plan");
      return this->idl_dp_.release ();
    }
  }
}

// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CPR_Handler.h"
#include "CompPkgDesc_Handler.h"
#include "PC_Handler.h"
#include "Process_Element.h"
#include "Process_Basic_Type.h"

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void PC_Handler::process_PackageConfiguration
                    (::Deployment::PackageConfiguration &pc)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());
      if (node_name == XStr 
                    (ACE_TEXT ("Deployment:PackageConfiguration")))
        {
        }
      else if (process_string (this->iter_, node_name, "label", pc.label))
        {
        }
      else if (process_string (this->iter_, node_name, "UUID", pc.UUID))
        {
        }
      else if (process_sequence_remote
                 <Deployment::ComponentPackageDescription,
                  CompPkgDesc_Handler>
                     (this->doc_, this->iter_, node,
                      node_name, "basePackage", pc.basePackage,
                     &CompPkgDesc_Handler::process_ComponentPackageDescription,
                      this->id_map_))
        {
        }
      else if (process_sequence_remote
                <Deployment::ComponentPackageReference, CPR_Handler>
                   (this->doc_, this->iter_, node,
                    node_name, "reference", pc.reference,
                    &CPR_Handler::process_ComponentPackageReference,
                    this->id_map_))
        {
        }
      else if (process_sequence_common
             <Deployment::Requirement>
                (this->doc_, this->iter_, node,
                 node_name, "selectRequirement", pc.selectRequirement,
                 &Requirement_Handler::process_Requirement,this->id_map_))
        {
        }
      else if (process_sequence_common<Deployment::Property>
            (this->doc_, this->iter_, node,
             node_name, "configProperty", pc.configProperty,
             &Property_Handler::process_Property, this->id_map_))
        {
        }
      else
        {
          ACE_THROW (CORBA::INTERNAL());
        }
    }
    return;
}

END_DEPLOYMENT_NAMESPACE

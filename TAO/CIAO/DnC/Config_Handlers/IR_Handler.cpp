//$Id$
#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "IAD_Handler.h"
#include "IR_Handler.h"
#include "RUK_Handler.h"
#include "Utils.h"
#include "Process_Basic_Type.h"
#include "Process_Element.h"
#include "Property_Handler.h"

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void IR_Handler::process_ImplementationRequirement
(::Deployment::ImplementationRequirement &ir)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
        (ACE_TEXT ("Deployment:ImplementationRequirement")))
        {
        }
      else if
        (process_string (this->iter_, node_name, "resourcePort", 
                         ir.resourcePort));
      else if
        (process_string (this->iter_, node_name, "componentPort", 
                         ir.componentPort));
      else if
        (process_string (this->iter_, node_name, "resourceType", 
                         ir.resourceType));
      else if
        (process_string (this->iter_, node_name, "name", ir.name));
      else if
        (process_sequence_common<Deployment::Property>
           (this->doc_, this->iter_, node,
            node_name, "Property", ir.property,
            &Property_Handler::process_Property,
            this->id_map_));
      else
        {
          this->iter_->previousNode ();
          return;
        }
    }
}

END_DEPLOYMENT_NAMESPACE

// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "CompImplDesc_Handler.h"
#include "PCI_Handler.h"
#include "Process_Basic_Type.h"
#include "Process_Element.h"

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void PCI_Handler::process_PackagedComponentImplementation
                      (::Deployment::PackagedComponentImplementation &pci)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr 
            (ACE_TEXT ("Deployment:PackagedComponentImplementation")))
        {
        }
      else if
        (process_string (this->iter_, node_name, "name", pci.name));
      else if
        (process_element_remote<Deployment::ComponentImplementationDescription,
                                CompImplDesc_Handler>
           (this->doc_, this->iter_, node,
            node_name, "referencedImplementation", 
            pci.referencedImplementation,
            &CompImplDesc_Handler::process_ComponentImplementationDescription, 
            this->id_map_));
      else
        {
          this->iter_->previousNode ();
          return;
        }
    }
  return;
}

END_DEPLOYMENT_NAMESPACE

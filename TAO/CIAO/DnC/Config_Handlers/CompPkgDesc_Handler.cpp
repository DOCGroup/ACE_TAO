// $Id$

#ifndef COMPPKGDESC_HANDLER_C
#define COMPPKGDESC_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Process_Basic_Type.h"

#include "CompPkgDesc_Handler.h"
#include "Property_Handler.h"
#include "PCI_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void CompPkgDesc_Handler::process_ComponentPackageDescription
(::Deployment::ComponentPackageDescription &comppkgdesc)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      ACE_DEBUG ((LM_DEBUG, "i am here \n"));
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string (this->iter_, node_name, "label", comppkgdesc.label));
      else if
        (process_string (this->iter_, node_name, "UUID", comppkgdesc.UUID));
      else if
        // TODO: Which id-map here?
        (process_element_remote<Deployment::ComponentInterfaceDescription, CompIntrDesc_Handler>
         (this->doc_, this->iter_, node,
          node_name, "realizes", comppkgdesc.realizes,
          &CompIntrDesc_Handler::process_ComponentInterfaceDescription,
          this->id_map_));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "configProperty", comppkgdesc.configProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else if
        (process_sequence_remote<Deployment::PackagedComponentImplementation, PCI_Handler>
         (this->doc_, this->iter_, node,
          node_name, "implementation", comppkgdesc.implementation,
          &PCI_Handler::process_PackagedComponentImplementation,
          this->id_map_));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "infoProperty", comppkgdesc.infoProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else
        {
          // ??? How did we get here ???
          ACE_THROW (CORBA::INTERNAL());
        }
    }
}

END_DEPLOYMENT_NAMESPACE

#endif /* COMPPKGDESC_HANDLER_C */

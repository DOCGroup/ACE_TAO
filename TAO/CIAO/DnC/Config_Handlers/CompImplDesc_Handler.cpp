// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CAD_Handler.h"
#include "MID_Handler.h"
#include "ID_Handler.h"
#include "Cap_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "CompImplDesc_Handler.h"

#include "Process_Basic_Type.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;


BEGIN_DEPLOYMENT_NAMESPACE

/// handle the component implementation description and populate it
void CompImplDesc_Handler::process_ComponentImplementationDescription
(::Deployment::ComponentImplementationDescription &cid)
{
  //this->iter_->nextNode();
  ACE_TString root_node_name;

  root_node_name = XMLString::transcode
    (this->doc_->getDocumentElement ()->getNodeName ());


  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());
      if (node_name == XStr
          (ACE_TEXT ("Deployment:ComponentImplementationDescription")))
        {
        }
      else if
        (process_string (this->iter_, node_name, "label", cid.label));
      else if
        (process_string (this->iter_, node_name, "UUID", cid.UUID));
      else if
        (process_element_remote<Deployment::ComponentInterfaceDescription, CompIntrDesc_Handler>
         (this->doc_, this->iter_, node,
          node_name, "implements", cid.implements,
          &CompIntrDesc_Handler::process_ComponentInterfaceDescription, this->id_map_));
      else if
        (process_sequence_remote<Deployment::ComponentAssemblyDescription, CAD_Handler>
         (this->doc_, this->iter_, node,
          node_name, "assemblyImpl", cid.assemblyImpl,
          &CAD_Handler::process_ComponentAssemblyDescription, this->id_map_));
      else if
        (process_sequence_remote<Deployment::MonolithicImplementationDescription, MID_Handler>
         (this->doc_, this->iter_, node,
          node_name, "monolithicImpl", cid.monolithicImpl,
          &MID_Handler::process_MonolithicImplementationDescription, this->id_map_));
      else if
        (process_sequence_common<Deployment::Property>
         (this->doc_, this->iter_, node,
          node_name, "configProperty", cid.configProperty,
          &Property_Handler::process_Property,
          this->id_map_));
      else if
        (process_sequence_remote<Deployment::Capability, CAP_Handler>
         (this->doc_, this->iter_, node,
          node_name, "capability", cid.capability,
          &CAP_Handler::process_Capability, this->id_map_));
      else if
        (process_sequence_common<Deployment::ImplementationDependency>
         (this->doc_, this->iter_, node,
          node_name, "dependsOn", cid.dependsOn,
          &ID_Handler::process_ImplementationDependency,
          this->id_map_));
      else if
        (process_sequence_common<Deployment::Property>
         (this->doc_, this->iter_, node,
          node_name, "infoProperty", cid.infoProperty,
          &Property_Handler::process_Property,
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

//$Id$

#ifndef CompIntrDesc_HANDLER_C
#define CompIntrDesc_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Process_Basic_Type.h"
#include "Process_Element.h"
#include "Property_Handler.h"
#include "CompIntrDesc_Handler.h"
#include "DT_Handler.h"
#include "CPK_Handler.h"
#include <iostream>

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the package configuration and populate it
void CompIntrDesc_Handler::process_ComponentInterfaceDescription
(::Deployment::ComponentInterfaceDescription &ccd)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:ComponentInterfaceDescription")))
        {
        }
      else if
        (process_string(this->iter_, node_name, "label", ccd.label));
      else if
        (process_string(this->iter_, node_name, "UUID", ccd.UUID));
      else if
        (process_string(this->iter_, node_name, "specificType", ccd.specificType));
      else if
        (process_string_seq(this->iter_, node_name, "supportedType", ccd.supportedType));
      else if
        (process_string_seq(this->iter_, node_name, "idlFile", ccd.idlFile));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "configProperty", ccd.configProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else if
        (process_sequence_local<Deployment::ComponentPortDescription>
         (this->doc_, this->iter_, node,
          node_name, "port", ccd.port,
          this, &CompIntrDesc_Handler::process_port));
      else if
        (process_sequence_local<Deployment::ComponentPropertyDescription>
         (this->doc_, this->iter_, node,
          node_name, "property", ccd.property,
          this, &CompIntrDesc_Handler::process_comp_property));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "infoProperty", ccd.infoProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else
        {
          // ??? How did we get here ???
          ACE_THROW (CORBA::INTERNAL());
        }
    }
  return;
}

/// process component property element
void CompIntrDesc_Handler::process_comp_property (DOMNodeIterator* iter,
                                                  Deployment::ComponentPropertyDescription& property)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:ComponentPropertyDescription")))
        {
        }
      else if
        (process_string(iter, node_name, "name", property.name));
      else if (node_name == XStr (ACE_TEXT ("type")))
        {
          int argc = 0;
          char ** argv = 0;
          CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "");
          DT_Handler::process_DataType (iter, property.type.out (), orb);
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

/// process port element
void CompIntrDesc_Handler::process_port (DOMNodeIterator* iter,
                                Deployment::ComponentPortDescription& port)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:ComponentPortDescription")))
        {
        }
      else if
        (process_string(iter, node_name, "name", port.name));
      else if
        (process_string(iter, node_name, "specificType", port.specificType));
      else if
        (process_string_seq(iter, node_name, "supportedType", port.supportedType));
      else if
        (process_boolean(iter, node_name, "provider", port.provider));
      else if
        (process_boolean(iter, node_name, "exclusiveProvider", port.exclusiveProvider));
      else if
        (process_boolean(iter, node_name, "exclusiveUser", port.exclusiveUser));
      else if
        (process_boolean(iter, node_name, "optional", port.optional));
      else if (node_name == XStr(ACE_TEXT("kind")))
        CPK_Handler::process_CCMComponentPortKind (iter, port.kind);
      else
        {
          iter->previousNode();
          return;
        }
    }
}

END_DEPLOYMENT_NAMESPACE

#endif /* CompIntrDesc_HANDLER_C */

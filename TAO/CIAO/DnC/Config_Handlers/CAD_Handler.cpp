// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "CAD_Handler.h"
#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CompPkgDesc_Handler.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the connection resource deployment description and populate it
void CAD_Handler::process_ComponentAssemblyDescription
(::Deployment::ComponentAssemblyDescription& cad)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_sequence_local<Deployment::SubcomponentInstantiationDescriptions>
         (this->doc_, this->iter_, node,
          node_name, "instance", cad.instance,
          this, &CAD_Handler::process_instance));
      else if
        (process_sequence_local<Deployment::AssemblyConnectionDescription>
         (this->doc_, this->iter_, node,
          node_name, "connection", cad.connection,
          this, &CAD_Handler::process_connection));
      else if
        (process_sequence_local<Deployment::AssemblyPropertyMappings>
         (this->doc_, this->iter_, node,
          node_name, "externalProperty", cad.externalProperty,
          this, &CAD_Handler::process_property));
      else
        {
          break;
        }
    }

  this->update_spe_refs (cad);
  this->update_spr_refs (cad);

  return;
}

void CAD_Handler::update_spe_refs
(Deployment::ComponentAssemblyDescription& cad)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int value;
  ACE_TString ref_name;

  for (x = 0; x < cad.connection.length (); ++x)
    {
      for (y = 0; y < cad.connection[x].internalEndpoint.length (); ++y)
        {
          ref_value = cad.connection[x].internalEndpoint[y].instanceRef;
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, value) == 0)
                {
                  cad.connection[x].internalEndpoint[y].instanceRef= value;
                }
            }
        }
    }
}

void CAD_Handler::update_spr_refs
(Deployment::ComponentAssemblyDescription& cad)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int value;
  ACE_TString ref_name;

  for (x = 0; x < cad.externalProperty.length (); ++x)
    {
      for (y = 0; y < cad.externalProperty[x].delegatesTo.length (); ++y)
        {
          ref_value = cad.externalProperty[x].delegatesTo[y].instanceRef;
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, value) == 0)
                {
                  cad.externalProperty[x].delegatesTo[y].instanceRef = value;
                }
            }
        }
    }
}

void CAD_Handler::process_instance (DOMNodeIterator* iter,
                                    Deployment::SubcomponentInstantiationDescription& sid)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string (iter, node_name, "name", sid.name));
      else if
        (process_sequence_remote<Deployment::ComponentPackageDescription, CompPkgDesc_Handler>
         (this->doc_, this->iter_, node,
          node_name, "Package", sid.Package,
          &CompPkgDesc_Handler::process_ComponentPackageDescription, this->id_map_));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "configProperty", sid.configProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else if
        (process_sequence_common<Deployment::Requirement>(this->doc_, this->iter_, node,
                                                          node_name, "selectRequirement", sid.selectRequirement,
                                                          &Requirement_Handler::process_Requirement,
                                                          this->id_map_));
      else if
        (process_sequence_remote<Deployment::ComponentPackageReference, CPR_Handler>
         (this->doc_, this->iter_, node,
          node_name, "reference", sid.reference,
          &CPR_Handler::process_ComponentPackageReference, this->id_map_));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void CAD_Handler::process_connection (DOMNodeIterator* iter,
                                      Deployment::AssemblyConnectionDescription& acd)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string(iter, node_name, "name", acd.name));
      else if
        (process_sequence_common<Deployment::Requirement>(node->getOwnerDocument(), iter, node,
                                                          node_name, "deployRequirement", acd.deployRequirement,
                                                          &Requirement_Handler::process_Requirement,
                                                          this->id_map_));
      else if
        (process_sequence_common<Deployment::ComponentExternalPortEndpoint>
         (node->getOwnerDocument(), iter, node,
          node_name, "externalEndpoint", acd.externalEndpoint,
          &CEPE_Handler::process_ComponentExternalPortEndpoint,
          this->id_map_));
      else if
        (process_sequence_local<Deployment::SubcomponentPortEndpoint>
         (node->getOwnerDocument(), iter, node,
          node_name, "internalEndpoint", acd.internalEndpoint,
          this, &CAD_Handler::process_spe));
      else if
        (process_sequence_common<Deployment::ExternalReferenceEndpoints>
         (node->getOwnerDocument(), iter, node,
          node_name, "externalReference", acd.externalReference,
          &ERE_Handler::process_ExternalReferenceEndpoints,
          this->id_map_));
      else
        {
          iter->previousNode();
          return;
        }
    }
}
      
void CAD_Handler::process_spe (DOMNodeIterator* iter,
                               Deployment::SubcomponentPortEndpoint& spe)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string (iter, node_name, "portName", spe.portName));
      else if
        (process_reference(node, node_name, "instance",
                           spe.instanceRef,
                           this->index_, this->idref_map_));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void CAD_Handler::process_spr (DOMNodeIterator* iter,
                               Deployment::SubcomponentPropertyReference& spr)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string (iter, node_name, "propertyName", spr.propertyName));
      else if
        (process_reference(node, node_name, "instance",
                           spr.instanceRef,
                           this->index_, this->idref_map_));
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void CAD_Handler::process_property(DOMNodeIterator* iter,
                                   Deployment::AssemblyPropertyMapping& apm)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (false);
      else if
        (process_string (iter, node_name, "name", apm.name));
      else if
        (process_string (iter, node_name, "externalName", apm.externalName));
      else if
        (process_sequence_local<Deployment::SubcomponentPropertyReference>
         (node->getOwnerDocument(), iter, node,
          node_name, "delegatesTo", apm.delegatesTo,
          this, &CAD_Handler::process_spr));
      else
        {
          iter->previousNode();
          return;
        }
    }
}
      
END_DEPLOYMENT_NAMESPACE

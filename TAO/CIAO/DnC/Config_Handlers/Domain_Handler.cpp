// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Domain_Handler.h"
#include "Property_Handler.h"
#include "SP_Handler.h"

#include "Process_Element.h"
#include "Process_Basic_Type.h"

#include <iostream>
#include "string.h"

using std::cerr;
using std::endl;

BEGIN_DEPLOYMENT_NAMESPACE

/// handle the Domain type and populate the IDL structure
void Domain_Handler::process_domain (Deployment::Domain& domain)
{
  for (DOMNode* node = this->iter_->nextNode();
       node != 0;
       node = this->iter_->nextNode())
    {
      XStr node_name (node->getNodeName ());

      if (node_name == XStr (ACE_TEXT ("Deployment:Domain")))
        {
        }
      else if
        (process_string(this->iter_, node_name, "UUID", domain.UUID));
      else if
        (process_string(this->iter_, node_name, "label", domain.label));
      else if
        (process_sequence_local<Deployment::SharedResource>(this->doc_, this->iter_, node,
                                                            node_name, "sharedResource", domain.sharedResource,
                                                            this, &Domain_Handler::process_sr));
      else if
        (process_sequence_local<Deployment::Node>(this->doc_, this->iter_, node,
                                            node_name, "node", domain.node,
                                            this, &Domain_Handler::process_node));
      else if
        (process_sequence_local<Deployment::Interconnect>(this->doc_, this->iter_, node,
                                                          node_name, "interconnect", domain.interconnect,
                                                          this, &Domain_Handler::process_interconnect));
      else if
        (process_sequence_local<Deployment::Bridge>(this->doc_, this->iter_, node,
                                                    node_name, "bridge", domain.bridge,
                                                    this, &Domain_Handler::process_bridge));
      else if
        (process_sequence_common<Deployment::Property>(this->doc_, this->iter_, node,
                                                       node_name, "infoProperty", domain.infoProperty,
                                                       &Property_Handler::process_Property,
                                                       this->id_map_));
      else
        {
          // ??? How did we get here ???
          ACE_THROW (CORBA::INTERNAL());
        }
    }

  this->update_node_refs (domain);
  this->update_ic_refs (domain);
  this->update_bridge_refs (domain);
  this->update_sr_refs (domain);

  return;
}

void Domain_Handler::process_node (DOMNodeIterator* iter,
                                   Deployment::Node& domain_node)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:Node")))
        {
        }
      else if
        (process_string(iter, node_name, "name", domain_node.name));
      else if
        (process_string(iter, node_name, "label", domain_node.label));
      else if
        (process_sequence_local<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                      node_name, "resource", domain_node.resource,
                                                      this, &Domain_Handler::process_resource));
      else if
        (process_reference_seq(node, node_name, "connection",
                               domain_node.connectionRef,
                               this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_reference_seq(node, node_name, "sharedResource",
                               domain_node.sharedResourceRef,
                               this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Domain_Handler::process_resource 
(DOMNodeIterator* iter, Deployment::Resource& domain_resource)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:Resource")))
        {
        }
      else if
        (process_string(iter, node_name, "name", domain_resource.name));
      else if
        (process_string_seq(iter, node_name, "resourceType", domain_resource.resourceType));
      else if
        (process_sequence_common<Deployment::SatisfierProperty>(node->getOwnerDocument(), iter, node,
                                                                node_name, "property", domain_resource.property,
                                                                &SP_Handler::process_SatisfierProperty,
                                                                this->id_map_));
      else
        {
          node = iter->previousNode ();
          break;
        }
    }
}

void Domain_Handler::process_bridge 
(DOMNodeIterator* iter, Deployment::Bridge& domain_bridge)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());
          
      if (node_name == XStr
               (ACE_TEXT ("Deployment:Bridge")))
        {
        }
      else if
        (process_string(iter, node_name, "name", domain_bridge.name));
      else if
        (process_string(iter, node_name, "label", domain_bridge.label));
      else if
        (process_reference_seq(node, node_name, "connect",
                               domain_bridge.connectRef,
                               this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_sequence_local<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                      node_name, "resource", domain_bridge.resource,
                                                      this, &Domain_Handler::process_resource));
      else
        {
          iter->previousNode();
          return;
        }
    }
}


void Domain_Handler::process_interconnect (DOMNodeIterator* iter,
                                           Deployment::Interconnect& domain_ic)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());

      if (node_name == XStr
               (ACE_TEXT ("Deployment:Interconnect")))
        {
        }
      else if
        (process_string(iter, node_name, "name", domain_ic.name));
      else if
        (process_string(iter, node_name, "label", domain_ic.label));
      else if
        (process_reference_seq(node, node_name, "connect",
                           domain_ic.connectRef,
                           this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_reference_seq(node, node_name, "connection",
                               domain_ic.connectionRef,
                               this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_sequence_local<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                      node_name, "resource", domain_ic.resource,
                                                      this, &Domain_Handler::process_resource));
      else
        {
          iter->previousNode();
          return;
        }
    }
}


void Domain_Handler::process_sr 
(DOMNodeIterator* iter,
 Deployment::SharedResource& domain_sr)
{
  for (DOMNode* node = iter->nextNode();
       node != 0;
       node = iter->nextNode ())
    {
      XStr node_name (node->getNodeName());
          
      if (node_name == XStr
               (ACE_TEXT ("Deployment:SharedResource")))
        {
        }
      else if
        (process_string(iter, node_name, "name", domain_sr.name));
      else if
        (process_string_seq(iter, node_name, "resourceType", domain_sr.resourceType));
      else if
        (process_reference_seq(node, node_name, "node",
                               domain_sr.nodeRef,
                               this->index_, this->idref_map_))
        {
          this->index_ = this->index_ + 1;
        }
      else if
        (process_sequence_common<Deployment::SatisfierProperty>(node->getOwnerDocument(), iter, node,
                                                                node_name, "property", domain_sr.property,
                                                                &SP_Handler::process_SatisfierProperty,
                                                                this->id_map_));
      // TODO: What about "any SatisfierProperty::value"
      else
        {
          iter->previousNode();
          return;
        }
    }
}

void Domain_Handler::update_node_refs (Deployment::Domain& domain)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < domain.node.length (); ++x)
    {
      for (y = 0; y < domain.node[x].connectionRef.length (); ++y)
        {
          ref_value =  domain.node[x].connectionRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.node[x].connectionRef[y] = orig_value;
                }
            }
        }

      for (y = 0; y < domain.node[x].sharedResourceRef.length (); ++y)
        {
          ref_value = domain.node[x].sharedResourceRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.node[x].sharedResourceRef[y] = orig_value;
                }
            }
        }

    }
}

void Domain_Handler::update_ic_refs (Deployment::Domain& domain)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < domain.interconnect.length (); ++x)
    {
      for (y = 0; y < domain.interconnect[x].connectionRef.length (); ++y)
        {
          ref_value = domain.interconnect[x].connectionRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.interconnect[x].connectionRef[y] = orig_value;
                }
            }
        }

      for (y = 0; y < domain.interconnect[x].connectRef.length (); ++y)
        {
          ref_value = domain.interconnect[x].connectRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.interconnect[x].connectRef[y] = orig_value;
                }
            }
        }
    }
}

void Domain_Handler::update_bridge_refs (Deployment::Domain& domain)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < domain.bridge.length (); ++x)
    {
      for (y = 0; y < domain.bridge[x].connectRef.length (); ++y)
        {
          ref_value = domain.bridge[x].connectRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.bridge[x].connectRef[y] = orig_value;
                }
            }
        }
    }
}

void Domain_Handler::update_sr_refs (Deployment::Domain& domain)
{
  CORBA::ULong x;
  CORBA::ULong y;
  int ref_value;
  int orig_value;
  ACE_TString ref_name;

  for (x = 0; x < domain.sharedResource.length (); ++x)
    {
      for (y = 0; y < domain.sharedResource[x].nodeRef.length (); ++y)
        {
          ref_value = domain.sharedResource[x].nodeRef[y];
          if (idref_map_.find (ref_value, ref_name) == 0)
            {
              if (id_map_.find (ref_name, orig_value) == 0)
                {
                  domain.sharedResource[x].nodeRef[y] = orig_value;
                }
            }
        }
    }
}

END_DEPLOYMENT_NAMESPACE

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


namespace CIAO
{
  namespace Config_Handler
  {
    Domain_Handler::Domain_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true),
        index_ (0)
    {}

    Domain_Handler::Domain_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter),
        release_ (release)
    {}

    Domain_Handler::~Domain_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

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
            (process_sequence<Deployment::SharedResource>(this->doc_, this->iter_, node,
                                                          node_name, "sharedResource", domain.sharedResource,
                                                          this, &Domain_Handler::process_sr,
                                                          this->id_map_));
          else if
            (process_sequence<Deployment::Node>(this->doc_, this->iter_, node,
                                                node_name, "node", domain.node,
                                                this, &Domain_Handler::process_node,
                                                this->id_map_));
          else if
            (process_sequence<Deployment::Interconnect>(this->doc_, this->iter_, node,
                                                        node_name, "interconnect", domain.interconnect,
                                                        this, &Domain_Handler::process_interconnect,
                                                        this->id_map_));
          else if
            (process_sequence<Deployment::Bridge>(this->doc_, this->iter_, node,
                                                  node_name, "bridge", domain.bridge,
                                                  this, &Domain_Handler::process_bridge,
                                                  this->id_map_));
          else if
            (process_sequence<Deployment::Property>(this->doc_, this->iter_, node,
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

          if (false);
          else if
            (process_string(iter, node_name, "name", domain_node.name));
          else if
            (process_string(iter, node_name, "label", domain_node.label));
          else if
            (process_sequence<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                    node_name, "resource", domain_node.resource,
                                                    this, &Domain_Handler::process_resource,
                                                    this->id_map_));
          else if
            (process_reference(node, node_name, "connection",
                               domain_node.connectionRef,
                               this->index_, this->idref_map_));
          else if
            (process_reference(node, node_name, "sharedResource",
                               domain_node.sharedResourceRef,
                               this->index_, this->idref_map_));
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
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = iter->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              domain_resource.name = XMLString::transcode 
                 (text->getNodeValue ());
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              node = iter->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              CORBA::ULong resource_type_length =
                domain_resource.resourceType.length ();
              domain_resource.resourceType.length (resource_type_length + 1);
              domain_resource.resourceType[resource_type_length] =
                 XMLString::transcode (text->getNodeValue ());
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong sp_length 
                     (domain_resource.property.length ());
                  domain_resource.property.length (sp_length + 1);
                  if (length == 1)
                    {
                      SP_Handler::process_SatisfierProperty
                        (iter,
                         domain_resource.property[sp_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_satisfier_property 
                        (named_node_map, node->getOwnerDocument(),
                         iter, sp_length,
                         domain_resource.property[sp_length]);
                    }
                }
            }
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
          
          if (false);
          else if
            (process_string(iter, node_name, "name", domain_bridge.name));
          else if
            (process_string(iter, node_name, "label", domain_bridge.label));
          else if
            (process_reference(node, node_name, "connect",
                               domain_bridge.connectRef,
                               this->index_, this->idref_map_));
          else if
            (process_sequence<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                    node_name, "resource", domain_bridge.resource,
                                                    this, &Domain_Handler::process_resource,
                                                    this->id_map_));
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_interconnect (
          DOMNodeIterator* iter, Deployment::Interconnect& domain_ic)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());

          if (false);
          else if
            (process_string(iter, node_name, "name", domain_ic.name));
          else if
            (process_string(iter, node_name, "label", domain_ic.label));
          else if
            (process_reference(node, node_name, "connect",
                               domain_ic.connectRef,
                               this->index_, this->idref_map_));
          else if
            (process_reference(node, node_name, "connection",
                               domain_ic.connectionRef,
                               this->index_, this->idref_map_));
          else if
            (process_sequence<Deployment::Resource>(node->getOwnerDocument(), iter, node,
                                                    node_name, "resource", domain_ic.resource,
                                                    this, &Domain_Handler::process_resource,
                                                    this->id_map_));
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
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_sr_name (text->getNodeValue (), domain_sr);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_sr_resource_type 
                  (text->getNodeValue (), domain_sr);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              CORBA::ULong node_ref_length = (domain_sr.nodeRef.length ());
              domain_sr.nodeRef.length (node_ref_length + 1);
              domain_sr.nodeRef[node_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong sp_length 
                     (domain_sr.property.length ());
                  domain_sr.property.length (sp_length + 1);
                  if (length == 1)
                    {
                      SP_Handler::process_SatisfierProperty
                        (iter,
                         domain_sr.property[sp_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_satisfier_property 
                        (named_node_map, node->getOwnerDocument(),
                         iter, sp_length,
                         domain_sr.property[sp_length]);
                    }
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    ////////////////////////////////

    void Domain_Handler::process_sr_resource_type 
         (const XMLCh* resource_type,
          Deployment::SharedResource& domain_sr)
    {
      if (resource_type)
        {
          CORBA::ULong i (domain_sr.resourceType.length ());
          domain_sr.resourceType.length (i + 1);
          domain_sr.resourceType[i] = XMLString::transcode (resource_type);
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
  }
}

// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Domain_Handler.h"
#include "Property_Handler.h"
#include "SP_Handler.h"

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

    void Domain_Handler::dump (Deployment::Domain& domain)
    {
      ACE_DEBUG ((LM_DEBUG, "UUID: %s \n", domain.UUID.in ()));
      ACE_DEBUG ((LM_DEBUG, "label: %s \n", domain.label.in ()));
      this->dump_nodes (domain);
      this->dump_ics (domain);
      this->dump_bridges (domain);
      this->dump_srs (domain);
    }

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
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_uuid (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              this->process_sr_element (node, this->doc_,
                                        this->iter_,
                                        domain);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              this->process_node_element (node, this->doc_,
                                          this->iter_,
                                          domain);
            }
          else if (node_name == XStr (ACE_TEXT ("interconnect")))
            {
              this->process_ic_element (node, this->doc_,
                                        this->iter_,
                                        domain);
            }
          else if (node_name == XStr (ACE_TEXT ("bridge")))
            {
              this->process_bridge_element (node, this->doc_,
                                            this->iter_,
                                            domain);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              this->process_property_element (node, this->doc_,
                                              this->iter_,
                                              domain);
            }
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

    // handle the sr element
    void Domain_Handler::process_sr_element (DOMNode* node,
                                             DOMDocument* doc,
                                             DOMNodeIterator* iter,
                                             Deployment::Domain& domain)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
          int length = named_node_map->getLength ();
          CORBA::ULong i (domain.sharedResource.length ());
          domain.sharedResource.length (i + 1);
          if (length == 1)
            {
              this->process_sr
                (doc, iter, domain.sharedResource[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_sr
                (named_node_map, doc,
                 iter, i, domain.sharedResource[i]);
            }
        }      
    }

    // handle the node element
    void Domain_Handler::process_node_element (DOMNode* node,
                                               DOMDocument* doc,
                                               DOMNodeIterator* iter,
                                               Deployment::Domain& domain)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
          int length = named_node_map->getLength ();
          CORBA::ULong i (domain.node.length ());
          domain.node.length (i + 1);
          if (length == 1)
            {
              this->process_node (doc,
                                  iter,
                                  domain.node[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_node
                (named_node_map, doc,
                 iter, i, domain.node[i]);
            }
        }
    }

    // handle the interconnect element
    void Domain_Handler::process_ic_element (DOMNode* node,
                                             DOMDocument* doc,
                                             DOMNodeIterator* iter,
                                             Deployment::Domain& domain)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
          int length = named_node_map->getLength ();
          CORBA::ULong i (domain.interconnect.length ());
          domain.interconnect.length (i + 1);
          if (length == 1)
            {
              this->process_interconnect
                 (doc, iter, domain.interconnect[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_ic
                (named_node_map, doc,
                 iter, i, domain.interconnect[i]);
            }
        }
    }

    // handle the bridge element
    void Domain_Handler::process_bridge_element (DOMNode* node,
                                                 DOMDocument* doc,
                                                 DOMNodeIterator* iter,
                                                 Deployment::Domain& domain)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
          int length = named_node_map->getLength ();
          CORBA::ULong i (domain.interconnect.length ());
          domain.interconnect.length (i + 1);
          if (length == 1)
            {
              this->process_bridge
                 (doc, iter, domain.bridge[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_bridge
                (named_node_map, doc,
                 iter, i, domain.bridge[i]);
            }
        }
    }

    // handle the sr element
    void Domain_Handler::process_property_element (DOMNode* node,
                                                   DOMDocument* doc,
                                                   DOMNodeIterator* iter,
                                                   Deployment::Domain& domain)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
          int length = named_node_map->getLength ();
          CORBA::ULong i (domain.interconnect.length ());
          domain.interconnect.length (i + 1);
          if (length == 1)
            {
              Property_Handler::process_Property 
                 (iter, domain.infoProperty[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, domain.infoProperty[i]);
            }
        }
    }

    /// handle uuid attribute
    void Domain_Handler::process_uuid (const XMLCh* uuid,
                                       Deployment::Domain& domain)
    {
      if (uuid)
        {
          CORBA::String_var value (XMLString::transcode (uuid));
          domain.UUID = value.in ();
        }
    }

    /// handle label attribute
    void Domain_Handler::process_label (const XMLCh* label,
                                        Deployment::Domain& domain)
    {
      if (label)
        {
          CORBA::String_var value (XMLString::transcode (label));
          domain.label = value.in ();
        }
    }

    void Domain_Handler::process_node (DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       Deployment::Node& domain_node)
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
              this->process_node_name (text->getNodeValue (), domain_node);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node_label (text->getNodeValue (), domain_node);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
                  int length = named_node_map->getLength ();
                  CORBA::ULong resource_length 
                     (domain_node.resource.length ());
                  domain_node.resource.length (resource_length + 1);
                  if (length == 1)
                    {
                      this->process_resource 
                        (doc, iter, domain_node.resource[resource_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_resource 
                        (named_node_map, doc,
                         iter, resource_length,
                         domain_node.resource[resource_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length = 
                domain_node.connectionRef.length ();
              domain_node.connectionRef.length (connection_ref_length + 1);
              domain_node.connectionRef[connection_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
                  this->process_refs (named_node_map);
                }
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              CORBA::ULong shared_resource_ref_length = 
                domain_node.sharedResourceRef.length ();
              domain_node.sharedResourceRef.length 
                (shared_resource_ref_length + 1);
              domain_node.sharedResourceRef[shared_resource_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
                  this->process_refs (named_node_map);
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void Domain_Handler::process_resource 
           (DOMDocument* doc,
            DOMNodeIterator* iter, Deployment::Resource& domain_resource)
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
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
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
                        (named_node_map, doc,
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

    void Domain_Handler::process_attributes_for_resource 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Resource& domain_resource)
    {
      int length = named_node_map->getLength ();
      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              this->process_resource 
                 (doc, iter, domain_resource);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_resource(href_doc,
                                     href_iter,
                                     domain_resource);
            }
        }

      return;
    }

    void Domain_Handler::process_bridge 
           (DOMDocument* doc,
            DOMNodeIterator* iter, Deployment::Bridge& domain_bridge)
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
              this->process_bridge_name 
                 (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_bridge_label 
                  (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length = domain_bridge.resource.length ();
              domain_bridge.resource.length (resource_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      this->process_resource 
                        (doc, iter, domain_bridge.resource[resource_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_resource 
                        (named_node_map, doc,
                         iter, resource_length,
                         domain_bridge.resource[resource_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              CORBA::ULong connect_ref_length = 
                 domain_bridge.connectRef.length ();
              domain_bridge.connectRef.length (connect_ref_length + 1);
              domain_bridge.connectRef[connect_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_interconnect (
          DOMDocument* doc,
          DOMNodeIterator* iter, Deployment::Interconnect& domain_ic)
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
              this->process_ic_name (text->getNodeValue (), domain_ic);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ic_label (text->getNodeValue (), domain_ic);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length = (domain_ic.resource.length ());
              domain_ic.resource.length (resource_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      this->process_resource 
                        (doc, iter, domain_ic.resource[resource_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_resource 
                        (named_node_map, doc,
                         iter, resource_length,
                         domain_ic.resource[resource_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              CORBA::ULong connect_ref_length = 
                  (domain_ic.connectRef.length ());
              domain_ic.connectRef.length (connect_ref_length + 1);
              domain_ic.connectRef[connect_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length = 
                 (domain_ic.connectionRef.length ());
              domain_ic.connectionRef.length (connection_ref_length + 1);
              domain_ic.connectionRef[connection_ref_length] = 0;
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  this->process_refs (named_node_map);
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_sr 
                (DOMDocument* doc, DOMNodeIterator* iter,
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
                  //auto_ptr<DOMNamedNodeMap> cleanup_map (named_node_map);
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
                        (named_node_map, doc,
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

    void Domain_Handler::process_node_name (const XMLCh* name,
                                            Deployment::Node& domain_node)
    {
      if (name)
        {
          domain_node.name = XMLString::transcode (name);
        }
    }

    void Domain_Handler::process_sr_name (const XMLCh* name,
                                          Deployment::SharedResource& domain_sr)
    {
      if (name)
        {
          domain_sr.name = XMLString::transcode (name);
        }
    }

    void Domain_Handler::process_ic_name (const XMLCh* name,
                                          Deployment::Interconnect& domain_ic)
    {
      if (name)
        {
          domain_ic.name = XMLString::transcode (name);
        }
    }

    void Domain_Handler::process_bridge_name (const XMLCh* name,
                                              Deployment::Bridge& domain_bridge)
    {
      if (name)
        {
          domain_bridge.name = XMLString::transcode (name);
        }
    }

    void Domain_Handler::process_node_label (const XMLCh* label,
                                             Deployment::Node& domain_node)
    {
      if (label)
        {
          domain_node.label = XMLString::transcode (label);
        }
    }

    void Domain_Handler::process_bridge_label 
          (const XMLCh* label,
           Deployment::Bridge& domain_bridge)
    {
      if (label)
        {
          domain_bridge.label = XMLString::transcode (label);
        }
    }

    void Domain_Handler::process_ic_label 
          (const XMLCh* label,
           Deployment::Interconnect& domain_ic)
    {
      if (label)
        {
          domain_ic.label = XMLString::transcode (label);
        }
    }

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

    DOMDocument* Domain_Handler::create_document (const char *url)
    {

      xercesc::XMLPlatformUtils::Initialize();
      static const XMLCh gLS[] = { xercesc::chLatin_L,
                                   xercesc::chLatin_S,
                                   xercesc::chNull };

      DOMImplementation* impl
        = DOMImplementationRegistry::getDOMImplementation(gLS);

      DOMBuilder* parser =
        ((DOMImplementationLS*)impl)->createDOMBuilder
              (DOMImplementationLS::MODE_SYNCHRONOUS, 0);

      // Discard comment nodes in the document
      parser->setFeature (XMLUni::fgDOMComments, false);

      // Disable datatype normalization. The XML 1.0 attribute value
      // normalization always occurs though.
      parser->setFeature (XMLUni::fgDOMDatatypeNormalization, true);

      // Do not create EntityReference nodes in the DOM tree. No
      // EntityReference nodes will be created, only the nodes
      // corresponding to their fully expanded sustitution text will be
      // created.
      parser->setFeature (XMLUni::fgDOMEntities, false);

      // Perform Namespace processing.
      parser->setFeature (XMLUni::fgDOMNamespaces, true);

      // Perform Validation
      parser->setFeature (XMLUni::fgDOMValidation, true);

      // Do not include ignorable whitespace in the DOM tree.
      parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);

      // Enable the parser's schema support.
      parser->setFeature (XMLUni::fgXercesSchema, true);

      // Enable full schema constraint checking, including checking which
      // may be time-consuming or memory intensive. Currently, particle
      // unique attribution constraint checking and particle derivation
      // restriction checking are controlled by this option.
      parser->setFeature (XMLUni::fgXercesSchemaFullChecking, true);

      // The parser will treat validation error as fatal and will exit.
      parser->setFeature (XMLUni::fgXercesValidationErrorAsFatal, true);


      DOMDocument* doc = parser->parseURI (url);
      ACE_TString root_node_name;
      root_node_name = XMLString::transcode 
          (doc->getDocumentElement ()->getNodeName ());

      return doc;
    }

    void Domain_Handler::dump_nodes (Deployment::Domain& domain)
    {
      for (CORBA::ULong i = 0; i < domain.node.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\nNode %d: \n", i + 1));
          ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                      domain.node[i].name.in ()));
          for (CORBA::ULong j = 0; j < domain.node[i].resource.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.node[i].resource[j].name.in ()));
              for (CORBA::ULong k = 0;
                   k < domain.node[i].resource[j].resourceType.length (); ++k)
                {
                  ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                     domain.node[i].resource[j].resourceType[k].in ()));
                }
            }

          for (CORBA::ULong j = 0;
               j < domain.node[i].connectionRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
              int value = domain.node[i].connectionRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.interconnect[value].name.in ()));
            }

          for (CORBA::ULong j = 0;
               j < domain.node[i].sharedResourceRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     SharedResource %d: \n", j + 1));
              int value = domain.node[i].sharedResourceRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.sharedResource[value].name.in ()));
            }

        }
    }

    void Domain_Handler::dump_ics (Deployment::Domain& domain)
    {
      for (CORBA::ULong i = 0; i < domain.interconnect.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\nInterconnect %d: \n", i + 1));
          ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                      domain.interconnect[i].name.in ()));
          ACE_DEBUG ((LM_DEBUG, "     Label: %s \n",
                      domain.interconnect[i].label.in ()));
          for (CORBA::ULong j = 0;
               j < domain.interconnect[i].resource.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.interconnect[i].resource[j].name.in ()));
              for (CORBA::ULong k = 0;
                k < domain.interconnect[i].resource[j].resourceType.length ();
                ++k)
                {
                  ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                   domain.interconnect[i].resource[j].resourceType[k].in ()));
                }
            }

          for (CORBA::ULong j = 0;
               j < domain.interconnect[i].connectionRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
              int value = domain.interconnect[i].connectionRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.bridge[value].name.in ()));
            }

          for (CORBA::ULong j = 0;
               j < domain.interconnect[i].connectRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     SharedResource %d: \n", j + 1));
              int value = domain.interconnect[i].connectRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.node[value].name.in ()));
            }

        }
    }

    void Domain_Handler::dump_bridges (Deployment::Domain& domain)
    {
      for (CORBA::ULong i = 0; i < domain.bridge.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\nBridge %d: \n", i + 1));
          ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                      domain.bridge[i].name.in ()));
          ACE_DEBUG ((LM_DEBUG, "     Label: %s \n",
                      domain.bridge[i].label.in ()));
          for (CORBA::ULong j = 0;
               j < domain.bridge[i].resource.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.bridge[i].resource[j].name.in ()));
              for (CORBA::ULong k = 0;
                   k < domain.bridge[i].resource[j].resourceType.length ();
                   ++k)
                {
                  ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                    domain.bridge[i].resource[j].resourceType[k].in ()));
                }
            }

          for (CORBA::ULong j = 0; j < domain.bridge[i].connectRef.length ();
               ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
              int value = domain.bridge[i].connectRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.interconnect[value].name.in ()));
            }
        }
    }

    void Domain_Handler::dump_srs (Deployment::Domain& domain)
    {
      for (CORBA::ULong i = 0; i < domain.sharedResource.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\nSharedResource %d: \n", i + 1));
          ACE_DEBUG ((LM_DEBUG, "     Name: %s \n",
                      domain.sharedResource[i].name.in ()));
          for (CORBA::ULong k = 0;
               k < domain.sharedResource[i].resourceType.length (); ++k)
            {
              ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n",
                domain.sharedResource[i].resourceType[k].in ()));
            }

          for (CORBA::ULong j = 0;
               j < domain.sharedResource[i].nodeRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
              int value = domain.sharedResource[i].nodeRef[j];
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n",
                          domain.node[value].name.in ()));
            }
        }
    }

    void Domain_Handler::process_attributes_for_sr 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::SharedResource& domain_sr)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              this->process_sr (doc,
                                iter,
                                domain_sr);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_sr (href_doc,
                                       href_iter,
                                       domain_sr);
            }
        }

      return;
    }

    void Domain_Handler::process_attributes_for_node 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Node& domain_node)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              this->process_node (doc,
                                  iter,
                                  domain_node);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_node (href_doc,
                                  href_iter,
                                  domain_node);
            }
        }

      return;
    }

    void Domain_Handler::process_attributes_for_ic 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Interconnect& domain_ic)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              this->process_interconnect (doc,
                                          iter,
                                          domain_ic);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_interconnect (href_doc,
                                          href_iter,
                                          domain_ic);
            }
        }

      return;
    }

    void Domain_Handler::process_attributes_for_bridge 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Bridge& domain_bridge)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              this->process_bridge (doc,
                                    iter,
                                    domain_bridge);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              this->process_bridge (href_doc,
                                           href_iter,
                                           domain_bridge);
            }
        }

      return;
    }

    void Domain_Handler::process_attributes_for_property 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Property& domain_property)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              Property_Handler::process_Property (iter,
                                                  domain_property);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              Property_Handler::process_Property (href_iter,
                                                  domain_property);
            }
        }

      return;
    }

    void Domain_Handler::process_attributes_for_satisfier_property 
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::SatisfierProperty& domain_satisfier_property)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename
             (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue =  XMLString::transcode
             (attribute_node->getNodeValue ());

          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              SP_Handler::process_SatisfierProperty
                 (iter,
                  domain_satisfier_property);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              ACE_TString doc_path =
               XMLString::transcode ( doc->getDocumentURI ());
              result.makeRelativeTo
                 (XMLString::transcode (doc_path.c_str ()));
              ACE_TString final_url =
               XMLString::transcode (result.getURLText ());

              DOMDocument* href_doc;

              if (xml_url.isRelative ())
                {
                  href_doc = this->create_document 
                       (final_url.c_str ());
                }
              else
                {
                  href_doc = this->create_document 
                       (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse (href_doc);
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                                              (root,
                                               filter,
                                               0,
                                               true);
              href_iter->nextNode ();
              SP_Handler::process_SatisfierProperty
                 (href_iter,
                  domain_satisfier_property);
            }
        }

      return;
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

    void Domain_Handler::process_refs (DOMNamedNodeMap* named_node_map)
    {
      int length = named_node_map->getLength ();

      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          ACE_TString aceattrnodevalue = XMLString::transcode
             (attribute_node->getNodeValue ());
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->index_ = this->index_ + 1;
              idref_map_.bind (this->index_, aceattrnodevalue);
            }
        }
    }

  }
}

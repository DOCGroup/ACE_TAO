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
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      CORBA::ULong i (domain.sharedResource.length ());
                      domain.sharedResource.length (i + 1);
                      this->process_domain_sr 
                        (this->doc_, this->iter_, domain.sharedResource[i]);
                    }
                  else if (length > 1)
                    {
                      CORBA::ULong i (domain.sharedResource.length ());
                      domain.sharedResource.length (i + 1);
                      this->process_attributes_for_sr
                        (named_node_map, this->doc_,
                         this->iter_, domain.sharedResource[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      CORBA::ULong i (domain.node.length ());
                      domain.node.length (i + 1);
                      this->process_domain_node (this->doc_,
                                                 this->iter_,
                                                 domain.node[i]);
                    }
                  else if (length > 1)
                    {
                      CORBA::ULong i (domain.node.length ());
                      domain.node.length (i + 1);
                      this->process_attributes_for_node
                        (named_node_map, this->doc_,
                         this->iter_, domain.node[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("interconnect")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      CORBA::ULong i (domain.interconnect.length ());
                      domain.interconnect.length (i + 1);
                      this->process_domain_interconnect 
                         (this->doc_, this->iter_, domain.interconnect[i]);
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename 
                             (attribute_node->getNodeName ());
                          ACE_TString aceattrnodevalue =  XMLString::transcode
                             (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              CORBA::ULong i (domain.interconnect.length ());
                              domain.interconnect.length (i + 1);
                              this->process_domain_interconnect 
                                 (this->doc_, this->iter_, 
                                  domain.interconnect[i]);
                              id_map_.bind (aceattrnodevalue, i);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.interconnect.length ());
                              domain.interconnect.length (i + 1);
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              XMLURL result (aceattrnodevalue.c_str ());
                              ACE_TString doc_path = 
                               XMLString::transcode ( doc_->getDocumentURI ());
                              result.makeRelativeTo 
                                 (XMLString::transcode (doc_path.c_str ()));
                              ACE_TString final_url = 
                               XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = 
                                    this->create_document (final_url.c_str ());
                                  DOMDocumentTraversal* traverse (href_doc);
                                  DOMNode* root = 
                                    (href_doc->getDocumentElement ());
                                  unsigned long filter =
                                    DOMNodeFilter::SHOW_ELEMENT |
                                    DOMNodeFilter::SHOW_TEXT;
                                  DOMNodeIterator* iter = 
                                    traverse->createNodeIterator (root,
                                                                  filter,
                                                                  0,
                                                                  true);
                                  iter->nextNode ();
                                  this->process_domain_interconnect 
                                     (href_doc, iter, domain.interconnect[i]);
                                }
                              else
                                {
                                  std::string url_string = 
                                    aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = 
                                    this->create_document (url_string.c_str ());
                                  DOMDocumentTraversal* traverse (href_doc);
                                  DOMNode* root = 
                                    (href_doc->getDocumentElement ());
                                  unsigned long filter =
                                    DOMNodeFilter::SHOW_ELEMENT |
                                    DOMNodeFilter::SHOW_TEXT;
                                  DOMNodeIterator* iter = 
                                    traverse->createNodeIterator (root,
                                                                  filter,
                                                                  0,
                                                                  true);
                                  iter->nextNode ();
                                  this->process_domain_interconnect 
                                     (href_doc, iter, domain.interconnect[i]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("bridge")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      CORBA::ULong i (domain.bridge.length ());
                      domain.bridge.length (i + 1);
                      this->process_domain_bridge (this->doc_,
                                                   this->iter_,
                                                   domain.bridge[i]);
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename 
                             (attribute_node->getNodeName ());
                          ACE_TString aceattrnodevalue =  XMLString::transcode
                             (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              CORBA::ULong i (domain.bridge.length ());
                              domain.bridge.length (i + 1);
                              this->process_domain_bridge (this->doc_,
                                                           this->iter_,
                                                           domain.bridge[i]);
                              id_map_.bind (aceattrnodevalue, i);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.bridge.length ());
                              domain.bridge.length (i + 1);
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              XMLURL result (aceattrnodevalue.c_str ());
                              ACE_TString doc_path = 
                               XMLString::transcode ( doc_->getDocumentURI ());
                              result.makeRelativeTo 
                                 (XMLString::transcode (doc_path.c_str ()));
                              ACE_TString final_url = 
                               XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = 
                                    this->create_document (final_url.c_str ());
                                  DOMDocumentTraversal* traverse (href_doc);
                                  DOMNode* root = 
                                    (href_doc->getDocumentElement ());
                                  unsigned long filter =
                                    DOMNodeFilter::SHOW_ELEMENT |
                                    DOMNodeFilter::SHOW_TEXT;
                                  DOMNodeIterator* iter = 
                                    traverse->createNodeIterator (root,
                                                                  filter,
                                                                  0,
                                                                  true);
                                  iter->nextNode ();
                                  this->process_domain_bridge 
                                    (href_doc,
                                     iter,
                                     domain.bridge[i]);
                                }
                              else
                                {
                                  std::string url_string = 
                                    aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = 
                                    this->create_document (url_string.c_str ());
                                  DOMDocumentTraversal* traverse (href_doc);
                                  DOMNode* root = 
                                    (href_doc->getDocumentElement ());
                                  unsigned long filter =
                                    DOMNodeFilter::SHOW_ELEMENT |
                                    DOMNodeFilter::SHOW_TEXT;
                                  DOMNodeIterator* iter = 
                                    traverse->createNodeIterator (root,
                                                                  filter,
                                                                  0,
                                                                  true);
                                  iter->nextNode ();
                                  this->process_domain_bridge 
                                    (href_doc,
                                     iter,
                                     domain.bridge[i]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              /*
              // Need to Handle this.
              //
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      CORBA::ULong i = (domain.infoProperty.length ());
                      domain.infoProperty.length (i + 1);
                      Property_Handler::process_Property 
                         (this->iter_,
                          domain.infoProperty[i]);
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename 
                             (attribute_node->getNodeName ());
                          ACE_TString aceattrnodevalue =  XMLString::transcode
                             (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              CORBA::ULong i = (domain.infoProperty.length ());
                              domain.infoProperty.length (i + 1);
                              Property_Handler::process_Property 
                                (this->iter_,
                                 domain.infoProperty[i]);
                              id_map_.bind (aceattrnodevalue, i);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i = 
                                (domain.infoProperty.length ());
                              domain.infoProperty.length (i + 1);
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              XMLURL result (aceattrnodevalue.c_str ());
                              ACE_TString doc_path = 
                               XMLString::transcode ( doc_->getDocumentURI ());
                              result.makeRelativeTo 
                                 (XMLString::transcode (doc_path.c_str ()));
                              ACE_TString final_url = 
                               XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = 
                                    this->create_document (final_url.c_str ());
                                  this->parse_property_href_doc 
                                     (href_doc, 
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain.infoProperty[i]);
                                }
                              else
                                {
                                  std::string url_string = 
                                    aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = 
                                    this->create_document 
                                      (url_string.c_str ());
                                  this->parse_property_href_doc 
                                    (href_doc, 
                                     DOMNodeFilter::SHOW_ELEMENT |
                                     DOMNodeFilter::SHOW_TEXT,
                                     domain.infoProperty[i]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
                */
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }

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

      return;
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

    void Domain_Handler::process_domain_node (DOMDocument* doc,
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
              CORBA::ULong resource_length (domain_node.resource.length ());
              domain_node.resource.length (resource_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      for (node = iter->nextNode();
                           node != 0;
                           node = iter->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              domain_node.resource[resource_length].name = 
                                XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr 
                             (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              CORBA::ULong resource_type_length = 
                                domain_node.resource[resource_length].
                                  resourceType.length ();
                              domain_node.resource[resource_length].
                                resourceType.length (resource_type_length + 1);
                              domain_node.resource[resource_length].
                                resourceType[resource_type_length] = 
                                XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                              /*
                              if (node->hasAttributes ())
                                {
                                  DOMNamedNodeMap* property_node_map = 
                                    node->getAttributes ();
                                  int attr_length = 
                                    property_node_map->getLength ();

                                  if (attr_length == 1)
                                    {
                                      CORBA::ULong property_length = 
                                        domain_node.resource[resource_length].
                                          property.length ();
                                      domain_node.resource[resource_length].
                                         property.length 
                                           (property_length + 1);
                                      SP_Handler::process_SatisfierProperty 
                                          (this->iter_,
                                           domain_node.resource
                                             [resource_length].
                                                property[property_length]);
                                    }
                                  else if (attr_length > 1)
                                    {
                                      for (int j = 0; j < attr_length; j++)
                                        {
                                          DOMNode* attribute_node = 
                                            property_node_map->item (j);
                                          XStr strattrnodename 
                                            (attribute_node->getNodeName ());
                                          ACE_TString aceattrnodevalue = 
                                             XMLString::transcode 
                                               (attribute_node->
                                                  getNodeValue ());
                                          if (strattrnodename == XStr 
                                              (ACE_TEXT ("xmi:id")))
                                            {
                                              CORBA::ULong property_length = 
                                                domain_node.
                                                  resource[resource_length].
                                                    property.length ();
                                              domain_node.
                                                 resource[resource_length].
                                                    property.length 
                                                      (property_length + 1);
                                              SP_Handler::
                                                process_SatisfierProperty 
                                                    (this->iter_,
                                                     domain_node.
                                                       resource
                                                        [resource_length].
                                                          property
                                                           [property_length]);
                                              id_map_.bind 
                                                 (aceattrnodevalue, 
                                                  property_length);
                                            }
                                          else if (strattrnodename == 
                                              XStr (ACE_TEXT ("href")))
                                            {
                                              CORBA::ULong property_length = 
                                                 domain_node.
                                                    resource[resource_length].
                                                       property.length ();
                                              domain_node.
                                                resource[resource_length].
                                                   property.length 
                                                      (property_length + 1);
                                              XMLURL xml_url = 
                                                aceattrnodevalue.c_str ();
                                              XMLURL result = 
                                                 aceattrnodevalue.c_str ();
                                              ACE_TString doc_path = 
                                                XMLString::transcode 
                                                   ( doc->getDocumentURI ());
                                              result.makeRelativeTo 
                                                 (XMLString::transcode 
                                                     (doc_path.c_str ()));
                                              ACE_TString final_url = 
                                                 XMLString::transcode 
                                                    (result.getURLText ());

                                              if (xml_url.isRelative ())
                                                {
                                                  DOMDocument* href_doc = 
                                                    this->create_document 
                                                         (final_url.c_str ());
                                                  this->
                                                    parse_sp_href_doc 
                                                       (href_doc, 
                                                        DOMNodeFilter::
                                                        SHOW_ELEMENT |
                                                        DOMNodeFilter::
                                                        SHOW_TEXT,
                                                        domain_node.
                                                        resource
                                                        [resource_length].
                                                        property
                                                        [property_length]);
                                                }
                                              else
                                                {
                                                  std::string url_string = 
                                                    aceattrnodevalue.c_str ();
                                                  DOMDocument* href_doc = 
                                                     this->create_document 
                                                       (url_string.c_str ());
                                                  this->parse_sp_href_doc 
                                                       (href_doc, 
                                                        DOMNodeFilter::
                                                        SHOW_ELEMENT |
                                                        DOMNodeFilter::
                                                        SHOW_TEXT,
                                                        domain_node.
                                                        resource
                                                        [resource_length].
                                                        property
                                                        [property_length]);
                                                }
                                            }
                                        }
                                    }
                                }
                              else
                                {
                                }
                                */
                            }
                          else
                            {
                              node = iter->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename = 
                             attribute_node->getNodeName ();
                          ACE_TString aceattrnodevalue = 
                            XMLString::transcode 
                              (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              XMLURL xml_url = aceattrnodevalue.c_str ();
                              XMLURL result = aceattrnodevalue.c_str ();
                              ACE_TString doc_path = XMLString::transcode 
                                 (doc->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode 
                                (doc_path.c_str ()));
                              ACE_TString final_url = XMLString::transcode 
                                 (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_node.resource[resource_length]);
                                }
                              else
                                {
                                  std::string url_string = 
                                    aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_node.resource[resource_length]);
                                }
                            }
                          else if (strattrnodename == XStr 
                             (ACE_TEXT ("xmi:id")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      domain_node.resource[resource_length].
                                         name = XMLString::transcode 
                                            (text->getNodeValue ());
                                      id_map_.bind 
                                        (aceattrnodevalue, resource_length);
                                    }
                                  else if (node_name == XStr 
                                     (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      CORBA::ULong resource_type_length = 
                                        domain_node.resource[resource_length].
                                          resourceType.length ();
                                      domain_node.resource[resource_length].
                                        resourceType.length 
                                           (resource_type_length + 1);
                                      domain_node.resource[resource_length].
                                         resourceType[resource_type_length] = 
                                           XMLString::transcode 
                                             (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr 
                                     (ACE_TEXT ("property")))
                                    {
                                      /*
                                      if (node->hasAttributes ())
                                        {
                                          property_node_map = node->getAttributes ();
                                          attr_length = property_node_map->getLength ();

                                          if (attr_length == 1)
                                            {
                                              property_length = domain_node.resource[resource_length].property.length ();
                                              domain_node.resource[resource_length].property.length (property_length + 1);
                                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                     domain_node.resource[resource_length].property[property_length]);
                                            }
                                          else if (attr_length > 1)
                                            {
                                              for (int j = 0; j < attr_length; j++)
                                                {
                                                  DOMNode* attribute_node = property_node_map->item (j);
                                                  strattrnodename = attribute_node->getNodeName ();
                                                  aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                                                  if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                                                    {
                                                      property_length = domain_node.resource[resource_length].property.length ();
                                                      domain_node.resource[resource_length].property.length (property_length + 1);
                                                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                             domain_node.resource[resource_length].property[property_length]);
                                                      id_map_.bind (aceattrnodevalue, property_length);
                                                    }
                                                  else if (strattrnodename == XStr (ACE_TEXT ("href")))
                                                    {
                                                      property_length = domain_node.resource[resource_length].property.length ();
                                                      domain_node.resource[resource_length].property.length (property_length + 1);
                                                      xml_url = aceattrnodevalue.c_str ();
                                                      result = aceattrnodevalue.c_str ();
                                                      doc_path = XMLString::transcode ( doc->getDocumentURI ());
                                                      result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                                                      final_url = XMLString::transcode (result.getURLText ());

                                                      if (xml_url.isRelative ())
                                                        {
                                                          DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_node.resource[resource_length].property[property_length]);
                                                        }
                                                      else
                                                        {
                                                          url_string = aceattrnodevalue.c_str ();
                                                          DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_node.resource[resource_length].property[property_length]);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                      else
                                        {
                                        }
                                        */
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length = 
                domain_node.connectionRef.length ();
              domain_node.connectionRef.length (connection_ref_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();

                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                         (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.connectionRef[connection_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              CORBA::ULong shared_resource_ref_length = 
                domain_node.sharedResourceRef.length ();
              domain_node.sharedResourceRef.length 
                (shared_resource_ref_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                         (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.sharedResourceRef
                            [shared_resource_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_bridge 
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
              this->process_bridge_name (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_bridge_label (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length = domain_bridge.resource.length ();
              domain_bridge.resource.length (resource_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      for (node = iter->nextNode();
                           node != 0;
                           node = iter->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              domain_bridge.resource[resource_length].name = 
                                XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              CORBA::ULong resource_type_length = 
                                domain_bridge.resource[resource_length].
                                  resourceType.length ();
                              domain_bridge.resource[resource_length].
                                resourceType.length (resource_type_length + 1);
                              domain_bridge.resource[resource_length].
                                resourceType[resource_type_length] = 
                                  XMLString::transcode 
                                    (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                              /*
                              if (node->hasAttributes ())
                                {
                                  property_node_map = node->getAttributes ();
                                  attr_length = property_node_map->getLength ();

                                  if (attr_length == 1)
                                    {
                                      property_length = domain_bridge.resource[resource_length].property.length ();
                                      domain_bridge.resource[resource_length].property.length (property_length + 1);
                                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                                             domain_bridge.resource[resource_length].property[property_length]);
                                    }
                                  else if (attr_length > 1)
                                    {
                                      for (int j = 0; j < attr_length; j++)
                                        {
                                          DOMNode* attribute_node = property_node_map->item (j);
                                          strattrnodename = attribute_node->getNodeName ();
                                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                                            {
                                              property_length = domain_bridge.resource[resource_length].property.length ();
                                              domain_bridge.resource[resource_length].property.length (property_length + 1);
                                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                     domain_bridge.resource[resource_length].property[property_length]);
                                              id_map_.bind (aceattrnodevalue, property_length);
                                            }
                                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                                            {
                                              property_length = domain_bridge.resource[resource_length].property.length ();
                                              domain_bridge.resource[resource_length].property.length (property_length + 1);
                                              xml_url = aceattrnodevalue.c_str ();
                                              result = aceattrnodevalue.c_str ();
                                              doc_path = XMLString::transcode ( doc->getDocumentURI ());
                                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                                              final_url = XMLString::transcode (result.getURLText ());

                                              if (xml_url.isRelative ())
                                                {
                                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                                           domain_bridge.resource[resource_length].property[property_length]);
                                                }
                                              else
                                                {
                                                  url_string = aceattrnodevalue.c_str ();
                                                  DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                                           domain_bridge.resource[resource_length].property[property_length]);
                                                }
                                            }
                                        }
                                    }
                                }
                              else
                                {
                                }
                                */
                            }
                          else
                            {
                              node = iter->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename (attribute_node->getNodeName ());
                          ACE_TString aceattrnodevalue = XMLString::transcode 
                            (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              XMLURL xml_url = aceattrnodevalue.c_str ();
                              XMLURL result = aceattrnodevalue.c_str ();
                              ACE_TString doc_path = XMLString::transcode 
                                (doc->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode 
                                 (doc_path.c_str ()));
                              ACE_TString final_url = XMLString::transcode 
                                 (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = 
                                    this->create_document 
                                      (final_url.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_bridge.resource[resource_length]);
                                }
                              else
                                {
                                  std::string url_string = 
                                    aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = 
                                    this->create_document 
                                       (url_string.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_bridge.resource[resource_length]);
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      domain_bridge.resource[resource_length].
                                        name = XMLString::transcode 
                                          (text->getNodeValue ());
                                      id_map_.bind 
                                        (aceattrnodevalue, resource_length);
                                    }
                                  else if (node_name == XStr 
                                    (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      CORBA::ULong resource_type_length = 
                                        domain_bridge.resource[resource_length].
                                          resourceType.length ();
                                      domain_bridge.resource[resource_length].
                                        resourceType.length 
                                           (resource_type_length + 1);
                                      domain_bridge.resource[resource_length].
                                         resourceType[resource_type_length] = 
                                           XMLString::transcode 
                                             (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr 
                                     (ACE_TEXT ("property")))
                                    {
                                      /*
                                      if (node->hasAttributes ())
                                        {
                                          property_node_map = node->getAttributes ();
                                          attr_length = property_node_map->getLength ();

                                          if (attr_length == 1)
                                            {
                                              property_length = domain_bridge.resource[resource_length].property.length ();
                                              domain_bridge.resource[resource_length].property.length (property_length + 1);
                                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                     domain_bridge.resource[resource_length].property[property_length]);
                                            }
                                          else if (attr_length > 1)
                                            {
                                              for (int j = 0; j < attr_length; j++)
                                                {
                                                  DOMNode* attribute_node = property_node_map->item (j);
                                                  strattrnodename = attribute_node->getNodeName ();
                                                  aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                                                  if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                                                    {
                                                      property_length = domain_bridge.resource[resource_length].property.length ();
                                                      domain_bridge.resource[resource_length].property.length (property_length + 1);
                                                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                             domain_bridge.resource[resource_length].property[property_length]);
                                                      id_map_.bind (aceattrnodevalue, property_length);
                                                    }
                                                  else if (strattrnodename == XStr (ACE_TEXT ("href")))
                                                    {
                                                      property_length = domain_bridge.resource[resource_length].property.length ();
                                                      domain_bridge.resource[resource_length].property.length (property_length + 1);
                                                      xml_url = aceattrnodevalue.c_str ();
                                                      result = aceattrnodevalue.c_str ();
                                                      doc_path = XMLString::transcode ( doc->getDocumentURI ());
                                                      result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                                                      final_url = XMLString::transcode (result.getURLText ());

                                                      if (xml_url.isRelative ())
                                                        {
                                                          DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_bridge.resource[resource_length].property[property_length]);
                                                        }
                                                      else
                                                        {
                                                          url_string = aceattrnodevalue.c_str ();
                                                          DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_bridge.resource[resource_length].property[property_length]);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                      else
                                        {
                                        }
                                        */
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              CORBA::ULong connect_ref_length = 
                 domain_bridge.connectRef.length ();
              domain_bridge.connectRef.length (connect_ref_length + 1);

              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                        (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_bridge.connectRef[connect_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_interconnect (
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
                  int length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      for (node = iter->nextNode();
                           node != 0;
                           node = iter->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              domain_ic.resource[resource_length].name = 
                                XMLString::transcode 
                                  (text->getNodeValue ());
                            }
                          else if (node_name == XStr 
                             (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast 
                                (DOMText*, node);
                              CORBA::ULong resource_type_length = 
                                domain_ic.resource[resource_length].
                                  resourceType.length ();
                              domain_ic.resource[resource_length].
                                resourceType.length 
                                  (resource_type_length + 1);
                              domain_ic.resource[resource_length].
                                resourceType[resource_type_length] = 
                                  XMLString::transcode 
                                    (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                              /*
                              if (node->hasAttributes ())
                                {
                                  property_node_map = node->getAttributes ();
                                  attr_length = property_node_map->getLength ();

                                  if (attr_length == 1)
                                    {
                                      property_length = domain_ic.resource[resource_length].property.length ();
                                      domain_ic.resource[resource_length].property.length (property_length + 1);
                                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                                             domain_ic.resource[resource_length].property[property_length]);
                                    }
                                  else if (attr_length > 1)
                                    {
                                      for (int j = 0; j < attr_length; j++)
                                        {
                                          DOMNode* attribute_node = property_node_map->item (j);
                                          strattrnodename = attribute_node->getNodeName ();
                                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                                            {
                                              property_length = domain_ic.resource[resource_length].property.length ();
                                              domain_ic.resource[resource_length].property.length (property_length + 1);
                                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                     domain_ic.resource[resource_length].property[property_length]);
                                              id_map_.bind (aceattrnodevalue, property_length);
                                            }
                                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                                            {
                                              property_length = domain_ic.resource[resource_length].property.length ();
                                              domain_ic.resource[resource_length].property.length (property_length + 1);
                                              xml_url = aceattrnodevalue.c_str ();
                                              result = aceattrnodevalue.c_str ();
                                              doc_path = XMLString::transcode ( doc->getDocumentURI ());
                                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                                              final_url = XMLString::transcode (result.getURLText ());

                                              if (xml_url.isRelative ())
                                                {
                                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                                           domain_ic.resource[resource_length].property[property_length]);
                                                }
                                              else
                                                {
                                                  url_string = aceattrnodevalue.c_str ();
                                                  DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                                           domain_ic.resource[resource_length].property[property_length]);
                                                }
                                            }
                                        }
                                    }
                                }
                              else
                                {
                                }
                                */
                            }
                          else
                            {
                              node = iter->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (int j = 0; j < length; j++)
                        {
                          DOMNode* attribute_node = named_node_map->item (j);
                          XStr strattrnodename (attribute_node->getNodeName ());
                          ACE_TString aceattrnodevalue = XMLString::transcode 
                             (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              ACE_TString doc_path;
                              ACE_TString final_url;
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              XMLURL result (aceattrnodevalue.c_str ());
                              doc_path = XMLString::transcode ( doc->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                              final_url = XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {

                                  DOMDocument* href_doc = 
                                    this->create_document 
                                       (final_url.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_ic.resource[resource_length]);
                                }
                              else
                                {
                                  std::string url_string 
                                    (aceattrnodevalue.c_str ());
                                  DOMDocument* href_doc = 
                                    this->create_document 
                                       (url_string.c_str ());
                                  this->parse_resource_href_doc 
                                     (href_doc,
                                      DOMNodeFilter::SHOW_ELEMENT |
                                      DOMNodeFilter::SHOW_TEXT,
                                      domain_ic.resource[resource_length]);
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  XStr node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      domain_ic.resource[resource_length].
                                         name = XMLString::transcode 
                                            (text->getNodeValue ());
                                      id_map_.bind 
                                        (aceattrnodevalue, resource_length);
                                    }
                                  else if (node_name == XStr 
                                     (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast 
                                        (DOMText*, node);
                                      CORBA::ULong resource_type_length = 
                                         domain_ic.resource[resource_length].
                                            resourceType.length ();
                                      domain_ic.resource[resource_length].
                                         resourceType.length 
                                            (resource_type_length + 1);
                                      domain_ic.resource[resource_length].
                                         resourceType[resource_type_length] = 
                                            XMLString::transcode 
                                               (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr 
                                      (ACE_TEXT ("property")))
                                    {
                                      /*
                                      if (node->hasAttributes ())
                                        {
                                          property_node_map = node->getAttributes ();
                                          attr_length = property_node_map->getLength ();

                                          if (attr_length == 1)
                                            {
                                              property_length = domain_ic.resource[resource_length].property.length ();
                                              domain_ic.resource[resource_length].property.length (property_length + 1);
                                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                     domain_ic.resource[resource_length].property[property_length]);
                                            }
                                          else if (attr_length > 1)
                                            {
                                              for (j = 0; j < attr_length; j++)
                                                {
                                                  DOMNode* attribute_node = property_node_map->item (j);
                                                  strattrnodename = attribute_node->getNodeName ();
                                                  aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                                                  if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                                                    {
                                                      property_length = domain_ic.resource[resource_length].property.length ();
                                                      domain_ic.resource[resource_length].property.length (property_length + 1);
                                                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                                                             domain_ic.resource[resource_length].property[property_length]);
                                                      id_map_.bind (aceattrnodevalue, property_length);
                                                    }
                                                  else if (strattrnodename == XStr (ACE_TEXT ("href")))
                                                    {
                                                      property_length = domain_ic.resource[resource_length].property.length ();
                                                      domain_ic.resource[resource_length].property.length (property_length + 1);
                                                      xml_url = aceattrnodevalue.c_str ();
                                                      result = aceattrnodevalue.c_str ();
                                                      doc_path = XMLString::transcode ( doc->getDocumentURI ());
                                                      result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                                                      final_url = XMLString::transcode (result.getURLText ());

                                                      if (xml_url.isRelative ())
                                                        {
                                                          DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_ic.resource[resource_length].property[property_length]);
                                                        }
                                                      else
                                                        {
                                                          url_string = aceattrnodevalue.c_str ();
                                                          DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                                          this->parse_satisfier_property_href_doc (href_doc, 
                                                                                                   DOMNodeFilter::SHOW_ELEMENT |
                                                                                                   DOMNodeFilter::SHOW_TEXT,
                                                                                                   domain_ic.resource[resource_length].property[property_length]);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                      else
                                        {
                                        }
                                        */
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connect")))
            {
              CORBA::ULong connect_ref_length = 
                  (domain_ic.connectRef.length ());
              domain_ic.connectRef.length (connect_ref_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                          (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectRef[connect_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length = 
                 (domain_ic.connectionRef.length ());
              domain_ic.connectionRef.length (connection_ref_length + 1);
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                         (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectionRef[connection_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_sr 
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
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  for (int j = 0; j < length; j++)
                    {
                      DOMNode* attribute_node = named_node_map->item (j);
                      XStr strattrnodename (attribute_node->getNodeName ());
                      ACE_TString aceattrnodevalue = XMLString::transcode 
                         (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_sr.nodeRef[node_ref_length] = 0;
                          this->index_ = this->index_ + 1;
                          idref_map_.bind (this->index_, aceattrnodevalue);
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              /*
              if (node->hasAttributes ())
                {
                  property_node_map = node->getAttributes ();
                  attr_length = property_node_map->getLength ();
                  if (attr_length == 1)
                    {
                      property_length = domain_sr.property.length ();
                      domain_sr.property.length (property_length + 1);
                      SP_Handler::process_SatisfierProperty (this->iter_,
                                                             domain_sr.property[property_length]);
                    }
                  else if (attr_length > 1)
                    {
                      for (int j = 0; j < attr_length; j++)
                        {
                          DOMNode* attribute_node = property_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              property_length = domain_sr.property.length ();
                              domain_sr.property.length (property_length + 1);
                              SP_Handler::process_SatisfierProperty (this->iter_,
                                                                     domain_sr.property[property_length]);
                              id_map_.bind (aceattrnodevalue, property_length);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              property_length = domain_sr.property.length ();
                              domain_sr.property.length (property_length + 1);
                              xml_url = aceattrnodevalue.c_str ();
                              result = aceattrnodevalue.c_str ();
                              doc_path = XMLString::transcode ( doc->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                              final_url = XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                           domain_sr.property[property_length]);
                                }
                              else
                                {
                                  url_string = aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                           domain_sr.property[property_length]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
                */
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

    void Domain_Handler::process_bridge_label (const XMLCh* label,
                                               Deployment::Bridge& domain_bridge)
    {
      if (label)
        {
          domain_bridge.label = XMLString::transcode (label);
        }
    }

    void Domain_Handler::process_ic_label (const XMLCh* label,
                                           Deployment::Interconnect& domain_ic)
    {
      if (label)
        {
          domain_ic.label = XMLString::transcode (label);
        }
    }

    void Domain_Handler::process_sr_resource_type (const XMLCh* resource_type,
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
        ((DOMImplementationLS*)impl)->createDOMBuilder(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

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
      root_node_name = XMLString::transcode (doc->getDocumentElement ()->getNodeName ());

      return doc;
    }

    void Domain_Handler::parse_resource_href_doc (DOMDocument* href_doc,
                                                  unsigned long filter,
                                                  Deployment::Resource& domain_resource)
    {
      DOMDocumentTraversal* traverse (href_doc);
      DOMNode* root (href_doc->getDocumentElement ());

      DOMNodeIterator* iter (traverse->createNodeIterator (root,
                                                           filter,
                                                           0,
                                                           true));
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode())
        {
          XStr node_name = node->getNodeName ();
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
              domain_resource.resourceType.length 
                 (resource_type_length + 1);
              domain_resource.resourceType[resource_type_length] = 
                XMLString::transcode (text->getNodeValue ());
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              /*
              if (node->hasAttributes ())
                {
                  property_node_map = node->getAttributes ();
                  attr_length = property_node_map->getLength ();

                  if (attr_length == 1)
                    {
                      property_length = domain_resource.property.length ();
                      domain_resource.property.length (property_length + 1);
                      SP_Handler::process_SatisfierProperty (iter,
                                                             domain_resource.property[property_length]);
                    }
                  else if (attr_length > 1)
                    {
                      for (int j = 0; j < attr_length; j++)
                        {
                          DOMNode* attribute_node = property_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              property_length = domain_resource.property.length ();
                              domain_resource.property.length (property_length + 1);
                              SP_Handler::process_SatisfierProperty (iter,
                                                                     domain_resource.property[property_length]);
                              id_map_.bind (aceattrnodevalue, property_length);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              property_length = domain_resource.property.length ();
                              domain_resource.property.length (property_length + 1);
                              xml_url = aceattrnodevalue.c_str ();
                              result = aceattrnodevalue.c_str ();
                              doc_path = XMLString::transcode ( doc->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                              final_url = XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {
                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());
                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                           domain_resource.property[property_length]);
                                }
                              else
                                {
                                  url_string = aceattrnodevalue.c_str ();
                                  DOMDocument* href_doc = this->create_document (url_string.c_str ());
                                  this->parse_satisfier_property_href_doc (href_doc, 
                                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                                           DOMNodeFilter::SHOW_TEXT,
                                                                           domain_resource.property[property_length]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
                */
            }
          else
            {
              iter->previousNode ();
              break;
            }
        }
      return;
    }

    void Domain_Handler::parse_property_href_doc (DOMDocument* href_doc,
                                                  unsigned long filter,
                                                  Deployment::Property& property)
    {
      DOMDocumentTraversal* traverse (href_doc);
      DOMNode* root (href_doc->getDocumentElement ());

      DOMNodeIterator* iter (traverse->createNodeIterator (root,
                                                           filter,
                                                           0,
                                                           true));
      /*
      Property_Handler::process_Property (iter,
                                          property);
      */

      return;
    }

    void Domain_Handler::parse_satisfier_property_href_doc (DOMDocument* href_doc,
                                                            unsigned long filter,
                                                            Deployment::SatisfierProperty& satisfier_property)
    {
      DOMDocumentTraversal* traverse (href_doc);
      DOMNode* root (href_doc->getDocumentElement ());

      DOMNodeIterator* iter (traverse->createNodeIterator (root,
                                                           filter,
                                                           0,
                                                           true));
      /*
      SP_Handler::process_SatisfierProperty (iter,
                                             satisfier_property);
      */

      return;
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
              this->process_domain_sr (doc,
                                       iter,
                                       domain_sr);
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
              this->process_domain_sr (href_doc,
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
              this->process_domain_node (doc,
                                         iter,
                                         domain_node);
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
              this->process_domain_node (href_doc,
                                         href_iter,
                                         domain_node);
            }
        }

      return;
    }

  }
}

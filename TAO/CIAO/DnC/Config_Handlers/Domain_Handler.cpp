// $Id$

#ifndef DOMAIN_HANDLER_C
#define DOMAIN_HANDLER_C

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "Domain_Handler.h"

#include <iostream>
#include "string.h"

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    typedef ACE_Hash_Map_Iterator<ACE_TString, ACE_TString, ACE_SYNCH_RW_MUTEX> REF_ITER;
    typedef ACE_Hash_Map_Entry<ACE_TString, ACE_TString> REF_ENTRY;
    Domain_Handler::Domain_Handler (DOMDocument* doc, unsigned long filter)
      : traverse_ (doc),
        doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (traverse_->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    Domain_Handler::Domain_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}

    void Domain_Handler::dump (Deployment::Domain& domain)
    {
      ACE_DEBUG ((LM_DEBUG, "UUID: %s \n", domain.UUID.in ()));
      ACE_DEBUG ((LM_DEBUG, "label: %s \n", domain.label.in ()));
      for (CORBA::ULong i = 0; i < domain.node.length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "\nNode %d: \n", i + 1));
          ACE_DEBUG ((LM_DEBUG, "     Name: %s \n", domain.node[i].name.in ()));
          for (CORBA::ULong j = 0; j < domain.node[i].resource.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Resource %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n", domain.node[i].resource[j].name.in ()));
              for (CORBA::ULong k = 0; k < domain.node[i].resource[j].resourceType.length (); ++k)
                {
                  ACE_DEBUG ((LM_DEBUG, "         ResourceType: %s \n", domain.node[i].resource[j].resourceType[k].in ()));
                }
            }
          for (CORBA::ULong j = 0; j < domain.node[i].connectionRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     Connection %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n", domain.node[i].connectionRef[j].in ()));
            }
          for (CORBA::ULong j = 0; j < domain.node[i].sharedResourceRef.length (); ++j)
            {
              ACE_DEBUG ((LM_DEBUG, "     SharedResource %d: \n", j + 1));
              ACE_DEBUG ((LM_DEBUG, "         Name: %s \n", domain.node[i].sharedResourceRef[j].in ()));
            }
        }

      REF_ITER end = id_map_.end ();

      for (REF_ITER iter = id_map_.begin ();
           iter != end;
           ++iter)
        {
          REF_ENTRY &entry = *iter;
        }
    }

    Domain_Handler::~Domain_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the Domain type and populate the IDL structure
    void Domain_Handler::process_domain (Deployment::Domain& domain)
    {
      // This is bogus and should be replaced later.
      ACE_DECLARE_NEW_CORBA_ENV;

      CORBA::ULong x;
      CORBA::ULong y;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          int length;
          int j;
          XStr node_name (node->getNodeName ());
          DOMNode* attribute_node;
          CORBA::String_var attrnodename;
          ACE_TString aceattrnodename;
          ACE_TString aceattrnodevalue;
          ACE_TString acemapname;
          XStr strattrnodename;
          CORBA::String_var attrnodevalue;
          DOMNamedNodeMap* named_node_map;

          if (node_name == XStr (ACE_TEXT ("Deployment:Domain")))
            {
              ACE_DEBUG ((LM_DEBUG, "=========================================================================== \n\n"));
              ACE_DEBUG ((LM_DEBUG, "                            DOMAIN DESCRIPTOR \n"));
              ACE_DEBUG ((LM_DEBUG, "=========================================================================== \n\n"));
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_uuid (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), domain);
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      // increase the length of the sequence
                      CORBA::ULong i (domain.sharedResource.length ());
                      domain.sharedResource.length (i + 1);
                      this->process_domain_sr (domain.sharedResource[i]);
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          aceattrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.sharedResource.length ());
                              domain.sharedResource.length (i + 1);

                              this->process_domain_sr (domain.sharedResource[i]);
                              acemapname = domain.sharedResource[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.sharedResource.length ());
                              domain.sharedResource.length (i + 1);

                              this->process_domain_sr (domain.sharedResource[i]);
                              acemapname = domain.sharedResource[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.sharedResource.length ());
                              domain.sharedResource.length (i + 1);

                              ACE_TString doc_path;
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              doc_path = XMLString::transcode ( doc_->getDocumentURI ());

                              if (xml_url.isRelative ())
                                {

                                  std::string s1 (doc_path.c_str ());
                                  std::string s2 ("/../");
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  std::string s4;
                                  s4 = s1 + s2 + s3;


                                  DOMDocument* href_doc = this->create_document (s4.c_str ());

                                  this->parse_sr_href_doc (href_doc, 
                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                           DOMNodeFilter::SHOW_TEXT,
                                                           domain.sharedResource[i]);

                                }
                              else
                                {
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  DOMDocument* href_doc = this->create_document (s3.c_str ());

                                  this->parse_sr_href_doc (href_doc, 
                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                           DOMNodeFilter::SHOW_TEXT,
                                                           domain.sharedResource[i]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      CORBA::ULong i (domain.node.length ());
                      domain.node.length (i + 1);
                      this->process_domain_node (domain.node[i]);
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          aceattrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              CORBA::ULong i (domain.node.length ());
                              domain.node.length (i + 1);

                              this->process_domain_node (domain.node[i]);
                              acemapname = domain.node[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              CORBA::ULong i (domain.node.length ());
                              domain.node.length (i + 1);

                              this->process_domain_node (domain.node[i]);
                              acemapname = domain.node[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.node.length ());
                              domain.node.length (i + 1);

                              ACE_TString doc_path;
                              ACE_TString final_url;
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              XMLURL result (aceattrnodevalue.c_str ());
                              doc_path = XMLString::transcode ( doc_->getDocumentURI ());
                              result.makeRelativeTo (XMLString::transcode (doc_path.c_str ()));
                              final_url = XMLString::transcode (result.getURLText ());

                              if (xml_url.isRelative ())
                                {

                                  DOMDocument* href_doc = this->create_document (final_url.c_str ());

                                  this->parse_node_href_doc (href_doc, 
                                                             DOMNodeFilter::SHOW_ELEMENT |
                                                             DOMNodeFilter::SHOW_TEXT,
                                                             domain.node[i]);
                                  

                                }
                              else
                                {
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  DOMDocument* href_doc = this->create_document (s3.c_str ());

                                  this->parse_node_href_doc (href_doc, 
                                                             DOMNodeFilter::SHOW_ELEMENT |
                                                             DOMNodeFilter::SHOW_TEXT,
                                                             domain.node[i]);
                                }
                            }
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("interconnect")))
            {
              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      // increase the length of the sequence
                      CORBA::ULong i (domain.interconnect.length ());
                      domain.interconnect.length (i + 1);
                      this->process_domain_interconnect (domain.interconnect[i]);
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.interconnect.length ());
                              domain.interconnect.length (i + 1);

                              this->process_domain_interconnect (domain.interconnect[i]);
                              acemapname = domain.interconnect[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.interconnect.length ());
                              domain.interconnect.length (i + 1);

                              this->process_domain_interconnect (domain.interconnect[i]);
                              acemapname = domain.interconnect[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.interconnect.length ());
                              domain.interconnect.length (i + 1);

                              ACE_TString doc_path;
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              doc_path = XMLString::transcode ( doc_->getDocumentURI ());

                              if (xml_url.isRelative ())
                                {

                                  std::string s1 (doc_path.c_str ());
                                  std::string s2 ("/../");
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  std::string s4;
                                  s4 = s1 + s2 + s3;

                                  DOMDocument* href_doc = this->create_document (s4.c_str ());

                                  this->parse_ic_href_doc (href_doc, 
                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                           DOMNodeFilter::SHOW_TEXT,
                                                           domain.interconnect[i]);

                                }
                              else
                                {
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  DOMDocument* href_doc = this->create_document (s3.c_str ());

                                  this->parse_ic_href_doc (href_doc, 
                                                           DOMNodeFilter::SHOW_ELEMENT |
                                                           DOMNodeFilter::SHOW_TEXT,
                                                           domain.interconnect[i]);
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
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();
                  if (length == 1)
                    {
                      // increase the length of the sequence
                      CORBA::ULong i (domain.bridge.length ());
                      domain.bridge.length (i + 1);
                      this->process_domain_bridge (domain.bridge[i]);
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.bridge.length ());
                              domain.bridge.length (i + 1);

                              this->process_domain_bridge (domain.bridge[i]);
                              acemapname = domain.bridge[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              // increase the length of the sequence
                              CORBA::ULong i (domain.bridge.length ());
                              domain.bridge.length (i + 1);

                              this->process_domain_bridge (domain.bridge[i]);
                              acemapname = domain.bridge[i].name.in ();
                              id_map_.bind (aceattrnodevalue, acemapname);
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                              CORBA::ULong i (domain.bridge.length ());
                              domain.bridge.length (i + 1);

                              ACE_TString doc_path;
                              XMLURL xml_url (aceattrnodevalue.c_str ());
                              doc_path = XMLString::transcode ( doc_->getDocumentURI ());

                              if (xml_url.isRelative ())
                                {

                                  std::string s1 (doc_path.c_str ());
                                  std::string s2 ("/../");
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  std::string s4;
                                  s4 = s1 + s2 + s3;


                                  DOMDocument* href_doc = this->create_document (s4.c_str ());

                                  this->parse_bridge_href_doc (href_doc, 
                                                               DOMNodeFilter::SHOW_ELEMENT |
                                                               DOMNodeFilter::SHOW_TEXT,
                                                               domain.bridge[i]);

                                }
                              else
                                {
                                  std::string s3 (aceattrnodevalue.c_str ());
                                  DOMDocument* href_doc = this->create_document (s3.c_str ());

                                  this->parse_bridge_href_doc (href_doc, 
                                                               DOMNodeFilter::SHOW_ELEMENT |
                                                               DOMNodeFilter::SHOW_TEXT,
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
              // Need to Handle this.
              //
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }

      ACE_TString name;
      ACE_TString value;

      for (x = 0; x < domain.node.length (); ++x)
        {
          for (y = 0; y < domain.node[x].connectionRef.length (); ++y)
            {
              name =  domain.node[x].connectionRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.node[x].connectionRef[y] = value.c_str ();
               }
            }

          for (y = 0; y < domain.node[x].sharedResourceRef.length (); ++y)
            {
              name = domain.node[x].sharedResourceRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.node[x].sharedResourceRef[y] = value.c_str ();
               }
            }
        }

      for (x = 0; x < domain.interconnect.length (); ++x)
        {
          for (y = 0; y < domain.interconnect[x].connectionRef.length (); ++y)
            {
              name = domain.interconnect[x].connectionRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.interconnect[x].connectionRef[y] = value.c_str ();
               }
            }

          for (y = 0; y < domain.interconnect[x].connectRef.length (); ++y)
            {
              name = domain.interconnect[x].connectRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.interconnect[x].connectRef[y] = value.c_str ();
               }
            }
        }

      for (x = 0; x < domain.bridge.length (); ++x)
        {
          for (y = 0; y < domain.bridge[x].connectRef.length (); ++y)
            {
              name = domain.bridge[x].connectRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.bridge[x].connectRef[y] = value.c_str ();
               }
            }
        }

      for (x = 0; x < domain.sharedResource.length (); ++x)
        {
          for (y = 0; y < domain.sharedResource[x].nodeRef.length (); ++y)
            {
              name = domain.sharedResource[x].nodeRef[y].in ();
              if (id_map_.find (name, value) == 0)
               {
                 domain.sharedResource[x].nodeRef[y] = value.c_str ();
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

    void Domain_Handler::process_domain_node (Deployment::Node& domain_node)
    {
      int j;
      int length;
      DOMNode* attribute_node;
      CORBA::String_var attrnodename;
      ACE_TString aceattrnodename;
      ACE_TString aceattrnodevalue;
      ACE_TString acemapname;
      XStr strattrnodename;
      CORBA::String_var attrnodevalue;
      DOMNamedNodeMap* named_node_map;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node_name (text->getNodeValue (), domain_node);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_node_label (text->getNodeValue (), domain_node);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length (domain_node.resource.length ());
              domain_node.resource.length (resource_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      for (node = this->iter_->nextNode();
                           node != 0;
                           node = this->iter_->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                              domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                            }
                          else
                            {
                              node = this->iter_->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());

                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_node.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                                      domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_node.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                                      domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
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
              CORBA::ULong connection_ref_length (domain_node.connectionRef.length ());
              domain_node.connectionRef.length (connection_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_node.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              CORBA::ULong shared_resource_ref_length (domain_node.sharedResourceRef.length ());
              domain_node.sharedResourceRef.length (shared_resource_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();
                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_node.sharedResourceRef[shared_resource_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.sharedResourceRef[shared_resource_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              this->iter_->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_bridge (Deployment::Bridge& domain_bridge)
    {
      int j;
      int length;
      DOMNode* attribute_node;
      CORBA::String_var attrnodename;
      ACE_TString aceattrnodename;
      ACE_TString aceattrnodevalue;
      ACE_TString acemapname;
      XStr strattrnodename;
      CORBA::String_var attrnodevalue;
      DOMNamedNodeMap* named_node_map;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_bridge_name (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_bridge_label (text->getNodeValue (), domain_bridge);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length (domain_bridge.resource.length ());
              domain_bridge.resource.length (resource_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();
                  
                  if (length == 1)
                    {
                      for (node = this->iter_->nextNode();
                           node != 0;
                           node = this->iter_->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                              domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                            }
                          else
                            {
                              node = this->iter_->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_bridge.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                                      domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_bridge.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                                      domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
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
              CORBA::ULong connect_ref_length (domain_bridge.connectRef.length ());
              domain_bridge.connectRef.length (connect_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_bridge.connectRef[connect_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_bridge.connectRef[connect_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              this->iter_->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_interconnect (Deployment::Interconnect& domain_ic)
    {
      int j;
      int length;
      DOMNode* attribute_node;
      CORBA::String_var attrnodename;
      ACE_TString aceattrnodename;
      ACE_TString aceattrnodevalue;
      ACE_TString acemapname;
      XStr strattrnodename;
      CORBA::String_var attrnodevalue;
      DOMNamedNodeMap* named_node_map;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ic_name (text->getNodeValue (), domain_ic);
            }
          else if (node_name == XStr (ACE_TEXT ("label")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ic_label (text->getNodeValue (), domain_ic);
            }
          else if (node_name == XStr (ACE_TEXT ("resource")))
            {
              CORBA::ULong resource_length (domain_ic.resource.length ());
              domain_ic.resource.length (resource_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  if (length == 1)
                    {
                      for (node = this->iter_->nextNode();
                           node != 0;
                           node = this->iter_->nextNode())
                        {
                          node_name = node->getNodeName ();
                          if (node_name == XStr (ACE_TEXT ("name")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = this->iter_->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                              domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
                            }
                          else
                            {
                              node = this->iter_->previousNode ();
                              break;
                            }
                        }
                    }
                  else if (length > 1)
                    {
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_ic.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                                      domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = this->iter_->nextNode();
                                   node != 0;
                                   node = this->iter_->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_ic.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = this->iter_->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                                      domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = this->iter_->previousNode ();
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
              CORBA::ULong connect_ref_length (domain_ic.connectRef.length ());
              domain_ic.connectRef.length (connect_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_ic.connectRef[connect_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectRef[connect_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length (domain_ic.connectionRef.length ());
              domain_ic.connectionRef.length (connection_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_ic.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
              this->iter_->previousNode();
              return;
            }
        }
    }


    void Domain_Handler::process_domain_sr (Deployment::SharedResource& domain_sr)
    {
      int j;
      int length;
      DOMNode* attribute_node;
      CORBA::String_var attrnodename;
      ACE_TString aceattrnodename;
      ACE_TString aceattrnodevalue;
      XStr strattrnodename;
      CORBA::String_var attrnodevalue;
      DOMNamedNodeMap* named_node_map;

      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_sr_name (text->getNodeValue (), domain_sr);
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              node = this->iter_->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_sr_resource_type (text->getNodeValue (), domain_sr);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              CORBA::ULong node_ref_length (domain_sr.nodeRef.length ());
              domain_sr.nodeRef.length (node_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_sr.nodeRef[node_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_sr.nodeRef[node_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              node = this->iter_->nextNode ();
            }
          else
            {
              this->iter_->previousNode();
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


    void Domain_Handler::parse_node_href_doc (DOMDocument* href_doc,
                                              unsigned long filter,
                                              Deployment::Node& domain_node)
    {
      DOMDocumentTraversal* traverse (href_doc);
      DOMNode* root = (href_doc->getDocumentElement ());
      ACE_TString doc_path;
      doc_path = XMLString::transcode ( href_doc->getDocumentURI ());
      ACE_TString root_node_name;
      root_node_name = XMLString::transcode (href_doc->getDocumentElement ()->getNodeName ());

      DOMNodeIterator* iter = traverse->createNodeIterator (root,
                                                           filter,
                                                           0,
                                                           true);

      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode())
        {
          XStr node_name (node->getNodeName ());
          DOMNode* attribute_node;
          CORBA::String_var attrnodename;
          ACE_TString aceattrnodename;
          ACE_TString aceattrnodevalue;
          ACE_TString acemapname;
          XStr strattrnodename;
          CORBA::String_var attrnodevalue;
          DOMNamedNodeMap* named_node_map;

          int length;
          int j;
        
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
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

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
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                              domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
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
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());

                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
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
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_node.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                                      domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_node.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_node.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_node.resource[resource_length].resourceType.length ());
                                      domain_node.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_node.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
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
              CORBA::ULong connection_ref_length (domain_node.connectionRef.length ());
              domain_node.connectionRef.length (connection_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_node.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("sharedResource")))
            {
              CORBA::ULong shared_resource_ref_length (domain_node.sharedResourceRef.length ());
              domain_node.sharedResourceRef.length (shared_resource_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();
                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_node.sharedResourceRef[shared_resource_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_node.sharedResourceRef[shared_resource_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else
            {
            }
        }

      return;
    }

    void Domain_Handler::parse_sr_href_doc (DOMDocument* href_doc,
                                            unsigned long filter,
                                            Deployment::SharedResource& domain_sr)
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
          DOMNode* attribute_node;
          CORBA::String_var attrnodename;
          ACE_TString aceattrnodename;
          ACE_TString aceattrnodevalue;
          ACE_TString acemapname;
          XStr strattrnodename;
          CORBA::String_var attrnodevalue;
          DOMNamedNodeMap* named_node_map;
          int length;
          int j;

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
              this->process_sr_resource_type (text->getNodeValue (), domain_sr);
            }
          else if (node_name == XStr (ACE_TEXT ("node")))
            {
              CORBA::ULong node_ref_length (domain_sr.nodeRef.length ());
              domain_sr.nodeRef.length (node_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_sr.nodeRef[node_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_sr.nodeRef[node_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              node = iter->nextNode ();
            }
          else
            {
              iter->previousNode();
              break;
            }
        }

        return;

    }

    void Domain_Handler::parse_ic_href_doc (DOMDocument* href_doc,
                                            unsigned long filter,
                                            Deployment::Interconnect& domain_ic)
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
          DOMNode* attribute_node;
          CORBA::String_var attrnodename;
          ACE_TString aceattrnodename;
          ACE_TString aceattrnodevalue;
          ACE_TString acemapname;
          XStr strattrnodename;
          CORBA::String_var attrnodevalue;
          DOMNamedNodeMap* named_node_map;
          int length;
          int j;

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
              CORBA::ULong resource_length (domain_ic.resource.length ());
              domain_ic.resource.length (resource_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();

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
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                              domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
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
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
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
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_ic.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                                      domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_ic.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_ic.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_ic.resource[resource_length].resourceType.length ());
                                      domain_ic.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_ic.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
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
              CORBA::ULong connect_ref_length (domain_ic.connectRef.length ());
              domain_ic.connectRef.length (connect_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_ic.connectRef[connect_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectRef[connect_ref_length] = attrnodevalue;
                        }
                    }
                }
              else
                {
                }
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              CORBA::ULong connection_ref_length (domain_ic.connectionRef.length ());
              domain_ic.connectionRef.length (connection_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_ic.connectionRef[connection_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_ic.connectionRef[connection_ref_length] = attrnodevalue;
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
              break;
            }

        }

      return;

    }

    void Domain_Handler::parse_bridge_href_doc (DOMDocument* href_doc,
                                                unsigned long filter,
                                                Deployment::Bridge& domain_bridge)
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
          DOMNode* attribute_node;
          CORBA::String_var attrnodename;
          ACE_TString aceattrnodename;
          ACE_TString aceattrnodevalue;
          ACE_TString acemapname;
          XStr strattrnodename;
          CORBA::String_var attrnodevalue;
          DOMNamedNodeMap* named_node_map;
          int length;
          int j;

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
              CORBA::ULong resource_length (domain_bridge.resource.length ());
              domain_bridge.resource.length (resource_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();
                  
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
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                              domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("resourceType")))
                            {
                              node = iter->nextNode();
                              DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                              CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                              domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                              domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                            }
                          else if (node_name == XStr (ACE_TEXT ("property")))
                            {
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
                      for (j = 0; j < length; j++)
                        {
                          attribute_node = named_node_map->item (j);
                          strattrnodename = attribute_node->getNodeName ();
                          attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                          attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                          if (strattrnodename == XStr (ACE_TEXT ("href")))
                            {
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
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_bridge.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                                      domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
                                    }
                                  else
                                    {
                                      node = iter->previousNode ();
                                      break;
                                    }
                                }
                            }
                          else if (strattrnodename == XStr (ACE_TEXT ("XMI:ID")))
                            {
                              for (node = iter->nextNode();
                                   node != 0;
                                   node = iter->nextNode())
                                {
                                  node_name = node->getNodeName ();
                                  if (node_name == XStr (ACE_TEXT ("name")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);
                                      domain_bridge.resource[resource_length].name = XMLString::transcode (text->getNodeValue ());
                                      acemapname = domain_bridge.resource[resource_length].name.in ();
                                      id_map_.bind (aceattrnodevalue, acemapname);
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("resourceType")))
                                    {
                                      node = iter->nextNode();
                                      DOMText* text = ACE_reinterpret_cast (DOMText*, node);

                                      CORBA::ULong resource_type_length (domain_bridge.resource[resource_length].resourceType.length ());
                                      domain_bridge.resource[resource_length].resourceType.length (resource_type_length + 1);
                                      domain_bridge.resource[resource_length].resourceType[resource_type_length] = XMLString::transcode (text->getNodeValue ());
                                    }
                                  else if (node_name == XStr (ACE_TEXT ("property")))
                                    {
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
              CORBA::ULong connect_ref_length (domain_bridge.connectRef.length ());
              domain_bridge.connectRef.length (connect_ref_length + 1);

              if (node->hasAttributes ())
                {
                  named_node_map = node->getAttributes ();
                  length = named_node_map->getLength ();


                  for (j = 0; j < length; j++)
                    {
                      attribute_node = named_node_map->item (j);
                      strattrnodename = attribute_node->getNodeName ();
                      attrnodename = XMLString::transcode (attribute_node->getNodeName ());
                      attrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      aceattrnodevalue = XMLString::transcode (attribute_node->getNodeValue ());
                      if (strattrnodename == XStr (ACE_TEXT ("XMI:IDREF")))
                        {
                          domain_bridge.connectRef[connect_ref_length] = attrnodevalue;
                        }
                      else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
                        {
                          domain_bridge.connectRef[connect_ref_length] = attrnodevalue;
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
              break;
            }

        }

      return;

    }

  }
}

#endif /* DOMAIN_HANDLER_C */

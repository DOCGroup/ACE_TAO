// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "CAD_Handler.h"
#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "CompPkgDesc_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    CAD_Handler::CAD_Handler (DOMDocument* doc, unsigned long filter)
      : doc_ (doc),
        root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc_->createNodeIterator (this->root_,
                                         this->filter_,
                                         0,
                                         true)),
        release_ (true)
    {}

    CAD_Handler::CAD_Handler (DOMNodeIterator* iter, bool release)
      : doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    CAD_Handler::~CAD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the connection resource deployment description and populate it
    void CAD_Handler::process_ComponentAssemblyDescription
       (::Deployment::ComponentAssemblyDescription& cad)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("instance")))
            {
              this->process_instance_element (node, this->doc_,
                                              this->iter_,
                                              cad);
            }
          else if (node_name == XStr (ACE_TEXT ("connection")))
            {
              this->process_connection_element (node, this->doc_,
                                                this->iter_,
                                                cad);
            }
          else if (node_name == XStr (ACE_TEXT ("externalProperty")))
            {
              this->process_property_element (node, this->doc_,
                                              this->iter_,
                                              cad);
            }
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
              ref_value = cad.externalProperty[x].delegatesTo[y].instance;
              if (idref_map_.find (ref_value, ref_name) == 0)
                {
                  if (id_map_.find (ref_name, value) == 0)
                    {
                      cad.externalProperty[x].delegatesTo[y].instance= value;
                    }
                }
            }
        }
    }

    void CAD_Handler::process_instance_element 
       (DOMNode* node,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ComponentAssemblyDescription& cad)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (cad.instance.length ());
          cad.instance.length (i + 1);
          if (length == 1)
            {
              this->process_instance
                (doc, iter, cad.instance[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_instance
                (named_node_map, doc,
                 iter, i, cad.instance[i]);
            }
        }
    }

    void CAD_Handler::process_connection_element
       (DOMNode* node,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ComponentAssemblyDescription& cad)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (cad.connection.length ());
          cad.connection.length (i + 1);
          if (length == 1)
            {
              this->process_connection
                (doc, iter, cad.connection[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_connection
                (named_node_map, doc,
                 iter, i, cad.connection[i]);
            }
        }
    }

    void CAD_Handler::process_property_element
       (DOMNode* node,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ComponentAssemblyDescription& cad)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (cad.externalProperty.length ());
          cad.externalProperty.length (i + 1);
          if (length == 1)
            {
              this->process_property
                (doc, iter, cad.externalProperty[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, cad.externalProperty[i]);
            }
        }
    }

    void CAD_Handler::process_instance 
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::SubcomponentInstantiationDescription& sid)
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
              this->process_instance_name (text->getNodeValue (), sid);
            }
          else if (node_name == XStr (ACE_TEXT ("package")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong package_length
                    (sid.package.length ());
                  sid.package.length (package_length + 1);
                  if (length == 1)
                    {
                      this->process_package (doc, iter,
                         sid.package[package_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_package
                        (named_node_map, doc,
                         iter, package_length,
                         sid.package[package_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("configProperty")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong property_length
                    (sid.configProperty.length ());
                  sid.configProperty.length (property_length + 1);
                  if (length == 1)
                    {
                      Property_Handler::process_Property
                        (iter, sid.configProperty[property_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_config_property
                        (named_node_map, doc,
                         iter, property_length, 
                         sid.configProperty[property_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("selectRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong req_length
                    (sid.selectRequirement.length ());
                  sid.selectRequirement.length (req_length + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement
                        (iter,
                         sid.selectRequirement[req_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_req
                        (named_node_map, doc,
                         iter, req_length, 
                         sid.selectRequirement[req_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("reference")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong ref_length
                    (sid.reference.length ());
                  sid.reference.length (ref_length + 1);
                  if (length == 1)
                    {
                      this->process_reference (doc, iter,
                        sid.reference[ref_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_reference
                        (named_node_map, doc,
                         iter, ref_length,
                         sid.reference[ref_length]);
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

    void CAD_Handler::process_attributes_for_req
      (DOMNamedNodeMap* named_node_map,
       DOMDocument* doc,
       DOMNodeIterator* iter,
       int value,
       Deployment::Requirement& req)
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
              Requirement_Handler::process_Requirement
                        (iter, req);
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
              Requirement_Handler::process_Requirement
                        (href_iter, req);
            }
        }

      return;
    }

    void CAD_Handler::process_attributes_for_config_property
      (DOMNamedNodeMap* named_node_map,
       DOMDocument* doc,
       DOMNodeIterator* iter,
       int value,
       Deployment::Property& config_property)
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
                                                  config_property);
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
                                                  config_property);
            }
        }

      return;
    }

    void CAD_Handler::process_attributes_for_reference
      (DOMNamedNodeMap* named_node_map,
       DOMDocument* doc,
       DOMNodeIterator* iter,
       int value,
       Deployment::ComponentPackageReference& sid_ref)
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
              this->process_reference
                 (doc, iter, sid_ref);
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
              this->process_reference(href_doc,
                                     href_iter,
                                     sid_ref);
            }
        }

      return;

    }

    void CAD_Handler::process_reference
      (DOMDocument*,
       DOMNodeIterator* iter,
       Deployment::ComponentPackageReference& sid_ref)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("requiredUUID")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ref_uuid (text->getNodeValue (), sid_ref);
            }
          else if (node_name == XStr (ACE_TEXT ("requiredName")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ref_name (text->getNodeValue (), sid_ref);
            }
          else if (node_name == XStr (ACE_TEXT ("requiredType")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_ref_type (text->getNodeValue (), sid_ref);
            }
          else
            {
              iter->previousNode();
              return;
            }
        }
    }

    void CAD_Handler::process_ref_uuid
      (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref)
    {
      if (name)
        {
          sid_ref.requiredUUID = XMLString::transcode (name);
        }
    }

    void CAD_Handler::process_ref_name
      (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref)
    {
      if (name)
        {
          sid_ref.requiredName = XMLString::transcode (name);
        }
    }

    void CAD_Handler::process_ref_type
      (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref)
    {
      if (name)
        {
          sid_ref.requiredType = XMLString::transcode (name);
        }
    }

    void CAD_Handler::process_attributes_for_package
      (DOMNamedNodeMap* named_node_map,
       DOMDocument* doc,
       DOMNodeIterator* iter,
       int value,
       Deployment::ComponentPackageDescription& sid_package)
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
              CompPkgDesc_Handler handler (iter, false);
              handler.process_ComponentPackageDescription (sid_package);
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
              CompPkgDesc_Handler handler (href_iter, false);
              handler.process_ComponentPackageDescription (sid_package);
            }
        }

      return;
    }

    void CAD_Handler::process_package
      (DOMDocument*,
       DOMNodeIterator* iter,
       Deployment::ComponentPackageDescription& sid_package)
    {
      CompPkgDesc_Handler handler (iter, false);
      handler.process_ComponentPackageDescription (sid_package);
    }
 
    void CAD_Handler::process_instance_name
      (const XMLCh* name,
       Deployment::SubcomponentInstantiationDescription& sid)
    {
      if (name)
        {
          sid.name = XMLString::transcode (name);
        }
    }

    void CAD_Handler::process_attributes_for_instance
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::SubcomponentInstantiationDescription& sid)
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
              this->process_instance
                 (doc, iter, sid);
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
              this->process_instance(href_doc,
                                     href_iter,
                                     sid);
            }
        }

      return;
    }

    void CAD_Handler::process_connection
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::AssemblyConnectionDescription& acd)
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
              this->process_acd_name (text->getNodeValue (), acd);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong req_length
                    (acd.deployRequirement.length ());
                  acd.deployRequirement.length (req_length + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement
                        (iter,
                         acd.deployRequirement[req_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_req
                        (named_node_map, doc,
                         iter, req_length, 
                         acd.deployRequirement[req_length]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("internalEndpoint")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong spe_length
                    (acd.internalEndpoint.length ());
                  acd.internalEndpoint.length (spe_length + 1);
                  if (length == 1)
                    {
                      this->process_spe
                        (doc, iter,
                         acd.internalEndpoint[spe_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_spe
                        (named_node_map, doc,
                         iter, spe_length, 
                         acd.internalEndpoint[spe_length]);
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

    void CAD_Handler::process_acd_name (const XMLCh* name,
      Deployment::AssemblyConnectionDescription& acd)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          acd.name = value.in ();
        }
    }

    void CAD_Handler::process_attributes_for_connection
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::AssemblyConnectionDescription& acd)
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
              this->process_connection (doc,
                                iter,
                                acd);
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
              this->process_connection (href_doc,
                                       href_iter,
                                       acd);
            }
        }

      return;
    }
      
    void CAD_Handler::process_spe
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::SubcomponentPortEndpoint& spe)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("portName")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_spe_name (text->getNodeValue (), spe);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              spe.instance = 0;
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

    void CAD_Handler::process_spe_name (const XMLCh* name,
      Deployment::SubcomponentPortEndpoint& spe)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          spe.portName = value.in ();
        }
    }

    void CAD_Handler::process_attributes_for_spe
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::SubcomponentPortEndpoint& spe)
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
              this->process_spe (doc,
                                iter,
                                spe);
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
              this->process_spe (href_doc,
                                 href_iter,
                                 spe);
            }
        }

      return;
    }

    void CAD_Handler::process_spr
       (DOMDocument*,
        DOMNodeIterator* iter,
        Deployment::SubcomponentPropertyReference& spr)
    {
      for (DOMNode* node = iter->nextNode();
           node != 0;
           node = iter->nextNode ())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("requiredUUID")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_spr_name (text->getNodeValue (), spr);
            }
          else if (node_name == XStr (ACE_TEXT ("instance")))
            {
              spr.instance = 0;
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

    void CAD_Handler::process_refs (DOMNamedNodeMap* named_node_map)
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

    void CAD_Handler::process_spr_name (const XMLCh* name,
      Deployment::SubcomponentPropertyReference& spr)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          spr.propertyName = value.in ();
        }
    }

    void CAD_Handler::process_attributes_for_spr
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::SubcomponentPropertyReference& spr)
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
              this->process_spr (doc,
                                iter,
                                spr);
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
              this->process_spr (href_doc,
                                 href_iter,
                                 spr);
            }
        }

      return;
    }

    void CAD_Handler::process_property
       (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::AssemblyPropertyMapping& apm)
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
              this->process_apm_name (text->getNodeValue (), apm);
            }
          else if (node_name == XStr (ACE_TEXT ("externalName")))
            {
              node = iter->nextNode ();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_apm_ext_name (text->getNodeValue (), apm);
            }
          else if (node_name == XStr (ACE_TEXT ("delegatesTo")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong spr_length
                    (apm.delegatesTo.length ());
                  apm.delegatesTo.length (spr_length + 1);
                  if (length == 1)
                    {
                      this->process_spr
                        (doc, iter,
                         apm.delegatesTo[spr_length]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_spr
                        (named_node_map, doc,
                         iter, spr_length, 
                         apm.delegatesTo[spr_length]);
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

    void CAD_Handler::process_apm_name (const XMLCh* name,
      Deployment::AssemblyPropertyMapping& apm)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          apm.name = value.in ();
        }
    }

    void CAD_Handler::process_apm_ext_name (const XMLCh* name,
      Deployment::AssemblyPropertyMapping& apm)
    {
      if (name)
        {
          CORBA::String_var value (XMLString::transcode (name));
          apm.externalName = value.in ();
        }
    }

    void CAD_Handler::process_attributes_for_property
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::AssemblyPropertyMapping& apm)
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
              this->process_property (doc,
                                iter,
                                apm);
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
              this->process_property (href_doc,
                                       href_iter,
                                       apm);
            }
        }

      return;
    }

    DOMDocument* CAD_Handler::create_document (const char *url)
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
      
  }
}

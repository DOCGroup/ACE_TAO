//==================================================================
/**
 *  @file  IAD_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "NIA_Handler.h"
#include "IAD_Handler.h"

#include <iostream>

using std::cerr;
using std::endl;

namespace CIAO
{
  namespace Config_Handler
  {
    IAD_Handler::IAD_Handler (DOMDocument* doc, unsigned long filter)
      : root_ (doc->getDocumentElement()),
        filter_ (filter),
        iter_ (doc->createNodeIterator (this->root_,
                                              this->filter_,
                                              0,
                                              true)),
        release_ (true)
    {}

    IAD_Handler::IAD_Handler (DOMNodeIterator* iter, bool release)
      : root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    IAD_Handler::~IAD_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    /// handle the package configuration and populate it
    void IAD_Handler::process_ImplementationArtifactDescription
      (::Deployment::ImplementationArtifactDescription &iad)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName());
          if (node_name == XStr (ACE_TEXT ("label")))
            {
              // Fetch the text node which contains the "label"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_label (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("UUID")))
            {
              // Fetch the text node which contains the "UUID"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_UUID (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("location")))
            {
              // Fetch the text node which contains the "location"
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              this->process_location (text->getNodeValue(), iad);
            }
          else if (node_name == XStr (ACE_TEXT ("execParameter")))
            {
              this->process_exec_parameter_element (node,
                                                    this->doc_,
                                                    this->iter_,
                                                    iad);
            }
          else if (node_name == XStr (ACE_TEXT ("infoProperty")))
            {
              this->process_info_property_element (node,
                                                   this->doc_,
                                                   this->iter_,
                                                   iad);
            }
          else if (node_name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (iad.deployRequirement.length ());
                  iad.deployRequirement.length (i + 1);
                  if (length == 1)
                    {
                      Requirement_Handler::process_Requirement
                         (this->iter_,
                          iad.deployRequirement[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deploy_requirement
                       (named_node_map, this->doc_,
                        this->iter_, i, iad.deployRequirement[i]);
                    }
                }
            }
          else if (node_name == XStr (ACE_TEXT ("dependsOn")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (iad.dependsOn.length ());
                  iad.dependsOn.length (i + 1);
                  if (length == 1)
                    {
                      NIA_Handler nia_handler (iter_, false);
                      nia_handler.process_NamedImplementationArtifact
                        (iad.dependsOn[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_nia
                       (named_node_map, this->doc_,
                        this->iter_, i, iad.dependsOn[i]);
                    }
                }
            }
          else
            {
              // ??? How did we get here ???
              ACE_THROW (CORBA::INTERNAL());
            }
        }
      return;
    }

    /// handle label attribute
    void IAD_Handler::process_label
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.label = XMLString::transcode (name);
        }
    }

    /// handle UUID attribute
    void IAD_Handler::process_UUID
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.UUID = XMLString::transcode (name);
        }
    }

    /// handle location attribute
    void IAD_Handler::process_location
      (const XMLCh* name, ::Deployment::ImplementationArtifactDescription &iad)
    {
      if (name)
        {
          iad.location = XMLString::transcode (name);
        }
    }

    // handle exec parameter element
    void IAD_Handler::process_exec_parameter_element (DOMNode* node,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          Deployment::ImplementationArtifactDescription& iad)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (iad.execParameter.length ());
          iad.execParameter.length (i + 1);
          if (length == 1)
            {
              Property_Handler::process_Property
                 (iter, iad.execParameter[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, iad.execParameter[i]);
            }
        }
    }

    // handle info property element
    void IAD_Handler::process_info_property_element (DOMNode* node,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          Deployment::ImplementationArtifactDescription& iad)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (iad.infoProperty.length ());
          iad.infoProperty.length (i + 1);
          if (length == 1)
            {
              Property_Handler::process_Property
                 (iter, iad.infoProperty[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, iad.infoProperty[i]);
            }
        }
    }

    void IAD_Handler::process_attributes_for_property
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Property& iad_property)
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
                                                  iad_property);
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
                                                  iad_property);
            }
        }

      return;
    }

    DOMDocument* IAD_Handler::create_document (const char *url)
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

    void IAD_Handler::process_attributes_for_deploy_requirement
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Requirement& iad_req)
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
              id_map_.bind (aceattrnodevalue, value);
              Requirement_Handler::process_Requirement
                  (iter,
                   iad_req);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->process_refs (named_node_map);
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
                  (href_iter,
                   iad_req);
            }
        }

      return;
    }

    void IAD_Handler::process_attributes_for_nia
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::NamedImplementationArtifact &nia)
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
              NIA_Handler nia_handler (iter, false);
              nia_handler.process_NamedImplementationArtifact
                 (nia);
              id_map_.bind (aceattrnodevalue, value);
            }
          else if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              this->process_refs (named_node_map);
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
              NIA_Handler nia_handler (href_iter, false);
              nia_handler.process_NamedImplementationArtifact
                 (nia);
            }
        }

      return;
    }

    void IAD_Handler::process_refs (DOMNamedNodeMap* named_node_map)
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

  }  // namespace Config_Handler
}  // namespace CIAO

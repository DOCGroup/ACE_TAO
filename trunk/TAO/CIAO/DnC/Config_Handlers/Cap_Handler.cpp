//==================================================================
/**
 *  @file  Cap_Handler.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */
//==================================================================

#include "Cap_Handler.h"
#include "SP_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"
#include <iostream>
#include "string.h"

namespace CIAO
{
  namespace Config_Handler
  {

    CAP_Handler::CAP_Handler (DOMDocument* doc, unsigned long filter)
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

    CAP_Handler::CAP_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter),
        release_ (release)
    {}

    CAP_Handler::~CAP_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    void
    CAP_Handler::process_Capability (DOMNodeIterator * iter,
       Deployment::Capability &cap)
    {
      //Check if the Schema IDs for both the elements match
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr node_name (node->getNodeName ());
          if (node_name == XStr (ACE_TEXT ("name")))
            {
              node = this->iter_->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              cap.name = (XMLString::transcode (text->getNodeValue ()));
            }
          else if (node_name == XStr (ACE_TEXT ("resourceType")))
            {
              node = iter->nextNode();
              DOMText* text = ACE_reinterpret_cast (DOMText*, node);
              CORBA::ULong resource_type_length =
                cap.resourceType.length ();
              cap.resourceType.length (resource_type_length + 1);
              cap.resourceType[resource_type_length] =
                 XMLString::transcode (text->getNodeValue ());
            }
          else if (node_name == XStr (ACE_TEXT ("property")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i (cap.property.length ());
                  cap.property.length (i + 1);
                  if (length == 1)
                    {
                      SP_Handler::process_SatisfierProperty
                         (iter, cap.property[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_satisfier_property
                        (named_node_map, this->doc_,
                         iter, i, cap.property[i]);
                    }
                }
            }
          else
            {
              iter->previousNode ();
              break;
            }
        }
      return;
    }

    void CAP_Handler::process_attributes_for_satisfier_property
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

    DOMDocument* CAP_Handler::create_document (const char *url)
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

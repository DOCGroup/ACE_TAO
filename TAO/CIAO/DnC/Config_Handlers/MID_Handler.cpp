// $Id$

#include "tao/Exception.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"

#include "Property_Handler.h"
#include "Requirement_Handler.h"
#include "NIA_Handler.h"
#include "MID_Handler.h"
#include "IR_Handler.h"
#include "Process_Element.h"

#include <iostream>

using std::cerr;
using std::endl;


namespace CIAO
{
  namespace Config_Handler
  {
    MID_Handler::MID_Handler (DOMDocument* doc, unsigned long filter)
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

    MID_Handler::MID_Handler (DOMNodeIterator* iter, bool release)
      : traverse_ (0), doc_ (0), root_ (0), filter_ (0), iter_ (iter), release_ (release)
    {}


    MID_Handler::~MID_Handler()
    {
      if (this->release_)
        this->iter_->release();
    }

    void MID_Handler::process_MonolithicImplementationDescription
      (::Deployment::MonolithicImplementationDescription &mid)
    {
      for (DOMNode* node = this->iter_->nextNode();
           node != 0;
           node = this->iter_->nextNode())
        {
          XStr name (node->getNodeName());

          if (name == XStr (ACE_TEXT ("execParameter")))
            {
              this->process_exec_parameter_element (node,
                                                    this->doc_,
                                                    this->iter_,
                                                    mid);
            }
          else if (name == XStr (ACE_TEXT ("deployRequirement")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (mid.deployRequirement.length ());
                  mid.deployRequirement.length (i + 1);
                  if (length == 1)
                    {
                      IR_Handler ir_handler (iter_, false);
                      ir_handler.process_ImplementationRequirement
                        (mid.deployRequirement[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_deploy_requirement
                       (named_node_map, this->doc_,
                        this->iter_, i, mid.deployRequirement[i]);
                    }
                }
            }
          else if (name == XStr (ACE_TEXT ("primaryArtifact")))
            {
              if (node->hasAttributes ())
                {
                  DOMNamedNodeMap* named_node_map = node->getAttributes ();
                  int length = named_node_map->getLength ();
                  CORBA::ULong i
                    (mid.primaryArtifact.length ());
                  mid.primaryArtifact.length (i + 1);
                  if (length == 1)
                    {
                      NIA_Handler nia_handler (iter_, false);
                      nia_handler.process_NamedImplementationArtifact
                        (mid.primaryArtifact[i]);
                    }
                  else if (length > 1)
                    {
                      this->process_attributes_for_nia
                       (named_node_map, this->doc_,
                        this->iter_, i, mid.primaryArtifact[i]);
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

    void MID_Handler::process_attributes_for_nia
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

    // handle exec parameter element
    void MID_Handler::process_exec_parameter_element (DOMNode* node,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          Deployment::MonolithicImplementationDescription& mid)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();
          CORBA::ULong i (mid.execParameter.length ());
          mid.execParameter.length (i + 1);
          if (length == 1)
            {
              Property_Handler::process_Property
                 (iter, mid.execParameter[i]);
            }
          else if (length > 1)
            {
              this->process_attributes_for_property
                (named_node_map, doc,
                 iter, i, mid.execParameter[i]);
            }
        }
    }

    void MID_Handler::process_attributes_for_property
         (DOMNamedNodeMap* named_node_map,
          DOMDocument* doc,
          DOMNodeIterator* iter,
          int value,
          Deployment::Property& mid_property)
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
                                                  mid_property);
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
                                                  mid_property);
            }
        }

      return;
    }

    DOMDocument* MID_Handler::create_document (const char *url)
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

    void MID_Handler::process_attributes_for_deploy_requirement
       (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ImplementationRequirement& mid_req)
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
              IR_Handler ir_handler (iter, false);
              ir_handler.process_ImplementationRequirement
                (mid_req);
/*
              Requirement_Handler::process_Requirement
                  (iter,
                   mid_req);
*/
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
              IR_Handler ir_handler (iter_, false);
              ir_handler.process_ImplementationRequirement
                (mid_req);
            }
        }

      return;
    }


    void MID_Handler::process_refs (DOMNamedNodeMap* named_node_map)
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

//==================================================================
/**
 *  @file Process_Element.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element.h"
#include "ace/Auto_Ptr.h"

DOMDocument* create_document (const char *url)
{
  xercesc::XMLPlatformUtils::Initialize();
  static const XMLCh gLS[] = { xercesc::chLatin_L,
                               xercesc::chLatin_S,
                               xercesc::chNull };

  DOMImplementation* impl
    = DOMImplementationRegistry::getDOMImplementation(gLS);
  auto_ptr<DOMImplementation> cleanup_impl (impl);

  DOMBuilder* parser =
    ((DOMImplementationLS*)impl)->createDOMBuilder
    (DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  auto_ptr<DOMBuilder> cleanup_parser (parser);

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
  auto_ptr<DOMDocument> cleanup_doc (doc);
  ACE_TString root_node_name;
  char* root_node_name_ch = XMLString::transcode
    (doc->getDocumentElement ()->getNodeName ());
  root_node_name = root_node_name_ch;
  XMLString::release (&root_node_name_ch);

  return doc;
}

/*
 *  Process references
 */

void
process_refs(DOMNode*& node,
             CORBA::ULongSeq& seq,
             int& index,
             IDREF_MAP& idref_map)
{
  CORBA::ULong i (seq.length ());
  seq.length (i + 1);
  seq[i] = index;
  if (node->hasAttributes())
    {
      DOMNamedNodeMap* named_node_map = node->getAttributes ();
      auto_ptr<DOMNamedNodeMap> cleanup_nodemap (named_node_map);
      
      int length = named_node_map->getLength ();
      
      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          char* aceattrnodevalue_ch = XMLString::transcode
            (attribute_node->getNodeValue ());
          ACE_TString aceattrnodevalue = aceattrnodevalue_ch;
          XMLString::release (aceattrnodevalue_ch);
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              //index = index + 1;
              idref_map.bind (index, aceattrnodevalue);
            }
        }
    }
}

void
process_ref(DOMNode*& node,
            CORBA::ULong& ref,
            int& index,
            IDREF_MAP& idref_map)
{
  ref = index;
  if (node->hasAttributes ())
    {
      DOMNamedNodeMap* named_node_map = node->getAttributes ();
      auto_ptr<DOMNamedNodeMap> cleanup_nodemap (named_node_map);
      
      int length = named_node_map->getLength ();
      
      for (int j = 0; j < length; j++)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          char* aceattrnodevalue_ch = XMLString::transcode
            (attribute_node->getNodeValue ());
          ACE_TString aceattrnodevalue = aceattrnodevalue_ch;
          XMLString::release (aceattrnodevalue_ch);
          if (strattrnodename == XStr (ACE_TEXT ("xmi:idref")))
            {
              //index = index + 1;
              idref_map.bind (index, aceattrnodevalue);
            }
        }
    }
}

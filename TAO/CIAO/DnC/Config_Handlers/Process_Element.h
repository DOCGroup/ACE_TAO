//==================================================================
/**
 *  @file Process_Element.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef PROCESS_ELEMENT_H
#define PROCESS_ELEMENT_H

inline
DOMDocument* create_document (const char *url)
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


/*
 *  For non-static process functions.  The function prototype should be:
 *  
 *  void OBJECT::process_function (DOMDocument*, DOMNodeIterator*, DATA&);
 *
 */

template <typename VALUE, typename DATA, typename OBJECT>
inline void process_element_attributes(DOMNamedNodeMap* named_node_map,
                                       DOMDocument* doc,
                                       DOMNodeIterator* iter,
                                       VALUE value,
                                       DATA& data,
                                       OBJECT& obj,
                                       void (OBJECT::*func)(DOMDocument*,
                                                            DOMNodeIterator*,
                                                            DATA&),
                                       ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex>& id_map)
{
  // the number of attributes
  int length = named_node_map->getLength();
  // iterate the attributes
  for (int j = 0; j < length; ++j)
    {
      DOMNode* attribute_node = named_node_map->item (j);
      XStr strattrnodename (attribute_node->getNodeName ());
      ACE_TString aceattrnodevalue =
        XMLString::transcode (attribute_node->getNodeValue ());

      // if xmi::id is given process the element and bind the value
      if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
        {
          (obj.*func)(doc, iter, data);
          id_map.bind (aceattrnodevalue, value);
        }
      // if href is given find out the referenced position
      // and process the element
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
              href_doc = create_document(final_url.c_str ());
            }
          else
            {
              href_doc = create_document (url_string.c_str ());
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
          
          (obj.*func) (href_doc, href_iter, data);
        }
    }
}

// This function only works for calling static process_ methods
template <typename DATA, typename VALUE, typename OBJECT>
inline void process_element (DOMNode* node,
                             DOMDocument* doc,
                             DOMNodeIterator* iter,
                             DATA& data,
                             VALUE val,
                             OBJECT& obj,
                             void (OBJECT::*func)(DOMDocument*,
                                                  DOMNodeIterator*,
                                                  DATA&),
                             ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex>& id_map)
{
  // fetch attributes
  DOMNamedNodeMap* named_node_map = node->getAttributes ();
  // the number of attributes the element have
  int length = named_node_map->getLength();
  // if there is no other attribute but 'version'
  if (length == 1)
    {
      // call directly the static process_ method
      (obj.*func)(doc, iter, data);
    }
  else if (length > 1)
    {
      // Check the xmi::id & href attributes
      process_element_attributes(named_node_map, doc, iter, val, data, obj, func, id_map);
    }
}

// This function only works for calling static process_ methods
template <typename SEQUENCE, typename DATA, typename OBJECT>
inline void process_sequential_element (DOMNode* node,
                                        DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        SEQUENCE& seq,
                                        OBJECT& obj,
                                        void (OBJECT::*func)(DOMDocument*,
                                                             DOMNodeIterator*,
                                                             DATA&),
                                        ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex>& id_map)
{
  if (node->hasAttributes ())
    {
      // the size of the sequence
      CORBA::ULong i (seq.length ());
      // add 1 to the size of the sequence
      seq.length (i + 1);
      // call process only one element
      process_element(node, doc, iter, seq[i], i, obj, func, id_map);
    }
}

#endif // PROCESS_ELEMENT_H

// -*-C++-*-
//=====================================================================
/**
 *  @file Process_Element.i
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element.h"
#include <iostream>

template <typename OBJECT, typename VALUE, typename DATA>
void process_element_attributes(DOMNamedNodeMap* named_node_map,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                VALUE value,
                                DATA& data,
                                Process_Function <DATA>* func,
                                REFMAP& id_map)
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
          (*func) (iter, data);
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

          static_cast< Process_Member_Function<OBJECT, DATA>* > (func)->doc(href_doc);
          (*func) (href_iter, data);
        }
    }
}

// This function only works for calling static process_ methods
template <typename OBJECT, typename DATA, typename VALUE>
 void process_element (DOMNode* node,
                       DOMDocument* doc,
                       DOMNodeIterator* iter,
                       DATA& data,
                       VALUE val,
                       Process_Function <DATA>* func,
                       REFMAP& id_map)
{
  // fetch attributes
  DOMNamedNodeMap* named_node_map = node->getAttributes ();
  // the number of attributes the element have
  int length = named_node_map->getLength();
  // if there is no other attribute but 'version'
  if (length == 1)
    {
      // call directly the static process_ method
      (*func) (iter, data);
    }
  else if (length > 1)
    {
      // Check the xmi::id & href attributes
      process_element_attributes<OBJECT>(named_node_map, doc, iter, val, data, func, id_map);
    }
}

// This function only works for calling static process_ methods
template <typename OBJECT, typename SEQUENCE, typename DATA>
 void process_sequential_element (DOMNode* node,
                                  DOMDocument* doc,
                                  DOMNodeIterator* iter,
                                  SEQUENCE& seq,
                                  Process_Function <DATA>* func,
                                  REFMAP& id_map)
{
  if (node->hasAttributes ())
    {
      // the size of the sequence
      CORBA::ULong i (seq.length ());
      // add 1 to the size of the sequence
      seq.length (i + 1);
      // call process only one element
      process_element<OBJECT>(node, doc, iter, seq[i], i, func, id_map);
    }
}

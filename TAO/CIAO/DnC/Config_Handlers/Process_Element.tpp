//=====================================================================
/**
 *  @file Process_Element.tpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element.h"
#include "Config_Handlers/Config_Error_Handler.h"
#include "Utils.h"
#include <iostream>
#include <memory>
#if !defined (__ACE_INLINE__)
#include "Process_Element.i"
#endif /* __ACE_INLINE__ */
BEGIN_DEPLOYMENT_NAMESPACE

template <typename VALUE, typename DATA>
void process_element_attributes(DOMNamedNodeMap* named_node_map,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                VALUE value,
                                DATA& data,
                                Process_Function <DATA>* func,
                                REF_MAP& id_map)
{
  try
    {
      // the number of attributes
      int length = named_node_map->getLength();
      // iterate the attributes
      for (int j = 0; j < length; ++j)
        {
          DOMNode* attribute_node = named_node_map->item (j);
          XStr strattrnodename (attribute_node->getNodeName ());
          char* aceattrnodevalue_ch =
            XMLString::transcode (attribute_node->getNodeValue ());
          ACE_TString aceattrnodevalue = aceattrnodevalue_ch;
          XMLString::release (&aceattrnodevalue_ch);

          // if xmi::id is given process the element and bind the value
          if (strattrnodename == XStr (ACE_TEXT ("xmi:id")))
            {
              (*func) (doc, iter, data);
              id_map.bind (aceattrnodevalue, value);
            }
          // if href is given find out the referenced position
          // and process the element
          else if (strattrnodename == XStr (ACE_TEXT ("href")))
            {
              XMLURL xml_url (aceattrnodevalue.c_str ());
              XMLURL result (aceattrnodevalue.c_str ());
              std::string url_string = aceattrnodevalue.c_str ();
              char* doc_path_ch =
                XMLString::transcode ( doc->getDocumentURI ());
              ACE_TString doc_path = doc_path_ch;
              XMLString::release (&doc_path_ch);
              XMLCh* rel_str =
                (XMLString::transcode (doc_path.c_str ()));
              result.makeRelativeTo
                (rel_str);
              char* final_url_ch =
                XMLString::transcode (result.getURLText ());
              ACE_TString final_url = final_url_ch;
              XMLString::release (&final_url_ch);
              XMLString::release (&rel_str);

              DOMDocument* href_doc;

              std::auto_ptr<DOMBuilder> parser (
                      CIAO::Config_Handler::Utils::create_parser  ());

              CIAO::Config_Handler::Config_Error_Handler handler;
              parser->setErrorHandler(&handler);
                      
              if (xml_url.isRelative ())
                {
                  href_doc = parser->parseURI (final_url.c_str ());
                }
              else
                {
                  href_doc = parser->parseURI (url_string.c_str ());
                }

              DOMDocumentTraversal* traverse = href_doc;
              DOMNode* root = (href_doc->getDocumentElement ());
              unsigned long filter = DOMNodeFilter::SHOW_ELEMENT |
                DOMNodeFilter::SHOW_TEXT;
              DOMNodeIterator* href_iter = traverse->createNodeIterator
                (root,
                 filter,
                 0,
                 true);
              href_iter->nextNode ();
              (*func) (href_doc, href_iter, data);
            }
        }
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "caught DOM exception\n"));
    }
}

template <typename SEQUENCE, typename DATA>
void
process_sequential_element (DOMNode* node,
                            DOMDocument* doc,
                            DOMNodeIterator* iter,
                            SEQUENCE& seq,
                            Process_Function <DATA>* func,
                            REF_MAP& id_map)
{
  if (node->hasAttributes ())
    {
      // the size of the sequence
      CORBA::ULong i = seq.length ();

      // add 1 to the size of the sequence
      seq.length (i + 1);

      // fetch attributes
      DOMNamedNodeMap* named_node_map = node->getAttributes ();

      // the number of attributes the element have
      int length = named_node_map->getLength();
      // if there is no other attribute but 'version'

      if (length == 1) // call directly the static process_ method
        (*func) (doc, iter, seq[i]);
      else             // Check the xmi::id & href attributes
        process_element_attributes(named_node_map, doc, iter, i, seq[i], func, id_map);
    }
}

END_DEPLOYMENT_NAMESPACE

//=====================================================================
/**
 *  @file Process_Element_T.cpp
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 *          Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element_T.h"
#include "Config_Handlers/Config_Error_Handler.h"
#include "ace/Auto_Ptr.h"
#include "Utils.h"
#include <iostream>
#include <memory>

BEGIN_DEPLOYMENT_NAMESPACE

class parser_error { };

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
ACE_INLINE bool
process_sequence_local(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                       XStr& node_name, const char* name,
                       SEQUENCE& seq, OBJECT* obj, FUNCTION func)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      Process_Member_Function<OBJECT, DATA>
        pf(obj, func);
      process_sequential_element (node, doc, iter, seq, &pf, obj->id_map());
    }

  return result;
}

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
ACE_INLINE bool
process_sequence_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode*
node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      OBJECT obj (doc, iter, false);

      Process_Member_Function_Remote<OBJECT, DATA>
        pf(obj, func);
      process_sequential_element (node, doc, iter, seq, &pf, id_map);
    }

  return result;
}

template<typename DATA, typename SEQUENCE, typename FUNCTION>
ACE_INLINE bool
process_sequence_common(DOMDocument* doc, DOMNodeIterator* iter, DOMNode*
node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      Process_Static_Function<DATA>
        pf(func);
      process_sequential_element (node, doc, iter, seq, &pf, id_map);
    }

  return result;
}

template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
ACE_INLINE bool
process_element(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                XStr& node_name, const char* name,
                ELEMENT& elem, OBJECT* obj, FUNCTION func,
                REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();

          Process_Member_Function<OBJECT, DATA>
            pf(obj, func);

          if (length == 1)
              pf(doc, iter, elem);
          else
              process_element_attributes(named_node_map, doc, iter, 0, elem,
&pf, id_map);
        }
    }

  return result;
}

template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
ACE_INLINE bool
process_element_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                       XStr& node_name, const char *name,
                       ELEMENT& elem, FUNCTION func,
                       REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      OBJECT obj (iter, false);

      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();

          Process_Member_Function_Remote<OBJECT, DATA>
            pf(&obj, func);

          if (length == 1)
              pf(doc, iter, elem);
          else
              process_element_attributes(named_node_map, doc, iter, 0, elem,
&pf, id_map);
        }
    }

  return result;
}

template <typename VALUE, typename DATA>
void process_element_attributes(DOMNamedNodeMap* named_node_map,
                                DOMDocument* doc,
                                DOMNodeIterator* iter,
                                VALUE value,
                                DATA& data,
                                Process_Function <DATA>* func,
                                REF_MAP& id_map)
{
  char* final_url_ch;
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
              final_url_ch =
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
                  if (handler.getErrors ())
                    {
                      ACE_DEBUG ((LM_DEBUG, "XML descriptor error\n"));
                      //throw parser_error ();
                      throw DOMException ();
                    }
                }
              else
                {
                  href_doc = parser->parseURI (url_string.c_str ());
                  if (handler.getErrors ())
                    {
                      ACE_DEBUG ((LM_DEBUG, "XML descriptor error\n"));
                      //throw parser_error ();
                      throw DOMException ();
                    }
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
  catch (const DOMException& e)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      ACE_ERROR ((LM_ERROR, "\nException occured while parsing %s: \
                  \n",final_url_ch));
      ACE_ERROR ((LM_ERROR, "DOMException code: %d\n ", e.code));
      if (DOMImplementation::loadDOMExceptionMsg (e.code, errText, maxChars))
        {
          char* message = XMLString::transcode (errText);
          ACE_Auto_Basic_Array_Ptr<char> cleanup_message (message);
          ACE_ERROR ((LM_ERROR, "Message is: %s\n", message));
        }
      ACE_ERROR ((LM_ERROR, "Caught DOM exception\n\n"));
      return;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught unknown exception\n\n"));
      return;
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

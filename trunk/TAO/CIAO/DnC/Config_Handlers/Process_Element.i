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

BEGIN_DEPLOYMENT_NAMESPACE

/*
 *  Process function for member functions - id_map: obj->id_map()
 */

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

/*
 *  Process function for member functions -
 */

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
ACE_INLINE bool
process_sequence_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
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

/*
 *  Process function for static functions
 */

template<typename DATA, typename SEQUENCE, typename FUNCTION>
ACE_INLINE bool
process_sequence_common(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
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

/*
 *  Process function for non-sequential elements
 */

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
              process_element_attributes(named_node_map, doc, iter, 0, elem, &pf, id_map);
        }
    }

  return result;
}

// Process function for non-sequential non-local elements
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
              process_element_attributes(named_node_map, doc, iter, 0, elem, &pf, id_map);
        }
    }

  return result;
}

END_DEPLOYMENT_NAMESPACE

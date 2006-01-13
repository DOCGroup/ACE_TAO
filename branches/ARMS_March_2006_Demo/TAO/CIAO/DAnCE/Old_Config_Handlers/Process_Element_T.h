//==================================================================
/**
 *  @file Process_Element_T.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 *          Jaiganesh Balasubramanian  <jai@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef PROCESS_ELEMENT_T_H
#define PROCESS_ELEMENT_T_H

#include "Process_Element.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

BEGIN_DEPLOYMENT_NAMESPACE

// processes sequence - not for common elements, process function is a 
// member of "this" -
template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
bool
process_sequence_local(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                      XStr& node_name, const char* name,
                      SEQUENCE& seq, OBJECT* obj, FUNCTION func);

// processes sequence - not for common elements, process function is not a 
// member of "this" -
template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
bool
process_sequence_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map);

// Processes sequence - common elements -
template<typename DATA, typename SEQUENCE, typename FUNCTION>
bool
process_sequence_common(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map);

//  Process function for non-sequential elements
template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
bool
process_element(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                XStr& node_name, const char* name,
                ELEMENT& elem, OBJECT* obj, FUNCTION func,
                REF_MAP& id_map);

// Process function for non-sequential non-local elements
template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
bool
process_element_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                       XStr& node_name, const char* name,
                       ELEMENT& elem, OBJECT* obj, FUNCTION func,
                       REF_MAP& id_map);

template <typename SEQUENCE, typename DATA>
void
process_sequential_element (DOMNode* node,
                            DOMDocument* doc,
                            DOMNodeIterator* iter,
                            SEQUENCE& seq,
                            Process_Function <DATA>* func,
                            REF_MAP& id_map);

template <typename VALUE, typename DATA>
void process_element_attributes (DOMNamedNodeMap* named_node_map,
                                 DOMDocument* doc, DOMNodeIterator* iter,
                                 VALUE value, DATA& data,
                                 Process_Function <DATA>* func,
                                 REF_MAP& id_map);

END_DEPLOYMENT_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Process_Element_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Process_Element_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif // PROCESS_ELEMENT_H

// -*-C++-*-
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

DOMDocument* create_document (const char *url);

/*
 *  For non-static process functions.  The function prototype should be:
 *
 *  void OBJECT::process_function (DOMDocument*, DOMNodeIterator*, DATA&);
 *
 */

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
                             ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex>& id_map);

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
                                        ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex>& id_map);

#endif // PROCESS_ELEMENT_H

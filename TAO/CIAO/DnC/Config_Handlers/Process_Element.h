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

#include "DeploymentC.h"
#include "Config_Handler_export.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "tao/Exception.h"
#include "XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLUri;
using xercesc::XMLURL;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMInputSource;
using xercesc::DOMText;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::DOMNamedNodeMap;

// ID map type definition
typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;

// profcesses sequence - not for common elements -
template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                 XStr& node_name, const char* name,
                 SEQUENCE& seq, OBJECT* obj, FUNCTION func,
                 REF_MAP& id_map);

// Processes sequence - common elements -
template<typename DATA, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                 XStr& node_name, const char* name,
                 SEQUENCE& seq, FUNCTION func,
                 REF_MAP& id_map);

// Processes reference sequences
inline bool
process_reference_seq (DOMNode* node,
                       XStr& node_name, const char* name,
                       CORBA::ULongSeq& seq,
                       int& index,
                       IDREF_MAP& idref_map);

// Process reference
inline bool
process_reference (DOMNode* node,
                   XStr& node_name, const char* name,
                   CORBA::ULong& ref,
                   int& index,
                   IDREF_MAP& idref_map);

/*
 *  Process function for non-sequential elements
 */

template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
inline bool
process_element(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                XStr& node_name, const char* name,
                ELEMENT& elem, OBJECT* obj, FUNCTION func,
                REF_MAP& id_map);

#include "Process_Element.i"
#include "Process_Element.tpp"

#endif // PROCESS_ELEMENT_H

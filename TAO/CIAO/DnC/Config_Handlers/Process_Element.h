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

#include <iostream>

// ID map type definition
typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REFMAP;


template <typename DATA>
class Process_Function {
public:
  virtual void call(DOMNodeIterator*, DATA&)=0;

  void operator() (DOMNodeIterator* iter, DATA& data)
  {
    call(iter, data);
  }
};

/*
 *  Wrapper class for the process member functions.
 */

template <typename OBJ, typename DATA>
class Process_Member_Function: public Process_Function<DATA> {
public:
  typedef void (OBJ::*func_type) (DOMNodeIterator*, DATA&);
  typedef DATA data_type;

  Process_Member_Function(OBJ& obj, func_type f)
    : obj_(&obj), f_(f)
  {
  }

  Process_Member_Function(OBJ* obj, func_type f)
    : obj_(obj), f_(f)
  {
  }

  virtual void call(DOMNodeIterator* iter, DATA& data)
  {
    (obj_->*f_) (iter, data);
  }

private:
  OBJ* obj_;
  func_type f_;
};

/*
 *  Wrapper class for the static process member functions.
 */

template <typename DATA>
class Process_Static_Function: public Process_Function<DATA> {
public:
  typedef void (*func_type) (DOMNodeIterator*, DATA&);
  typedef DATA data_type;

  Process_Static_Function(func_type f)
    : f_(f)
  {
  }

  virtual void call(DOMNodeIterator* iter, DATA& data)
  {
    (*f_) (iter, data);
  }

private:
  func_type f_;
};

DOMDocument* create_document (const char *url);

template <typename DATA, typename VALUE>
void process_element (DOMNode* node,
                      DOMDocument* doc,
                      DOMNodeIterator* iter,
                      DATA& data,
                      VALUE val,
                      Process_Function <DATA>* func,
                      REFMAP& id_map);

template <typename SEQUENCE, typename DATA>
void process_sequential_element (DOMNode* node,
                                 DOMDocument* doc,
                                 DOMNodeIterator* iter,
                                 SEQUENCE& seq,
                                 Process_Function <DATA>* func,
                                 REFMAP& id_map);

/*
 *  Process function for member functions
 */

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                 XStr& node_name, const char* name,
                 SEQUENCE& seq, OBJECT* obj, FUNCTION func,
                 REFMAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      Process_Member_Function<OBJECT, DATA>
        pf(obj, func);
      process_sequential_element (node, doc, iter, seq, &pf, id_map);
    }

  return result;
}

/*
 *  Process function for static functions
 */

template<typename DATA, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                 XStr& node_name, const char* name,
                 SEQUENCE& seq, FUNCTION func,
                 REFMAP& id_map)
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

#include "Process_Element.i"

#endif // PROCESS_ELEMENT_H

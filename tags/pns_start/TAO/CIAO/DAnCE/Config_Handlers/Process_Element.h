//==================================================================
/**
 *  @file Process_Element.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 *          Jaiganesh Balasubramanian  <jai@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef PROCESS_ELEMENT_H
#define PROCESS_ELEMENT_H

#include "ciao/DeploymentC.h"
#include "Deployment.h"
#include "Basic_Handler.h"
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

BEGIN_DEPLOYMENT_NAMESPACE

// ID map type definition
typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;

template <typename DATA>
class Process_Function 
{
public:
  virtual void call(DOMDocument*, DOMNodeIterator*, DATA&)=0;

  void operator() (DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    call(doc, iter, data);
  }
};

/*
 *  Wrapper class for the process member functions.
 */

template <typename OBJ, typename DATA>
class Process_Member_Function: public Process_Function<DATA> 
{
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

  virtual void call(DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    obj_->set_doc (doc);
    obj_->set_iter (iter);
    (obj_->*f_) (iter, data);
  }

private:
  OBJ* obj_;
  func_type f_;
};

/*
 *  Wrapper class for the process member functions which does not have 
    DOMNodeIterator parameter
 */

template <typename OBJ, typename DATA>
class Process_Member_Function_Remote: public Process_Function<DATA> 
{
public:
  typedef void (OBJ::*func_type) (DATA&);
  typedef DATA data_type;

  Process_Member_Function_Remote(OBJ& obj, func_type f)
    : obj_(&obj), f_(f)
  {
  }

  Process_Member_Function_Remote(OBJ* obj, func_type f)
    : obj_(obj), f_(f)
  {
  }

  virtual void call(DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    obj_->set_iter(iter);
    obj_->set_doc(doc);
    (obj_->*f_) (data);
  }

private:
  OBJ* obj_;
  func_type f_;
};

/*
 *  Wrapper class for the static process member functions.
 */

template <typename DATA>
class Process_Static_Function: public Process_Function<DATA> 
{
public:
  typedef void (*func_type) (DOMNodeIterator*, DATA&);
  typedef DATA data_type;

  Process_Static_Function(func_type f)
    : f_(f)
  {
  }

  virtual void call(DOMDocument*, DOMNodeIterator* iter, DATA& data)
  {
    (*f_) (iter, data);
  }

private:
  func_type f_;
};

// Processes reference sequences
bool
process_reference_seq (DOMNode* node,
                       XStr& node_name, const char* name,
                       CORBA::ULongSeq& seq,
                       int& index,
                       IDREF_MAP& idref_map);

// Process reference
bool
process_reference (DOMNode* node,
                   XStr& node_name, const char* name,
                   CORBA::ULong& ref,
                   int& index,
                   IDREF_MAP& idref_map);

END_DEPLOYMENT_NAMESPACE

#include "Process_Element_T.h"

#endif // PROCESS_ELEMENT_H

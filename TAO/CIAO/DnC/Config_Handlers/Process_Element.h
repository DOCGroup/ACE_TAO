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

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include <xercesc/dom/DOM.hpp>

using xercesc::DOMDocument;
using xercesc::DOMNode;
using xercesc::DOMNodeIterator;

// ID map type definition
typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;


template <typename OBJ>
class Process_Function {
public:
  virtual void call(DOMNodeIterator* iter, DATA& data) = 0;

  void operator() (DOMNodeIterator* iter, DATA& data)
  {
    call(iter, data);
  }
};

/*
 *  Wrapper class for the process member functions.
 */

template <typename OBJ>
class Process_Member_Function: public Process_Function<OBJ> {
public:
  typedef void (OBJ::*func_type) (DOMDocument*, DOMNodeIterator*, DATA&);

  Process_Function(OBJ& obj, func_type f, DOMDocument* doc)
    : obj_(&obj), f_(f), doc_(doc)
  {
  }

  Process_Function(OBJ* obj, func_type f, DOMDocument* doc)
    : obj_(obj), f_(f), doc_(doc)
  {
  }

  Process_Function& operator=(func_type ft)
  {
    f_ = f;
  }

  virtual void call(DOMNodeIterator* iter, DATA& data)
  {
    (obj_->*f_) (doc_, iter, data);
  }

  void doc(DOMDocument* val)
  {
    doc_ = val;
  }

private:
  OBJ* obj_;
  func_type f_;
  DOMDocument* doc_;
};

/*
 *  Wrapper class for the static process member functions.
 */

class Process_Static_Function: public Process_Function<OBJ> {
public:
  typedef void (*func_type) (DOMNodeIterator*, DATA&);

  Process_Function(func_type f)
    : f_(f)
  {
  }

  Process_Function(func_type f)
    : f_(f)
  {
  }

  Process_Function& operator=(func_type ft)
  {
    f_ = f;
  }

  virtual void call(DOMNodeIterator* iter, DATA& data)
  {
    (*f_) (iter, data);
  }

private:
  func_type f_;
};

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
                             Process_Function <OBJECT>* func,
                             REFMAP& id_map);

// This function only works for calling static process_ methods
template <typename SEQUENCE, typename DATA, typename OBJECT>
inline void process_sequential_element (DOMNode* node,
                                        DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        SEQUENCE& seq,
                                        Process_Function <OBJECT>* func,
                                        REFMAP& id_map);

#include "Process_Element.i"

#endif // PROCESS_ELEMENT_H

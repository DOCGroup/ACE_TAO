// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    removing_closure_visitor.h
//
// = DESCRIPTION
//    A DOM tree visitor.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDL_TO_PICML_REMOVING_CLOSURE_VISITOR_H
#define IDL_TO_PICML_REMOVING_CLOSURE_VISITOR_H

#include "xme_dom_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XercesString.h"

using namespace xercesc;

class removing_closure_visitor : public xme_dom_visitor
{
  //
  // = TITLE
  //    removing_closure_visitor.
  //
  // = DESCRIPTION
  //    PICML IDL importer DOM visitor. This visitor traverses
  //    all the GME folders under the root folder (except InterfaceDefinitions)
  //    and removes (1) GME references that refer to a removed element,
  //    and (2) GME connections that have a removed element as an
  //    endpoint.
  //
public:
  removing_closure_visitor (const XMLCh *id);
  
protected:
  virtual bool visit_gme_folder (DOMElement *node);
  virtual bool visit_gme_model (DOMElement *node);
  virtual bool visit_gme_reference (DOMElement *node);
  virtual bool visit_gme_connection (DOMElement *node);
  virtual bool visit_gme_set (DOMElement *node);
  
private:
  void check_endpoint (DOMElement *endpoint, DOMElement *node);
  bool derived_from (DOMElement *node);
  bool remove_and_do_closure (DOMElement *node);
  
private:
  XStr id_;
};

 #endif /* IDL_TO_PICML_REMOVING_CLOSURE_VISITOR_H */

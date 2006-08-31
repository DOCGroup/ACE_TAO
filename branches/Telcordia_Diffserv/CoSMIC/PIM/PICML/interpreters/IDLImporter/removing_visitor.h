// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    removing_visitor.h
//
// = DESCRIPTION
//    A DOM tree visitor.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDL_TO_PICML_REMOVING_VISITOR_H
#define IDL_TO_PICML_REMOVING_VISITOR_H

#include "xme_dom_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using namespace xercesc;

class removing_visitor : public xme_dom_visitor
{
  //
  // = TITLE
  //    removing_visitor
  //
  // = DESCRIPTION
  //    PICML IDL importer DOM visitor. This visitor traverses
  //    the GME InterfaceDefinitions folder (after adding_visitor
  //    is finished adding to or modifying it) and either removes
  //    an element that does not have a counterpart in the AST,
  //    modifies attributesor children as necessary, or leaves
  //    the element unchanged.
  //
public:
  removing_visitor (void);

protected:
  virtual bool visit_gme_folder (DOMElement *node);
  virtual bool visit_gme_model (DOMElement *node);
  virtual bool visit_gme_atom (DOMElement *node);
  virtual bool visit_gme_reference (DOMElement *node);
  virtual bool visit_gme_connection (DOMElement *node);
  
private:  
  bool found_in_idl (DOMElement *node);
  bool do_closure (void);
  
private:
  const XMLCh *id_holder_;
};

 #endif /* IDL_TO_PICML_REMOVING_VISITOR_H */

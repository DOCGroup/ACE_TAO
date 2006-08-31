// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_PICML_BE_DLL
//
// = FILENAME
//    xme_dom_visitor.h
//
// = DESCRIPTION
//    XME DOM tree visitor base class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDL_TO_PICML_XME_DOM_VISITOR_H
#define IDL_TO_PICML_XME_DOM_VISITOR_H

#include <xercesc/dom/DOM.hpp>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

using namespace xercesc;

class xme_dom_visitor
{
  //
  // = TITLE
  //    xme_dom_visitor
  //
  // = DESCRIPTION
  //    PICML IDL importer DOM visitor base class. This visitor 
  //    declares the methods that are overridden in the other DOM
  //    tree visitors in this library. The first two protected
  //    methods are defined here, since they are used unchanged in
  //    derived clases. The rest of the methods are not pure
  //    virtual, but defined as no-ops, so derived classes need
  //    to declare only the ones they need.
  //
public:
  bool visit_root (DOMElement *node);
  
protected:
  bool visit_children (DOMElement *node);
  
  // Visit methods for DOM node types.
  bool visit_dom_element (DOMElement *node);
  bool visit_dom_text (DOMText *node);
  bool visit_dom_attribute (DOMAttr *node);
  
  // Visit methods for metaGME types (indicated by
  // XML tag names).
  virtual bool visit_gme_folder (DOMElement *node);
  virtual bool visit_gme_model (DOMElement *node);
  virtual bool visit_gme_atom (DOMElement *node);
  virtual bool visit_gme_reference (DOMElement *node);
  virtual bool visit_gme_connection (DOMElement *node);
  virtual bool visit_gme_attribute (DOMElement *node);
  virtual bool visit_gme_regnode (DOMElement *node);
  virtual bool visit_gme_name (DOMElement *node);
  virtual bool visit_gme_set (DOMElement *node);
};

 #endif /* IDL_TO_PICML_XME_DOM_VISITOR_H */

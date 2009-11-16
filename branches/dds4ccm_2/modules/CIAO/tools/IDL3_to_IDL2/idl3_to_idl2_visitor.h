// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL3_TO_IDL2_BE_DLL
//
// = FILENAME
//    idl3_to_idl2_visitor.h
//
// = DESCRIPTION
//    Generates equivalent IDL2 from IDL3.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_IDL3_TO_IDL2_VISITOR_H
#define TAO_IDL3_TO_IDL2_VISITOR_H

#include "basic_visitor.h"
#include "utl_scoped_name.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_IDL3_TO_IDL2_BE_Export.h"

class TAO_IDL3_TO_IDL2_BE_Export idl3_to_idl2_visitor : public basic_visitor
{
  //
  // = TITLE
  //    idl3_to_idl2_visitor.
  //
  // = DESCRIPTION
  //    IDL3 to IDL2 conversion visitor. The methods are
  //    not pure virtual to facilitate the implementation of some
  //    derived visitors that override only a few.
  //
public:
  idl3_to_idl2_visitor (void);
  virtual ~idl3_to_idl2_visitor (void);

  virtual int visit_module (AST_Module *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_provides (AST_Provides *node);
  virtual int visit_uses (AST_Uses *node);
  virtual int visit_publishes (AST_Publishes *node);
  virtual int visit_emits (AST_Emits *node);
  virtual int visit_consumes (AST_Consumes *node);
  virtual int visit_extended_port (AST_Extended_Port *node);
  virtual int visit_mirror_port (AST_Mirror_Port *node);
  virtual int visit_connector (AST_Connector *node);
  virtual int visit_instantiated_connector (
    AST_Instantiated_Connector *node);
  virtual int visit_tmpl_port (AST_Tmpl_Port *node);
  virtual int visit_tmpl_mirror_port (
    AST_Tmpl_Mirror_Port *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
  virtual int visit_root (AST_Root *node);

private:
  UTL_ScopedName *create_scoped_name (const char *prefix,
                                      const char *local_name,
                                      const char *suffix,
                                      AST_Decl *parent);
  void tranfer_scope_elements (AST_Home *src, AST_Interface &dst);
  void gen_factories (AST_Home *node, AST_Interface &xplicit);
  void gen_finders (AST_Home *node, AST_Interface &xplicit);
};

#endif /* TAO_IDL3_TO_IDL2_VISITOR_H */

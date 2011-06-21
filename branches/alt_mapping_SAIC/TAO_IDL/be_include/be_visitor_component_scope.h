/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_component_scope.h
 *
 *  $Id$
 *
 *  Base class for visitors that need to visit the scope of a
 *  component and its ancestors, if any.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SCOPE_H_
#define _BE_COMPONENT_COMPONENT_SCOPE_H_

#include "ace/SString.h"

#include "be_visitor_scope.h"

/**
 * @class be_visitor_component_scope
 *
 * @brief be_visitor_component_scope
 *
 * This is an abstract visitor providing a scope visitation
 * method that traverses the base component recursively.
 */
class be_visitor_component_scope : public be_visitor_scope
{
protected:
  be_visitor_component_scope (be_visitor_context *ctx);

  virtual ~be_visitor_component_scope (void);

public:
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);

  /// Automatically iterates over ancestor scopes, if any.
  int visit_component_scope (be_component *node);

  /// These two methods are used instead of overriding
  // /visit_porttype so we can traverse a porttype only
  /// when it is referenced.

  int visit_porttype_scope (be_porttype *node);

  /// Swaps uses for provides members and vice versa.S
  int visit_porttype_scope_mirror (be_porttype *node);

  /// Set the member.
  void node (be_component *c);

protected:
  void gen_svnt_entrypoint_decl (void);
  void gen_svnt_entrypoint_defn (void);
  void gen_exec_entrypoint_decl (void);
  void gen_exec_entrypoint_defn (void);

protected:
  be_component *node_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
  bool in_ext_port_;
};

#endif /* _BE_COMPONENT_COMPONENT_SCOPE_H_ */


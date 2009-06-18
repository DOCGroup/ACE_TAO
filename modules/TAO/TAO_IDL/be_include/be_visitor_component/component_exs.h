//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    component_exs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the exec impl source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EXS_H_
#define _BE_COMPONENT_COMPONENT_EXS_H_

class be_visitor_component_exs : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the exec
  //   impl for component.
  //
  //
public:
  be_visitor_component_exs (be_visitor_context *ctx);
  
  ~be_visitor_component_exs (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  
private:
  int gen_facets (void);
  
  int gen_facet_ops_attrs (be_interface *node);
                           
  int gen_exec_class (void);
  
  int gen_component_attrs_r (AST_Component *node);
                     
  void gen_provides_r (AST_Component *node);
   
  void gen_provides (AST_Type *obj,
                     Identifier *port_id);
                                     
  void gen_consumes_r (AST_Component *node);
  
  void gen_consumes (AST_Type *obj,
                     Identifier *port_id);
                                     
  /// Base class overrides and CIAO-specific code.                
  void gen_non_type_specific (void);
  
  void gen_entrypoint (void);
                  
private:
  be_component *node_;
  be_interface *op_scope_;
  TAO_OutStream &os_;
  const char *comment_border_;
  const char *your_code_here_;
  ACE_CString export_macro_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Component_Exec_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Component_Exec_Op_Attr_Generator (be_visitor_scope * visitor);
  
  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};
                     
#endif /* _BE_COMPONENT_COMPONENT_EXS_H_ */


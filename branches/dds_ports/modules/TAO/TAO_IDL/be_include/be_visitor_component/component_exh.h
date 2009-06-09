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
//    component_exh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the exec impl header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EXH_H_
#define _BE_COMPONENT_COMPONENT_EXH_H_

class be_visitor_component_exh : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant header for component.
  //
  //
public:
  be_visitor_component_exh (be_visitor_context *ctx);
  
  ~be_visitor_component_exh (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  
private:
  enum port_kind
  {
    PROVIDES,
    USES,
    PUBLISHES,
    CONSUMES,
    EMITS,
    NUM_PORT_KINDS
  };
  
  int gen_facets (void);
  
  int gen_facet_ops_attrs (be_interface *node);
                           
  int gen_exec_class (void);
  
  int gen_component_attrs_r (AST_Component *node);
                     
  void gen_ports (AST_Component *node,
                  port_kind kind);
                  
  void gen_provides_r (AST_Component *node);
   
  void gen_provides (AST_Type *obj,
                     const char *port_name);
                                     
  void gen_consumes_r (AST_Component *node);
  
  void gen_consumes (const char *obj_name,
                     const char *port_name);
                                     
  /// Base class overrides and CIAO-specific code.                
  void gen_non_type_specific (void);
  
  void gen_entrypoint (void);
                  
  /// Helper method passed to traverse_inheritance_graph(),
  /// collects supported operations and attributes.
  static int op_attr_decl_helper (be_interface *,
                                  be_interface *,
                                  TAO_OutStream *);
                     
private:
  be_component *node_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_EXH_H_ */


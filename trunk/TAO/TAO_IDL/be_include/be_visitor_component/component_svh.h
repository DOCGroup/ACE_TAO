//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the servant header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVH_H_
#define _BE_COMPONENT_COMPONENT_SVH_H_

class be_visitor_component_svh : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant header for component.
  //
  //
public:
  be_visitor_component_svh (be_visitor_context *ctx);
  
  ~be_visitor_component_svh (void);
  
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
  
  void gen_context_class (void);
                           
  void gen_context_r (AST_Component *node);
  
  int gen_servant_class (void);
  
  int gen_servant_r (AST_Component *node);
  
  int gen_component_attrs (AST_Component *node);
                     
  void gen_ports (AST_Component *node,
                  port_kind kind,
                  bool in_servant);
   
  void gen_provides (AST_Component::port_description *pd);
                                     
  void gen_uses (AST_Component::port_description *pd,
                 bool in_servant);
                
  void gen_publishes (const char *obj_name,
                      const char *port_name,
                      bool in_servant);
  
  void gen_consumes (const char *obj_name,
                     const char *port_name);
                                     
  void gen_emits (const char *obj_name,
                  const char *port_name,
                  bool in_servant);
     
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
  bool in_ancestor_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVH_H_ */


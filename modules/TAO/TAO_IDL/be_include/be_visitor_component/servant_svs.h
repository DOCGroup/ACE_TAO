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
//    servant_svs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the Servant
//    class definition
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_SERVANT_SVS_H_
#define _BE_COMPONENT_SERVANT_SVS_H_

class be_visitor_servant_svs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_servant_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant class decl
  //   for a component.
  //
  //
public:
  be_visitor_servant_svs (be_visitor_context *ctx);
  
  ~be_visitor_servant_svs (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);
  
private:
  int gen_servant_r (be_component *node);
  
  void compute_slots (AST_Component *node);
                  
  void gen_obv_factory_registration_r (AST_Component *node);
  void gen_obv_factory_registration (AST_Type *t);
                  
  int gen_attr_set_r (AST_Component *node);

  void gen_provides_top (void);
  
  void gen_facet_executor_block (const char *port_name);
                  
  void gen_uses_top (void);
  
  void gen_connect_block (AST_Uses *u);
                          
  void gen_disconnect_block (AST_Uses *u);
                             
  void gen_receptacle_description (AST_Uses *u,
                                   ACE_CDR::ULong slot);
  
  void gen_publishes_top (void);
  
  void gen_subscribe_block (AST_Publishes *p);
                            
  void gen_unsubscribe_block (AST_Publishes *p);
                              
  void gen_event_source_description (AST_Publishes *p,
                                     ACE_CDR::ULong slot);
                                     
  void gen_emits_top (void);
  
  void gen_connect_consumer_block (AST_Emits *e);
  
  void gen_disconnect_consumer_block (AST_Emits *e);
                                      
  void gen_emitter_description (AST_Emits *e,
                                ACE_CDR::ULong slot);
  
  void gen_populate_r (AST_Component *node);
  
private:
  be_component *node_;
  be_interface *op_scope_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
  bool swapping_;
  ACE_CDR::ULong n_provides_;
  ACE_CDR::ULong n_uses_;
  ACE_CDR::ULong n_publishes_;
  ACE_CDR::ULong n_emits_;
  ACE_CDR::ULong n_consumes_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Component_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Component_Op_Attr_Generator (be_visitor_scope * visitor);
  
  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};
                     
#endif /* _BE_COMPONENT_SERVANT_SVS_H_ */


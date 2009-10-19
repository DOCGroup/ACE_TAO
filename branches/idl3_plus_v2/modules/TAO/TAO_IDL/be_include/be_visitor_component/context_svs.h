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
//    context_svs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the Context
//    class definition
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_CONTEXT_SVS_H_
#define _BE_COMPONENT_CONTEXT_SVS_H_

class be_visitor_context_svs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_context_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the context class defn
  //   for a component.
  //
  //
public:
  be_visitor_context_svs (be_visitor_context *ctx);
  
  ~be_visitor_context_svs (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_extended_port (be_extended_port *node);
  virtual int visit_mirror_port (be_mirror_port *node);
  
private:
  int gen_context_r (be_component *node);
  
  void gen_swapping_get_consumers_r (AST_Component *node);
  void gen_swapping_get_consumer_block (const char *port_name);
  
  void gen_uses_simplex (AST_Type *obj,
                         const char *port_name);       
  void gen_uses_multiplex (AST_Type *obj,
                           const char *port_name);
                                   
  
private:
  be_component *node_;
  TAO_OutStream &os_;
  bool swapping_;
  bool static_config_;
};

#endif /* _BE_COMPONENT_CONTEXT_SVS_H_ */


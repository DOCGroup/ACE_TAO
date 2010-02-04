/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    context_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the Context
 *  class definition
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_CONTEXT_SVS_H_
#define _BE_COMPONENT_CONTEXT_SVS_H_

/**
 * @class be_visitor_context_svs
 *
 * @brief be_visitor_context_svs
 *
 * This is a concrete visitor to generate the context class defn
 * for a component.
 */
class be_visitor_context_svs : public be_visitor_component_scope
{
public:
  be_visitor_context_svs (be_visitor_context *ctx);
  
  ~be_visitor_context_svs (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  
private:
  void gen_uses_simplex (AST_Type *obj,
                         const char *port_name);       
  void gen_uses_multiplex (AST_Type *obj,
                           const char *port_name);
};

// ======================================================

class be_visitor_swapping_get_consumer
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_swapping_get_comsumer
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate a block of code
  //   for each publishes port, including inherited ports.
  //
  //
public:
  be_visitor_swapping_get_consumer (be_visitor_context *ctx);
  
  ~be_visitor_swapping_get_consumer (void);
  
  virtual int visit_publishes (be_publishes *node);
};

#endif /* _BE_COMPONENT_CONTEXT_SVS_H_ */


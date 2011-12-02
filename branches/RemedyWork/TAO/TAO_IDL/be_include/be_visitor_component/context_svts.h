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
 * @class be_visitor_context_svts
 *
 * @brief be_visitor_context_svts
 *
 * This is a concrete visitor to generate the context class defn
 * for a component.
 */
class be_visitor_context_svts : public be_visitor_component_scope
{
public:
  be_visitor_context_svts (be_visitor_context *ctx);

  ~be_visitor_context_svts (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);

private:
  void gen_uses_simplex (AST_Type *obj,
                         const char *port_name);
  void gen_uses_multiplex (AST_Type *obj,
                           const char *port_name);
};

#endif /* _BE_COMPONENT_CONTEXT_SVS_H_ */


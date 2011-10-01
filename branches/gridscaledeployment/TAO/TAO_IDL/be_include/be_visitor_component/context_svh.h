/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    context_svh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the Context
 *  class declaration
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_CONTEXT_SVH_H_
#define _BE_COMPONENT_CONTEXT_SVH_H_

/**
 * @class be_visitor_context_svh
 *
 * @brief be_visitor_context_svh
 *
 * This is a concrete visitor to generate the context class decl
 * for a component.
 */
class be_visitor_context_svh : public be_visitor_component_scope
{
public:
  be_visitor_context_svh (be_visitor_context *ctx);

  ~be_visitor_context_svh (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
};

#endif /* _BE_COMPONENT_CONTEXT_SVH_H_ */


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
//    context_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the Context
//    class declaration
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_CONTEXT_SVH_H_
#define _BE_COMPONENT_CONTEXT_SVH_H_

class be_visitor_context_svh : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_context_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the context class decl
  //   for a component.
  //
  //
public:
  be_visitor_context_svh (be_visitor_context *ctx);
  
  ~be_visitor_context_svh (void);
  
  virtual int visit_component (be_component *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
};

#endif /* _BE_COMPONENT_CONTEXT_SVH_H_ */


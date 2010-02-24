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
//    context_ex_idl.h
//
// = DESCRIPTION
//    Provides code generation for the context class
//    in the CIAO executor IDL.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_CONTEXT_EX_IDL_H_
#define _BE_COMPONENT_CONTEXT_EX_IDL_H_

class be_visitor_context_ex_idl
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_context_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the
  //   CIAO context IDL.
  //
  //
public:
  be_visitor_context_ex_idl (be_visitor_context *ctx);

  ~be_visitor_context_ex_idl (void);

  virtual int visit_component (be_component *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
};

#endif /* _BE_COMPONENT_CONTEXT_EX_IDL_H_ */


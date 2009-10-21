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
//    component_ex_idl.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    Provides code generation for components in the CIAO executor IDL
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EX_IDL_H_
#define _BE_COMPONENT_COMPONENT_EX_IDL_H_

class be_visitor_component_ex_idl : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_component_ex_idl
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the 
  //   CIAO executor IDL for component.
  //
  //
public:
  be_visitor_component_ex_idl (be_visitor_context *ctx);

  ~be_visitor_component_ex_idl (void);

  virtual int visit_component (be_component *node);
  
private:
  TAO_OutStream &os_;
};

#endif /* _BE_COMPONENT_COMPONENT_EX_IDL_H_ */

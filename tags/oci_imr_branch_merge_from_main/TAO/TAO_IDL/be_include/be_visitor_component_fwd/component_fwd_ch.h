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
//    component_fwd_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Component Forward node.
//    This one provides code generation for component forward node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_FWD_CH_H_
#define _BE_COMPONENT_COMPONENT_FWD_CH_H_

class be_visitor_component_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_component_fwd_ch
  //
  // = DESCRIPTION
  //   This is the visitor for component_fwd for the header file
  //
  //
public:
  be_visitor_component_fwd_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_fwd_ch (void);
  // destructor

  virtual int visit_component_fwd (be_component_fwd *node);
  // visit interface_fwd.
};

#endif /* _BE_COMPONENT_COMPONENT_FWD_CH_H_ */

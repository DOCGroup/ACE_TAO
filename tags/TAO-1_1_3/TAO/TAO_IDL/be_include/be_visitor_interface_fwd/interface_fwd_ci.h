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
//    interface_fwd_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface Forward node.
//    This one provides code generation for interface forward node.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_FWD_CI_H_
#define _BE_INTERFACE_INTERFACE_FWD_CI_H_

class be_visitor_interface_fwd_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_fwd_ci
  //
  // = DESCRIPTION
  //   This is the visitor for interface_fwd for the inline file
  //
  //
public:
  be_visitor_interface_fwd_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_fwd_ci (void);
  // destructor

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd

};

#endif /* _BE_INTERFACE_INTERFACE_FWD_CH_I_ */

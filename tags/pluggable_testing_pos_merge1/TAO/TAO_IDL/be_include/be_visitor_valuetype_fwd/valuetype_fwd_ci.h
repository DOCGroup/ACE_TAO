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
//    valuetype_fwd_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface Forward node.
//    This one provides code generation for valuetype forward node.
//
// = AUTHOR
//    Torsten Kuepper
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_FWD_CI_H_
#define _BE_INTERFACE_INTERFACE_FWD_CI_H_

class be_visitor_valuetype_fwd_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_valuetype_fwd_ci
  //
  // = DESCRIPTION
  //   This is the visitor for valuetype_fwd for the inline file
  //
  //
public:
  be_visitor_valuetype_fwd_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_fwd_ci (void);
  // destructor

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype_fwd

};

#endif /* _BE_INTERFACE_INTERFACE_FWD_CH_I_ */

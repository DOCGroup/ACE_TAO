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
//    valuetype_fwd_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype Forward node.
//    This one provides code generation for valuetype forward node.
//
// = AUTHOR
//    Torsten Kuepper
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_FWD_CH_H_
#define _BE_VISITOR_VALUETYPE_FWD_CH_H_

class be_visitor_valuetype_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_valuetype_fwd_ch
  //
  // = DESCRIPTION
  //   This is the visitor for valuetype_fwd for the header file
  //
  //
public:
  be_visitor_valuetype_fwd_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_fwd_ch (void);
  // destructor

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
};

#endif /* _BE_VISITOR_VALUETYPE_FWD_CH_H_ */

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
//    union_fwd_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the be_union_fwd node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_INTERFACE_UNION_FWD_CH_H_
#define _BE_INTERFACE_UNION_FWD_CH_H_

class be_visitor_union_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_fwd_ch
  //
  // = DESCRIPTION
  //   This is the visitor for be_union_fwd for the header file
  //
  //
public:
  be_visitor_union_fwd_ch (be_visitor_context *ctx);

  ~be_visitor_union_fwd_ch (void);

  virtual int visit_union_fwd (be_union_fwd *node);
};

#endif /* _BE_INTERFACE_UNION_FWD_CH_H_ */

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
//    eventtype_fwd_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype Forward node.
//    This one provides code generation for eventtype forward node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EVENTTYPE_FWD_CH_H_
#define _BE_VISITOR_EVENTTYPE_FWD_CH_H_

class be_visitor_eventtype_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_eventtype_fwd_ch
  //
  // = DESCRIPTION
  //   This is the visitor for eventtype_fwd for the header file
  //
  //
public:
  be_visitor_eventtype_fwd_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_fwd_ch (void);
  // destructor

  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  // visit valuetype_fwd.
};

#endif /* _BE_VISITOR_EVENTTYPE_FWD_CH_H_ */

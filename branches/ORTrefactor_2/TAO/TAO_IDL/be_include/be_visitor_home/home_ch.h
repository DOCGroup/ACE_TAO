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
//    home_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Component Home node.
//    This one provides code generation for component home node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_HOME_CH_H_
#define _BE_VISITOR_HOME_CH_H_

class be_visitor_home_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_home_ch
  //
  // = DESCRIPTION
  //   This is the visitor for component home for the header file
  //
  //
public:
  be_visitor_home_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_home_ch (void);
  // destructor

  virtual int visit_home (be_home *node);
  // visit home.
};

#endif /* _BE_VISITOR_HOME_CH_H_ */

/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    constant_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the constant class generating code in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_CONSTANT_CONSTANT_CH_H_
#define _BE_VISITOR_CONSTANT_CONSTANT_CH_H_

class be_visitor_constant_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_constant_ch
  //
  // = DESCRIPTION
  //   This is the visitor for constant for the header file
  //
  //
public:
  be_visitor_constant_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_constant_ch (void);
  // destructor

  virtual int visit_constant (be_constant *node);
  // visit constant.
};

#endif /* _BE_VISITOR_CONSTANT_CONSTANT_CH_H_ */

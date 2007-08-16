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
//    native_ch.h
//
// = DESCRIPTION
//    Concrete visitor for Native generating code in the client header.
//
// = AUTHOR
//    Johnny Willemsen
//
// ============================================================================

#ifndef _BE_VISITOR_NATIVE_NATIVE_CH_H_
#define _BE_VISITOR_NATIVE_NATIVE_CH_H_

class be_visitor_native_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_native_ch
  //
  // = DESCRIPTION
  //   This is the base visitor for enum for the header file
  //
  //
public:
  be_visitor_native_ch (be_visitor_context *ctx);
  // conenumor

  ~be_visitor_native_ch (void);
  // deenumor

  virtual int visit_native (be_native *node);
  // visit native.
};

#endif /* _BE_VISITOR_NATIVE_NATIVE_CH_H_ */

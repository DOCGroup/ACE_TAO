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
//    valuetype_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype node.
//    This one provides code generation for valuetypes in the (client) header.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_CH_H_
#define _BE_VALUETYPE_VALUETYPE_CH_H_

// we need derived valuetype visitors for the client and server header files. For
// the others, they use the default valuetype visitor

class be_visitor_valuetype_ch : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for valuetype
  //
  //
public:
  be_visitor_valuetype_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_ch (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_operation (be_operation *node);
  virtual int visit_field (be_field *node);

  void begin_public (void);
  void begin_private (void);

  static int gen_supported_ops (be_interface *node,
                                be_interface *base,
                                TAO_OutStream *os);
  // Helper method passed to the template method to generate code for
  // the operations of the parents of supported interfaces.
};

#endif /* _BE_VALUETYPE_VALUETYPE_CH_H_ */

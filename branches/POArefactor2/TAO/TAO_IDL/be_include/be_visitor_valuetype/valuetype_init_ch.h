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
//    valuetype_init_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype node.
//    This one provides code generation for valuetype factory
//    in the (client) header.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_INIT_CH_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_CH_H_


class be_visitor_valuetype_init_ch : public be_visitor_valuetype_init
{
  //
  // = TITLE
  //   be_visitor_valuetype_init_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate valuetype factory in
  //   the client header
  //
  //
public:
  be_visitor_valuetype_init_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_init_ch (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // entry point

  virtual int visit_factory (be_factory *node);
  // visit a factory
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_CH_H_ */

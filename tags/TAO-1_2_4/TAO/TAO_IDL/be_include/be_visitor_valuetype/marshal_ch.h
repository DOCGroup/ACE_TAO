/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    marshal_ch.h
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for marshalling.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_
#define _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_

class be_visitor_valuetype_marshal_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_valuetype_marshal_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_valuetype_marshal_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_marshal_ch (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

};

#endif /* _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_ */

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
//    marshal_cs.h
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for marshalling.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_
#define _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_

class be_visitor_valuetype_marshal_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_valuetype_marshal_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_valuetype_marshal_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_marshal_cs (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  void class_name (be_valuetype *node, TAO_OutStream *os);
  int gen_fields (be_valuetype *node, be_visitor_context &ctx);
};

#endif /* _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_ */

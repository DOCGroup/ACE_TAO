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
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//    based on code from Torsten Kuepper <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_
#define _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_

class be_visitor_valuetype_cdr_op_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuetype that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_valuetype_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_cdr_op_cs (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype

  virtual int visit_field (be_field *node);
  // visit field

  virtual int visit_sequence (be_sequence *node);
  // visit sequence
};

#endif /* _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_ */

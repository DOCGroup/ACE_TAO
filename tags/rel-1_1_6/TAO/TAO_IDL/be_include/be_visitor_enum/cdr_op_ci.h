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
//    cdr_op_ci.h
//
// = DESCRIPTION
//    Concrete visitor for Enums generating code for the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_CDR_OP_CI_H_
#define _BE_VISITOR_ENUM_CDR_OP_CI_H_

class be_visitor_enum_cdr_op_ci : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_enum_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_cdr_op_ci (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

#endif /* _BE_VISITOR_ENUM_CDR_OP_CI_H_ */

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
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for Enums generating code for the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_CDR_OP_CS_H_
#define _BE_VISITOR_ENUM_CDR_OP_CS_H_

class be_visitor_enum_cdr_op_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_enum_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_cdr_op_cs (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

#endif /* _BE_VISITOR_ENUM_CDR_OP_CS_H_ */

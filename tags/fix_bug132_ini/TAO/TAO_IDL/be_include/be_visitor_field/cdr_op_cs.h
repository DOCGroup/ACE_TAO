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
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//    This generates code for structure members in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_FIELD_CDR_OP_CS_H_
#define _BE_VISITOR_FIELD_CDR_OP_CS_H_

#include "be_visitor_scope.h"

class be_visitor_field_cdr_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_field_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_cdr_op_cs (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);

};

#endif /*  _BE_VISITOR_FIELD_CDR_OP_CS_H_ */

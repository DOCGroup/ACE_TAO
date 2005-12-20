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
//    cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//    This generates code for structure members in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_FIELD_CDR_OP_CH_H_
#define _BE_VISITOR_FIELD_CDR_OP_CH_H_

class be_visitor_field_cdr_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client header.
  //
public:
  be_visitor_field_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_cdr_op_ch (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type
};

#endif /*  _BE_VISITOR_FIELD_CDR_OP_CH_H_ */

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
//    any_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//    This one provides code generation for the Any operators for the structure
//    in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_
#define _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_

class be_visitor_structure_any_op_ch : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the Any operator
  //   declarations
  //

public:
  be_visitor_structure_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_any_op_ch (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int visit_field (be_field *node);
  // visit field

  virtual int visit_union (be_union *node);
  // visit union

  virtual int visit_enum (be_enum *node);
  // visit field
};

#endif /* _BE_VISITOR_STRUCTURE_ANY_OP_CH_H_ */

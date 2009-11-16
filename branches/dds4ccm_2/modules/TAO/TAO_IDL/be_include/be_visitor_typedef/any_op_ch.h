
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
//    Concrete visitor for the Typedef class
//    This provides the code generation for the Any operators of the Typedef
//    class in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_ANY_OP_CH_H_
#define _BE_VISITOR_TYPEDEF_ANY_OP_CH_H_

class be_visitor_typedef_any_op_ch : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for typedef that generates the Any operator
  //   declarations and implementations
  //

public:
  be_visitor_typedef_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_any_op_ch (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

#endif /* _BE_VISITOR_TYPEDEF_ANY_OP_CH_H_*/

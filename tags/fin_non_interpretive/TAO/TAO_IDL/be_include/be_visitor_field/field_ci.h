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
//    field_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//    This generates code for structure members in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_FIELD_CI_H_
#define _BE_VISITOR_FIELD_CI_H_

class be_visitor_field_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client inline
  //   file
  //
public:
  be_visitor_field_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_ci (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be and for
  // which inline code must be generated

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

#endif /*  _BE_VISITOR_FIELD_CI_H_ */

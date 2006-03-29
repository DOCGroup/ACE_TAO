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
//    field_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Field" node
//    This generates code for structure members in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_FIELD_CS_H_
#define _BE_VISITOR_FIELD_CS_H_

class be_visitor_field_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_field_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_field node for the client stubs
  //   file
  //
public:
  be_visitor_field_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_field_cs (void);
  // destructor

  virtual int visit_field (be_field *node);
  // visit the field node

  // =visit operations on all possible data types that a field can be and impl
  // code must be generated

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

#endif /*  _BE_VISITOR_FIELD_CS_H_ */
